#include "icm20948getdata.h"
#include "iic.h"
#include "delay.h"


///////以下内容放入icm20948getdata.h中会有重复定义的报错...为啥
//scale factors
float _accelScale;
float _gyroScale;
/////// track success of interacting with sensor
int _status;
//////// configuration
enum AccelRange _accelRange;
enum GyroRange _gyroRange;
enum AccelDlpfBandwidth _accelBandwidth;
enum GyroDlpfBandwidth _gyroBandwidth;
enum USERBANK _currentUserBank = USER_BANK_0;

unsigned char _gyroSrd;
unsigned short int _accelSrd;

signed char GetMag(float *magX,float *magY,float *magZ)
{
	unsigned short int x,y,z;
	unsigned char buffer[6];
	if(ChangeUserBank(USER_BANK_0) < 0)
	{
		return -1;
	}
	if(ReadRegisters(0x3b,6,buffer) < 0)
	{
		return -2;
	}
	x = (((signed short int)buffer[1]) << 8) | buffer[0];  
	y = (((signed short int)buffer[3]) << 8) | buffer[2];
	z = (((signed short int)buffer[5]) << 8) | buffer[4];
	*magX = (float)x*_magScale;
	*magY = (float)y*_magScale;
	*magZ = (float)z*_magScale;

	return 1;
}
signed char GetTmp(float *tmp)
{
	unsigned char buffer[2];
	unsigned short int tmper;
	if(ChangeUserBank(USER_BANK_0) < 0)
	{
		return -1;
	}
	if(ReadRegisters(0x3b,2,buffer) < 0)
	{
		return -2;
	}
	tmper = (signed short int)(buffer[1] << 8) | buffer[0];
	*tmp = (((float)tmper - _tempOffset)/_tempScale) + _tempOffset;
	return 1;
}
signed char GetAccel(float *accelX,float *accelY,float *accelZ)
{
	unsigned char buffer[6];
	signed short int axcounts,aycounts,azcounts;
	if (ChangeUserBank(USER_BANK_0) < 0)
	{
  	return -1;
	}
	if(ReadRegisters(UB0_ACCEL_XOUT_H,6,buffer) < 0)
	{
		return -2;
	}
	axcounts = (((signed short int)buffer[0]) << 8) | buffer[1];  
	aycounts = (((signed short int)buffer[2]) << 8) | buffer[3];
	azcounts = (((signed short int)buffer[4]) << 8) | buffer[5];
	*accelX = (float)axcounts * _accelScale;
	*accelY = (float)aycounts * _accelScale;
	*accelZ = (float)azcounts * _accelScale;
	
	return 1;
}


signed char GetGyro(float *gyroX,float *gyroY,float *gyroZ)
{
	unsigned char buffer[6];
	signed short int gxcounts,gycounts,gzcounts;
	if (ChangeUserBank(USER_BANK_0) < 0)
	{
  	return -1;
	}
	if(ReadRegisters(UB0_GYRO_XOUT_H,6,buffer) < 0)
	{
		return -2;
	}
	gxcounts = (((signed short int)buffer[0]) << 8) | buffer[1];  
	gycounts = (((signed short int)buffer[2]) << 8) | buffer[3];
	gzcounts = (((signed short int)buffer[4]) << 8) | buffer[5];
	*gyroX = (float)gxcounts * _gyroScale * Pi/180.0f;
	*gyroY = (float)gycounts * _gyroScale * Pi/180.0f;
	*gyroZ = (float)gzcounts * _gyroScale * Pi/180.0f;
	
	return 1;
}



signed char ConfigMag(void)// TODO: Add possibility to use other modes
{ 
	
	if(WriteMagRegister(MAG_CNTL2, MAG_CNTL2_MODE_100HZ) < 0)
		
	{
		return -1;
	}
	return 1;
}




signed char WhoAmIMag(void)
{
	unsigned char whoAmIBuffer[2];
	if (ReadMagRegisters(MAG_WHO_AM_I, 2, whoAmIBuffer) < 0)
	{
		return -1;
	}
	if(((unsigned short int)(whoAmIBuffer[0] << 8) | whoAmIBuffer[1]) != MAG_AK09916_WHO_AM_I)
	{
		return -2;
	}
	return 1;
}

signed char SetAccelSrd(unsigned short int srd)
{  
	unsigned char srdHigh = srd >> 8 & 0x0F; // Only last 4 bits can be set
	unsigned char srdLow = srd & 0x0F; // Only last 4 bits can be set
	if (ChangeUserBank(USER_BANK_2) < 0)
	{
		return -1;
	}
	
	if (WriteByteRegister(UB2_ACCEL_SMPLRT_DIV_1, srdHigh) < 0)
	{
		return -1;
	}
	
	if (WriteByteRegister(UB2_ACCEL_SMPLRT_DIV_2, srdLow) < 0)
	{
		return -1;
	}
	_accelSrd = srd;
	return 1;
}


signed char SetGyroSrd(unsigned char srd)
{
	if (ChangeUserBank(USER_BANK_2) < 0 || WriteByteRegister(UB2_GYRO_SMPLRT_DIV, srd) < 0)
	{
		return -1;
	}
	_gyroSrd = srd;
	return 1;
}


/* sets the gyro full scale range to values other than default */
signed char ConfigGyro(enum GyroRange range,enum GyroDlpfBandwidth bandwidth)
{
	
	unsigned char gyroConfigRegValue = 0x00;
	unsigned char dlpfRegValue = 0x00;
	float gyroScale = 0x00;
  if (ChangeUserBank(USER_BANK_2) < 0)
	{
  	return -1;
  }
  
  
  switch(range) 
	{
    case GYRO_RANGE_250DPS:
		{
    	gyroConfigRegValue = UB2_GYRO_CONFIG_1_FS_SEL_250DPS;
      gyroScale = 250.0f/gyroRawScaling ; // setting the gyro scale to 250DPS
      break;
    }
    case GYRO_RANGE_500DPS:
		{
      gyroConfigRegValue = UB2_GYRO_CONFIG_1_FS_SEL_500DPS;
      gyroScale = 500.0f/gyroRawScaling; // setting the gyro scale to 500DPS
      break;  
    }
    case GYRO_RANGE_1000DPS:
		{
      gyroConfigRegValue = UB2_GYRO_CONFIG_1_FS_SEL_1000DPS;
      gyroScale = 1000.0f/gyroRawScaling; // setting the gyro scale to 1000DPS
      break;
    }
    case GYRO_RANGE_2000DPS:
		{
      gyroConfigRegValue = UB2_GYRO_CONFIG_1_FS_SEL_2000DPS;
      gyroScale = 2000.0f/gyroRawScaling; // setting the gyro scale to 2000DPS
      break;
    }
  }
  switch(bandwidth)
	{
  	case GYRO_DLPF_BANDWIDTH_12106HZ: dlpfRegValue = UB2_GYRO_CONFIG_1_DLPFCFG_12106HZ; break;
  	case GYRO_DLPF_BANDWIDTH_197HZ: dlpfRegValue = UB2_GYRO_CONFIG_1_DLPFCFG_197HZ; break;
  	case GYRO_DLPF_BANDWIDTH_152HZ: dlpfRegValue = UB2_GYRO_CONFIG_1_DLPFCFG_152HZ; break;
  	case GYRO_DLPF_BANDWIDTH_120HZ: dlpfRegValue = UB2_GYRO_CONFIG_1_DLPFCFG_120HZ; break;
  	case GYRO_DLPF_BANDWIDTH_51HZ: dlpfRegValue = UB2_GYRO_CONFIG_1_DLPFCFG_51HZ; break;
  	case GYRO_DLPF_BANDWIDTH_24HZ: dlpfRegValue = UB2_GYRO_CONFIG_1_DLPFCFG_24HZ; break;
  	case GYRO_DLPF_BANDWIDTH_12HZ: dlpfRegValue = UB2_GYRO_CONFIG_1_DLPFCFG_12HZ; break;
  	case GYRO_DLPF_BANDWIDTH_6HZ: dlpfRegValue = UB2_GYRO_CONFIG_1_DLPFCFG_6HZ; break;
  	case GYRO_DLPF_BANDWIDTH_361HZ: dlpfRegValue = UB2_GYRO_CONFIG_1_DLPFCFG_361HZ; break;
  }
  if (WriteByteRegister(UB2_GYRO_CONFIG_1, gyroConfigRegValue | dlpfRegValue) < 0)
	{
  	return -1;
	}
	_gyroScale = gyroScale;
  _gyroRange = range;
  _gyroBandwidth = bandwidth;
  return 1;
}


signed char ConfigAccel(enum AccelRange range,enum AccelDlpfBandwidth bandwidth)
{
	unsigned char accelRangeRegValue = 0x00;
  float accelScale = 0.0f;
	unsigned char dlpfRegValue = 0x00;	
	
	if (ChangeUserBank(USER_BANK_2) < 0) {
  	return -1;
  }
  
  switch(range)
	{
    case ACCEL_RANGE_2G:
		{
      accelRangeRegValue = UB2_ACCEL_CONFIG_FS_SEL_2G;
      accelScale = G * 2.0f/accRawScaling; // setting the accel scale to 2G
      break; 
    }
    case ACCEL_RANGE_4G:
		{
      accelRangeRegValue = UB2_ACCEL_CONFIG_FS_SEL_4G;
      accelScale = G * 4.0f/accRawScaling; // setting the accel scale to 4G
      break;
    }
    case ACCEL_RANGE_8G: 
		{
      accelRangeRegValue = UB2_ACCEL_CONFIG_FS_SEL_8G;
      accelScale = G*8.0f/accRawScaling; // setting the accel scale to 8G
      break;
    }
    case ACCEL_RANGE_16G:
		{
      accelRangeRegValue = UB2_ACCEL_CONFIG_FS_SEL_16G;
      accelScale = G*16.0f/accRawScaling; // setting the accel scale to 16G
			
      break;
    }
  }

  switch(bandwidth)
	{
  	case ACCEL_DLPF_BANDWIDTH_1209HZ: dlpfRegValue = UB2_ACCEL_CONFIG_DLPFCFG_1209HZ; break;
  	case ACCEL_DLPF_BANDWIDTH_246HZ: dlpfRegValue = UB2_ACCEL_CONFIG_DLPFCFG_246HZ; break;
  	case ACCEL_DLPF_BANDWIDTH_111HZ: dlpfRegValue = UB2_ACCEL_CONFIG_DLPFCFG_111HZ; break;
  	case ACCEL_DLPF_BANDWIDTH_50HZ: dlpfRegValue = UB2_ACCEL_CONFIG_DLPFCFG_50HZ; break;
  	case ACCEL_DLPF_BANDWIDTH_24HZ: dlpfRegValue = UB2_ACCEL_CONFIG_DLPFCFG_24HZ; break;
  	case ACCEL_DLPF_BANDWIDTH_12HZ: dlpfRegValue = UB2_ACCEL_CONFIG_DLPFCFG_12HZ; break;
  	case ACCEL_DLPF_BANDWIDTH_6HZ: dlpfRegValue = UB2_ACCEL_CONFIG_DLPFCFG_6HZ; break;
  	case ACCEL_DLPF_BANDWIDTH_473HZ: dlpfRegValue = UB2_ACCEL_CONFIG_DLPFCFG_473HZ; break;
  }
  if (WriteByteRegister(UB2_ACCEL_CONFIG, accelRangeRegValue | dlpfRegValue) < 0)
	{
		return -1;
  }
  _accelScale = accelScale;
  _accelRange = range;
  _accelBandwidth = bandwidth;
  return 1;
}



signed char EnableAccelGyro(void)
{
	if (ChangeUserBank(USER_BANK_0) < 0 || WriteByteRegister(UB0_PWR_MGMNT_2, UB0_PWR_MGMNT_2_SEN_ENABLE) < 0)
	{
    return -1;
  }
  return 1;
}


signed char WhoAmI(void)
{
	unsigned char whoAmIBuffer;
	if (ChangeUserBank(USER_BANK_0) < 0)
	{
		return -1;
	}
	// read the WHO AM I register
	if (ReadRegisters(UB0_WHO_AM_I, 1, &whoAmIBuffer) < 0)
	{
		return -2;
	}
	if(whoAmIBuffer != ICM20948_WHO_AM_I)
	{
		return -3;
	}
	return 1;
}


signed char ResetMag(void)
{
	if (WriteMagRegister(MAG_CNTL3, MAG_CNTL3_RESET) < 0)
	{
		return -1;
	}
	return 1;
}

signed char Reset(void)
{
	if (ChangeUserBank(USER_BANK_0) < 0)
	{
		return -1;
	}
	if (WriteByteRegister(UB0_PWR_MGMNT_1, UB0_PWR_MGMNT_1_DEV_RESET) < 0)
	{
		return -2;
	}
	return 1;
}

signed char PowerDownMag(void)
{

	if (WriteMagRegister(MAG_CNTL2,MAG_CNTL2_POWER_DOWN )< 0)
	{
		return -1;
	}
	return 1;
}

signed char EnableI2cMaster(void)
{
	if (ChangeUserBank(USER_BANK_0) < 0)
	{
		return -1;
	}
	if (WriteByteRegister(UB0_USER_CTRL, UB0_USER_CTRL_I2C_MST_EN) < 0)
	{
		return -2;
	}
	if (ChangeUserBank(USER_BANK_3) < 0)
	{
		return -3;
	}
	if(WriteByteRegister(UB3_I2C_MST_CTRL, UB3_I2C_MST_CTRL_CLK_400KHZ) < 0)
	{
		return -4;
	}
	return 1;
}


signed char SelectAutoClockSource(void)
{
	if (ChangeUserBank(USER_BANK_0) < 0 || WriteByteRegister(UB0_PWR_MGMNT_1, UB0_PWR_MGMNT_1_CLOCK_SEL_AUTO) < 0)
	{
		return -1;
    }
    return 1;
}
    

signed char ChangeUserBank(enum USERBANK userBank)
{
	unsigned char userBankRegValue = 0x00;
	if(userBank == _currentUserBank)
	{
		return 2;
	}
	switch(userBank) 
	{
    case USER_BANK_0: 
		{
			userBankRegValue = REG_BANK_SEL_USER_BANK_0;
			break;
		}
    case USER_BANK_1: 
		{
			userBankRegValue = REG_BANK_SEL_USER_BANK_1;
			break;
		}
    case USER_BANK_2: 
		{
			userBankRegValue = REG_BANK_SEL_USER_BANK_2;
			break;
		}
    case USER_BANK_3: 
		{
			userBankRegValue = REG_BANK_SEL_USER_BANK_3;
			break;
		}
	}
	if (WriteByteRegister(REG_BANK_SEL, userBankRegValue) < 0) 
	{
		return -1;
	}
	_currentUserBank = userBank;
	return 1;
}

////buffer可能有错
signed char WriteMagRegister(unsigned char subAddress,unsigned char data) 
{
	unsigned char magBuffer;
	if (ChangeUserBank(USER_BANK_3) < 0) 
	{
		return -1;
    }
	if (WriteByteRegister(UB3_I2C_SLV0_ADDR, MAG_AK09916_I2C_ADDR) < 0)
	{
		return -2;
    }
    // set the register to the desired magnetometer sub address 
	if (WriteByteRegister(UB3_I2C_SLV0_REG, subAddress) < 0)
	{
		return -3;
    }
    // store the data for write
	if (WriteByteRegister(UB3_I2C_SLV0_DO, data) < 0)
	{
		return -4;
    }
    // enable I2C and send 1 byte
	if (WriteByteRegister(UB3_I2C_SLV0_CTRL, UB3_I2C_SLV0_CTRL_EN | (unsigned char)1) < 0)
	{
		return -5;
    }
	// read the register and confirm
	if (ReadMagRegisters(subAddress, 1, &magBuffer) < 0)
	{
		return -6;
    }
    if(magBuffer != data)
	{
		return -7;
    }
    return 1;
}

signed char ReadMagRegisters(unsigned char subAddress,unsigned char count, unsigned char * dest) 
{
	if (ChangeUserBank(USER_BANK_3) < 0)
	{
		return -1;
    }
	if (WriteByteRegister(UB3_I2C_SLV0_ADDR, MAG_AK09916_I2C_ADDR | UB3_I2C_SLV0_ADDR_READ_FLAG) < 0)
	{
		return -2;
    }
    // set the register to the desired magnetometer sub address
	if (WriteByteRegister(UB3_I2C_SLV0_REG, subAddress) < 0)
	{
		return -3;
    }
	// enable I2C and request the bytes
	if (WriteByteRegister(UB3_I2C_SLV0_CTRL, UB3_I2C_SLV0_CTRL_EN | count) < 0)
	{
		return -4;
	}
	
	delay_ms(10); 
	// takes some time for these registers to fill
	// read the bytes off the ICM-20948 EXT_SLV_SENS_DATA registers
	if (ChangeUserBank(USER_BANK_0) < 0)
	{
		return -5;
	}
	_status = ReadRegisters(UB0_EXT_SLV_SENS_DATA_00, count, dest); 
	return _status;
}





////write a byte data
////return	: -1 -- 错误
////		      -2 -- 错误
////		      -3 -- 错误
////		       0 -- 正常
signed char WriteByteRegister(unsigned char address ,unsigned char Tx_Data)
{
	unsigned char iicCheckData;
	IIC_Start();
	IIC_Send_Byte(ICM20948ADDRESS<<1|0);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return -1;
	}
	IIC_Send_Byte(address);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return -2;
	}
	IIC_Send_Byte(Tx_Data);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return -3;
	}
	IIC_Stop();
	
	ReadRegisters(address,1,&iicCheckData);
	if(iicCheckData == Tx_Data)
	{
		return 1;
	}
	else
	{
		return -4;
	}

}

//read data
//return	: -1 -- 错误
//		      -2 -- 错误
//		      -3 -- 错误
//		       0 -- 正常
signed char ReadRegisters(unsigned char address,unsigned char count,unsigned char *Rx_Data)
{
	
	IIC_Start();
	IIC_Send_Byte(ICM20948ADDRESS<<1|0);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return -1;
	}
	IIC_Send_Byte(address);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return -2;
	}
	
	IIC_Start();
	IIC_Send_Byte(ICM20948ADDRESS<<1|1);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return -3;
	}
	while(count)
	{
		if(count == 1)
		{
			*Rx_Data=IIC_Read_Byte();
		}
		else 
		{
			*Rx_Data=IIC_Read_Byte();
			IIC_Ack();
		}
		count--;
		Rx_Data++;
	}
	IIC_NAck();
	IIC_Stop();
	return 1;
}

