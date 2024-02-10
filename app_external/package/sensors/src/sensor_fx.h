
#define  DEV_PATH  "/dev/i2c-1"
#define  DEV_ID    0x60
#define  WHOAMI    0x0C
#define  OUT_P_MSB 0x01
#define  OUT_P_CSB 0x02
#define  OUT_P_LSB 0x03
#define  OUT_T_MSB 0x04
#define  OUT_T_LSB 0x05

#define  DEGREE_CENTI_FACTOR 256.0
#define  PASCAL_FACTOR       64       
#define  PSI_FACTOR          0.00014504

int   openI2CDevice(void);
float getPressureReading(int fd); 
float getTemperatureReading(int fd);
void  closeI2CDevice(int fd);


