#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "cJSON.h"
#include <signal.h>
#include <syslog.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

#include "sensor_fx.h"

#define PORT 8080
#define MAX_CLIENTS 5
#define SENSOR_PIN 0

int fd;

void signalHandler(int signal) {
    syslog(LOG_INFO, "Received signal: %d. Shutting down server.", signal);
    closeI2CDevice(fd);
    exit(EXIT_SUCCESS);
}

void setSocketNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

void daemonize() {
    pid_t pid, sid;

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }

    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
}

void readSensorData(float *temperature, float *pressure) {
    // sensor values
    syslog(LOG_INFO, "I2C File Descriptor : %i", fd);
    
    *temperature = getTemperatureReading(fd);
    *pressure    = getPressureReading(fd);
}

void *handleClient(void *arg) {
    int *clientSocketPtr = (int *)arg;
    int clientSocket = *clientSocketPtr;

    time_t currentTime;
    char formattedTime[20];
    struct tm *timeInfo;
    char buffer[1024];

    setSocketNonBlocking(clientSocket);

    while (1) {
        float temperature, pressure;
        readSensorData(&temperature, &pressure);

	time(&currentTime);

	//Format time
        timeInfo = localtime(&currentTime);
        strftime(formattedTime, sizeof(formattedTime), "%m/%d/%Y %H:%M:%S", timeInfo);

        cJSON *jsonObject = cJSON_CreateObject();
        cJSON_AddStringToObject(jsonObject, "time", formattedTime);
        cJSON_AddNumberWithPrecisionToObject(jsonObject, "temperature", temperature,2);
        cJSON_AddNumberWithPrecisionToObject(jsonObject, "pressure", pressure,2);

        char *jsonStr = cJSON_Print(jsonObject);

	int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
		// No Data received -- ignore
            } else {
                // Error
                break;
            }
        } else if (bytesRead == 0) {
            // Client disconnected, break out of the loop
            break;
        } else {
            // Process received data if required
        }
        

        send(clientSocket, jsonStr, strlen(jsonStr), 0);

        cJSON_Delete(jsonObject);
        free(jsonStr);

        sleep(5);
    }

    close(clientSocket);

    return NULL;
}

int main() {
    openlog("SensorServer", LOG_PID, LOG_DAEMON);

    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

    syslog(LOG_INFO, "Sensor server starting...");

    fd = openI2CDevice();

    daemonize();

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    pthread_t clientThreads[MAX_CLIENTS];
    int clientCount = 0;
    
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        syslog(LOG_ERR, "Socket creation failed");
        closelog();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        syslog(LOG_ERR, "Socket bind failed");
        closelog();
        return 1;
    }

    if (listen(serverSocket, 5) == -1) {
        syslog(LOG_ERR, "Socket listen failed");
        closelog();
        return 1;
    }

    syslog(LOG_INFO, "Server listening on port %d...", PORT);

    while (1) {
        int clientAddrLen = sizeof(clientAddr);
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&clientAddrLen)) == -1) {
            syslog(LOG_ERR, "Connection accept failed");
            continue;
        }

        syslog(LOG_INFO, "Client connected: %s", inet_ntoa(clientAddr.sin_addr));

        if (pthread_create(&clientThreads[clientCount], NULL, handleClient, (void *)&clientSocket) != 0) {
            syslog(LOG_ERR, "Thread creation failed");
            close(clientSocket);
            continue;
        }

        clientCount++;

        if (clientCount >= MAX_CLIENTS) {
            syslog(LOG_INFO, "Maximum number of clients reached.");
            break;
        }
    }

    for (int i = 0; i < clientCount; i++) {
        pthread_join(clientThreads[i], NULL);
    }

    closelog();
    close(serverSocket);

    return 0;
}

