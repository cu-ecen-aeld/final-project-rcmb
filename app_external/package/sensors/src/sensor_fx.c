#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <syslog.h>
#include "sensor_fx.h"

int openI2CDevice(void) {

    int fd = 0;

    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
        syslog(LOG_ERR,"Unable to open i2c device\n");
        return -1;
    }

    printf("Dev access : PASS\n");

    if (ioctl(fd, I2C_SLAVE, DEV_ID) < 0) {
        syslog(LOG_ERR,"Unable to configure i2c slave device\n");
        close(fd);
        return -1;
    }

    return fd;
}

float getTemperatureReading(int fd) {

    int who = i2c_smbus_read_byte_data(fd, WHOAMI);

    if(who != 0xC4) {
        syslog(LOG_INFO, "Device ID error : %x", who);
        //close(fd);
        return -100;
    }

    int int_status = i2c_smbus_read_byte_data(fd,WHOAMI);
    int ctrl_reg = i2c_smbus_read_byte_data(fd,0x26);
    ctrl_reg = ctrl_reg | 0x01;
    i2c_smbus_write_byte_data(fd,0x26,ctrl_reg);
    sleep(1);

    int temp_raw = 0;
    __u8   data[5];
    float temp_deg = 0;

    if (int_status == 0xc4) {
        i2c_smbus_read_i2c_block_data(fd,(uint)OUT_P_MSB,(uint)5,data);
    }

    temp_raw =  (data[3] << 8) | data[4];
    temp_deg = temp_raw /  DEGREE_CENTI_FACTOR;

    return temp_deg;

}

float getPressureReading(int fd) {

    int who = i2c_smbus_read_byte_data(fd, WHOAMI);

    if(who != 0xC4) {
        syslog(LOG_INFO, "Device ID error : %x", who);
        //close(fd);
        return -100;
    }

    int int_status = i2c_smbus_read_byte_data(fd,WHOAMI);
    int ctrl_reg = i2c_smbus_read_byte_data(fd,0x26);
    ctrl_reg = ctrl_reg | 0x01;
    i2c_smbus_write_byte_data(fd,0x26,ctrl_reg);
    sleep(1);

    int pres_raw = 0;
    __u8  data[5];
    float pres_pascal = 0;
    float pres_psi = 0;

    if (int_status == 0xc4) {
        i2c_smbus_read_i2c_block_data(fd,(uint)OUT_P_MSB,(uint)5,data);
    }

    pres_raw =  ((data[0] << 16) | (data[1] << 8) | (data[2]));

    pres_pascal = pres_raw /  PASCAL_FACTOR;
    pres_psi = pres_pascal *  PSI_FACTOR;

    return pres_psi;

}

void closeI2CDevice(int fd) {
    syslog(LOG_INFO, "Closing I2C device");
    close(fd);
}

