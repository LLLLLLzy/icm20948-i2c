#ifndef __icm20948getdata__
#define __icm20948getdata__

#define ICM20948ADDRESS 0x68

enum GyroRange
    {
      GYRO_RANGE_250DPS,
      GYRO_RANGE_500DPS,
      GYRO_RANGE_1000DPS,
      GYRO_RANGE_2000DPS
    };
enum AccelRange
    {
      ACCEL_RANGE_2G,
      ACCEL_RANGE_4G,
      ACCEL_RANGE_8G,
      ACCEL_RANGE_16G    
    };
enum AccelDlpfBandwidth
    {
      ACCEL_DLPF_BANDWIDTH_1209HZ,
      ACCEL_DLPF_BANDWIDTH_246HZ,
      ACCEL_DLPF_BANDWIDTH_111HZ,
      ACCEL_DLPF_BANDWIDTH_50HZ,
      ACCEL_DLPF_BANDWIDTH_24HZ,
      ACCEL_DLPF_BANDWIDTH_12HZ,
      ACCEL_DLPF_BANDWIDTH_6HZ,
      ACCEL_DLPF_BANDWIDTH_473HZ
    };
enum GyroDlpfBandwidth
	{
		GYRO_DLPF_BANDWIDTH_12106HZ,
		GYRO_DLPF_BANDWIDTH_197HZ,
		GYRO_DLPF_BANDWIDTH_152HZ,
		GYRO_DLPF_BANDWIDTH_120HZ,
		GYRO_DLPF_BANDWIDTH_51HZ,
		GYRO_DLPF_BANDWIDTH_24HZ,
		GYRO_DLPF_BANDWIDTH_12HZ,
		GYRO_DLPF_BANDWIDTH_6HZ,
		GYRO_DLPF_BANDWIDTH_361HZ
	};
enum LpAccelOdr
	{
		LP_ACCEL_ODR_0_24HZ = 0,
		LP_ACCEL_ODR_0_49HZ = 1,
		LP_ACCEL_ODR_0_98HZ = 2,
		LP_ACCEL_ODR_1_95HZ = 3,
		LP_ACCEL_ODR_3_91HZ = 4,
		LP_ACCEL_ODR_7_81HZ = 5,
		LP_ACCEL_ODR_15_63HZ = 6,
		LP_ACCEL_ODR_31_25HZ = 7,
		LP_ACCEL_ODR_62_50HZ = 8,
		LP_ACCEL_ODR_125HZ = 9,
		LP_ACCEL_ODR_250HZ = 10,
		LP_ACCEL_ODR_500HZ = 11
	};
enum USERBANK
	{
		USER_BANK_0,
		USER_BANK_1,
		USER_BANK_2,
		USER_BANK_3,
	};

//为什么用const 会报错

//scale factors
#define _tempScale 333.87f
#define _tempOffset 21.0f



// buffer for reading from sensor
//unsigned char _buffer[21];


// constants
#define G  9.807f
#define Pi  3.14159265359f

#define accRawScaling 32767.5f // (                                                                                               
#define gyroRawScaling 32767.5f // (2^16-1)/2 16 bit representation of gyro value to cover +/- range
#define magRawScaling 32767.5f // (2^16-1)/2 16 bit representation of gyro value to cover +/- range

#define _magScale 4912.0f / magRawScaling; // micro Tesla, measurement range is +/- 4912 uT.



#define  ICM20948_WHO_AM_I 0xEA
//const signed char ICM20948_WHO_AM_I = 0xEA;


// ICM20948 registers
// User bank 0
#define UB0_WHO_AM_I  0x00
#define UB0_USER_CTRL  0x03
#define UB0_USER_CTRL_I2C_MST_EN  0x20

#define UB0_PWR_MGMNT_1  0x06
#define UB0_PWR_MGMNT_1_CLOCK_SEL_AUTO  0x01
#define UB0_PWR_MGMNT_1_DEV_RESET  0x80

#define UB0_PWR_MGMNT_2  0x07
#define UB0_PWR_MGMNT_2_SEN_ENABLE  0x00

#define UB0_INT_PIN_CFG  0x0F
#define UB0_INT_PIN_CFG_HIGH_50US  0x00

#define UB0_INT_ENABLE_1  0x11
#define UB0_INT_ENABLE_1_RAW_RDY_EN  0x01
#define UB0_INT_ENABLE_1_DIS  0x00


#define UB0_ACCEL_XOUT_H  0x2D
#define UB0_GYRO_XOUT_H 0x33
#define UB0_EXT_SLV_SENS_DATA_00  0x3B

// User bank 2
//用二进制会报错
#define UB2_GYRO_SMPLRT_DIV  0x00

#define UB2_GYRO_CONFIG_1  0x01
#define UB2_GYRO_CONFIG_1_FS_SEL_250DPS  0x00
#define UB2_GYRO_CONFIG_1_FS_SEL_500DPS  0x02
#define UB2_GYRO_CONFIG_1_FS_SEL_1000DPS  0x04
#define UB2_GYRO_CONFIG_1_FS_SEL_2000DPS  0x06
#define UB2_GYRO_CONFIG_1_DLPFCFG_12106HZ  0x00
#define UB2_GYRO_CONFIG_1_DLPFCFG_197HZ  0x00 | 0x01
#define UB2_GYRO_CONFIG_1_DLPFCFG_152HZ  0x08 | 0x01
#define UB2_GYRO_CONFIG_1_DLPFCFG_120HZ  0x10 | 0x01
#define UB2_GYRO_CONFIG_1_DLPFCFG_51HZ   0x18 | 0x01
#define UB2_GYRO_CONFIG_1_DLPFCFG_24HZ   0x20 | 0x01
#define UB2_GYRO_CONFIG_1_DLPFCFG_12HZ   0x28 | 0x01
#define UB2_GYRO_CONFIG_1_DLPFCFG_6HZ    0x30 | 0x01
#define UB2_GYRO_CONFIG_1_DLPFCFG_361HZ  0x38 | 0x01

#define UB2_ACCEL_SMPLRT_DIV_1  0x10
#define UB2_ACCEL_SMPLRT_DIV_2  0x11

#define UB2_ACCEL_CONFIG  0x14
#define UB2_ACCEL_CONFIG_FS_SEL_2G  0x00
#define UB2_ACCEL_CONFIG_FS_SEL_4G  0x02
#define UB2_ACCEL_CONFIG_FS_SEL_8G  0x04
#define UB2_ACCEL_CONFIG_FS_SEL_16G  0x06
#define UB2_ACCEL_CONFIG_DLPFCFG_1209HZ  0x00
#define UB2_ACCEL_CONFIG_DLPFCFG_246HZ  0x00 | 0x01
#define UB2_ACCEL_CONFIG_DLPFCFG_111HZ  0x10 | 0x01
#define UB2_ACCEL_CONFIG_DLPFCFG_50HZ   0x18 | 0x01
#define UB2_ACCEL_CONFIG_DLPFCFG_24HZ   0x20 | 0x01
#define UB2_ACCEL_CONFIG_DLPFCFG_12HZ   0x28 | 0x01
#define UB2_ACCEL_CONFIG_DLPFCFG_6HZ    0x30 | 0x01
#define UB2_ACCEL_CONFIG_DLPFCFG_473HZ  0x38 | 0x01

// User bank 3
#define UB3_I2C_MST_CTRL 0x01
#define UB3_I2C_MST_CTRL_CLK_400KHZ 0x07 // Gives 345.6kHz and is recommended to achieve max 400kHz

#define UB3_I2C_SLV0_ADDR 0x03
#define UB3_I2C_SLV0_ADDR_READ_FLAG 0x80

#define UB3_I2C_SLV0_REG 0x04

#define UB3_I2C_SLV0_CTRL 0x05
#define UB3_I2C_SLV0_CTRL_EN 0x80

#define UB3_I2C_SLV0_DO 0x06

// Common to all user banks
#define REG_BANK_SEL 0x7F
#define REG_BANK_SEL_USER_BANK_0 0x00
#define REG_BANK_SEL_USER_BANK_1 0x10
#define REG_BANK_SEL_USER_BANK_2 0x20
#define REG_BANK_SEL_USER_BANK_3 0x30

// Magnetometer constants
#define MAG_AK09916_I2C_ADDR 0x0C
#define MAG_AK09916_WHO_AM_I 0x4809
#define MAG_DATA_LENGTH 8 // Bytes

// Magnetometer (AK09916) registers
#define MAG_WHO_AM_I 0x00

#define MAG_HXL 0x11

#define MAG_CNTL2 0x31
#define MAG_CNTL2_POWER_DOWN 0x00
#define MAG_CNTL2_MODE_10HZ 0x02
#define MAG_CNTL2_MODE_50HZ 0x06
#define MAG_CNTL2_MODE_100HZ 0x08

#define MAG_CNTL3 0x32

#define MAG_CNTL3_RESET 0x01







//char WriteLenRegister(unsigned char address,unsigned char length,unsigned char *Tx_Data)
signed char GetTmp(float *tmp);
signed char GetMag(float *magX,float *magY,float *magZ);
signed char GetAccel(float *accelX,float *accelY,float *accelZ);
signed char GetGyro(float *gyroX,float *gyroY,float *gyroZ);
signed char ConfigMag(void);
signed char WhoAmIMag(void);
signed char SetAccelSrd(unsigned short int srd);
signed char SetGyroSrd(unsigned char srd);
signed char ConfigGyro(enum GyroRange range,enum GyroDlpfBandwidth bandwidth);
signed char ConfigAccel(enum AccelRange range,enum AccelDlpfBandwidth bandwidth);
signed char EnableAccelGyro(void);
signed char WhoAmI(void);
signed char ResetMag(void);
signed char Reset(void);
signed char PowerDownMag(void);
signed char EnableI2cMaster(void);
signed char SelectAutoClockSource(void) ;
signed char ChangeUserBank(enum USERBANK userBank);
signed char WriteMagRegister(unsigned char subAddress,unsigned char data) ;
signed char ReadMagRegisters(unsigned char subAddress,unsigned char count, unsigned char * dest) ;
signed char WriteByteRegister(unsigned char address ,unsigned char Tx_Data);
signed char ReadRegisters(unsigned char address,unsigned char count,unsigned char *Rx_Data);
signed char ReadMRegisters(unsigned char address,unsigned char count,unsigned char *Rx_Data);

#endif





