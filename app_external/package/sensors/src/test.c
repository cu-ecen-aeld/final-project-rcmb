#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define  DEV_PATH "/dev/i2c-1"
#define  DEV_ID   0x60
#define  WHOAMI   0x0C

int main(void) {

    int fd = 0;

    printf("Test 1.0\n");

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

    if (i2c_smbus_read_byte_data(fd, WHOAMI) != 0xC4) {
        perror("Device ID error\n");
        close(fd);
        return 3;
    }

    

    int ctrl_reg = i2c_smbus_read_byte_data(fd,0x26);
    ctrl_reg = ctrl_reg | 0x01;
    i2c_smbus_write_byte_data(fd,0x26,ctrl_reg);
    
    int int_status = i2c_smbus_read_byte_data(fd,0x12);

    int temp = 0;

    printf("Interrupt status : %i\n", int_status);

    if (int_status == 0xc4) {
	temp = (i2c_smbus_read_byte_data(fd,0x04) << 8) + i2c_smbus_read_byte_data(fd,0x05);
    }

    printf("Temperature raw data : %i\n",temp);

    int t_m = (temp >> 8) & 0xFF;
    int t_l = temp & 0xFF;

    float calc_temp = 0;

    if (t_m > 0x7f) t_m = t_m - 256;
	 calc_temp = t_m + t_l / 256.0;

    printf("Temperature : %f\n",calc_temp);

    close(fd);

    printf("All pass\n");

    return 0;

}


