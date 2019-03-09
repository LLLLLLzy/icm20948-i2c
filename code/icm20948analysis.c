#include "icm20948getdata.h"
#include "icm20948analysis.h"
#include "delay.h"

#include <math.h>
signed char qqq,ccc;

//�˺������Ի���ͨ������ż�����ǻ᷵��-4�����и��𷵻�-2������������� 3.6 13:20
//��ִ�����鷢�ֻ��Ƿ���-15
//Ӧ�����ϵ��᷵��-2
signed char Icm20948Init(void)
{
//	unsigned char buf;
	if (ChangeUserBank(USER_BANK_0) < 0)// Make sure that the user bank selection is in sync
	{ 
		return -1;
	}
	if (SelectAutoClockSource() < 0)// TODO: Why set clock source here? It is resetted anyway...//����
	{ 
		return -2;
	}
	// enable I2C master mode
	if(EnableI2cMaster() < 0)//����
	{
		return -3;
	}
		

	delay_ms(1);
	if (PowerDownMag() < 0)//����
	{
		return -4;
	}
	Reset(); // reset the ICM20948. Don't check return value as a reset clears the register and can't be verified.
	delay_ms(10); // wait for ICM-20948 to come back up
	ResetMag(); // Don't check return value as a reset clears the register and can't be verified.
	
	if (SelectAutoClockSource() < 0)
	{
		return -7;
	}
	
	if (WhoAmI() < 0)
	{
		return -8;
	}
	if (EnableAccelGyro() < 0)
	{
		return -9;
	}
	if (ConfigAccel(ACCEL_RANGE_16G, ACCEL_DLPF_BANDWIDTH_246HZ) < 0)
	{
		return -10;
	}
	if (ConfigGyro(GYRO_RANGE_2000DPS, GYRO_DLPF_BANDWIDTH_197HZ) < 0)
	{
		return -11;
	}
	if (SetGyroSrd(0) < 0) 
	{		
		return -12;
	} 
	if (SetAccelSrd(0) < 0)
	{ 
		return -13;
	}
	if(EnableI2cMaster() < 0)
	{
		return -14;
	}
	delay_ms(10);
	if(WhoAmIMag()< 0 )
	{
		return -15;
	}
	delay_ms(10);

	if(ConfigMag()< 0)
	{
		return -16;
	}
	delay_ms(1);
	if(SelectAutoClockSource() < 0)// TODO: Why do this again here?
	{ 
		return -17;
	}       
	return 1;
}

//У׼���ٶ� 
void Angular_Velocity_Calibration(float *x,float *y,float *z)//У׼�Ĳ���׼�����Ľ�
{
	
	float gyroX,gyroY,gyroZ;
	
	GetGyro(&gyroX,&gyroY,&gyroZ);
	
	//У׼
	*x -= gyroX;
	*y -= gyroY;
	*z -= gyroZ;

	
}
//���������ٶȵļн�,���ؽǶ�ֵ
void MPU_Angle_Analysis(float *rX,float *rY,float *rZ)
{
	
	float accelX,accelY,accelZ;//������
	float accelXSum,accelYSum,accelZSum;//������
	int i;
	float norm;//ģ��

	//�˲�
	for(i=0;i<10;i++)
	{
		GetAccel(&accelX,&accelY,&accelZ);
		
		//��ֹԽ��
		if(accelX>1)       accelX = 1;
		else if(accelX<-1) accelX = -1;
		
		if(accelY>1)       accelY = 1;
		else if(accelY<-1) accelY = -1;
		
		if(accelZ>1)       accelZ = 1;
		else if(accelZ<-1) accelZ = -1;

		accelXSum += accelX;
		accelYSum += accelY;
		accelZSum += accelZ;
		
	}
	accelX = accelXSum / 10.0f;
	accelY = accelYSum / 10.0f;
	accelZ = accelZSum / 10.0f;
	
	norm = sqrt(accelX*accelX+accelY*accelY+accelZ*accelZ);
	norm>1?(norm=1):(norm=norm);
	
	accelX = acos(accelX/norm);
	accelY = acos(accelY/norm);
	accelZ = acos(accelZ/norm);
	
	accelX = accelX*180.0f/Pi;//����ת�Ƕ�
	accelY = accelY*180.0f/Pi;
	accelZ = accelZ*180.0f/Pi;
	
	
	*rX = accelX;
	*rY = accelY;
	*rZ = accelZ;
	
	accelX = 0;
	accelY = 0;
	accelZ = 0;
}







