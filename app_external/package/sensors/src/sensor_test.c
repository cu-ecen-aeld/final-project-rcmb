#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define  DEV_PATH  "/dev/i2c-1"
#define  DEV_ID    0x60
#define  WHOAMI    0x0C
#define  OUT_P_MSB 0x01
#define  OUT_P_CSB 0x02
#define  OUT_P_LSB 0x03
#define  OUT_T_MSB 0x04
#define  OUT_T_LSB 0x05

int main(void) {

    int fd = 0;

    printf("Test 2.0\n");

    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
        perror("Unable to open i2c device\n");
        return 1;
    }

    printf("Dev access : PASS\n");

    if (ioctl(fd, I2C_SLAVE, DEV_ID) < 0) {
        perror("Unable to configure i2c slave device\n");
        close(fd);
        return 2;
    }

    printf("Slave set : PASS\n");

    int who = i2c_smbus_read_byte_data(fd, WHOAMI);

    printf("Value of Who : %04X\n",who);

    if(who != 0xC4) {
        perror("Device ID error\n");
        close(fd);
       return 3;
    }

    int int_status = i2c_smbus_read_byte_data(fd,WHOAMI);
    int ctrl_reg = i2c_smbus_read_byte_data(fd,0x26);
    ctrl_reg = ctrl_reg | 0x01;
    i2c_smbus_write_byte_data(fd,0x26,ctrl_reg);
    sleep(1);

    int temp_raw = 0;
    __u8   data[5];
    float temp_deg = 0;
    int pres_raw = 0;
    float pres_pascal = 0;

    printf("Interrupt status : %i\n", int_status);

    if (int_status == 0xc4) {
        i2c_smbus_read_i2c_block_data(fd,(uint)OUT_P_MSB,(uint)5,data);
    }

    printf("Pressure raw data : %i - %i - %i\n",data[0],data[1],data[2]);
    printf("Temperature raw data : %i - %i\n",data[3],data[4]);

    pres_raw =  ((data[0] << 16) | (data[1] << 8) | (data[2]));
    temp_raw =  (data[3] << 8) | data[4];

    printf("Temperature raw : %i\n",temp_raw);
    temp_deg = temp_raw /  256.0;
    printf("Temperature in degrees :  %.2f\n",temp_deg);

    printf("Pressure raw : %i\n",pres_raw);
    pres_pascal = pres_raw /  64;
    printf("Pressure in Pascalas :  %.2f\n",pres_pascal);
    printf("Pressure in Psi : %.2f\n",pres_pascal*0.00014504);
    close(fd);

    printf("All pass\n");

    return 0;

}

