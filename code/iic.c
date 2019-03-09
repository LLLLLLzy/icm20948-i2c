#include "stm32f4xx.h"
#include "iic.h"
#include "delay.h"

//#define SCL GPIO_Pin_8
//#define SDA GPIO_Pin_9
//#define iokou GPIOB //记得改时钟
#define SCL GPIO_Pin_0
#define SDA GPIO_Pin_1
#define iokou GPIOA //记得改时钟


#define SCL_H GPIO_SetBits(iokou,SCL)
#define SCL_L GPIO_ResetBits(iokou,SCL)
#define SDA_H GPIO_SetBits(iokou,SDA)
#define SDA_L GPIO_ResetBits(iokou,SDA)

#define Read_SDA GPIO_ReadInputDataBit(iokou,SDA)

void IIC_Init(void)
{
	GPIO_InitTypeDef gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	
	gpio.GPIO_Pin = SDA | SCL;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	
	GPIO_Init(iokou,&gpio);
	

}

void SDA_OUT(void)
{
	GPIO_InitTypeDef gpio;
	
	gpio.GPIO_Pin = SDA;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(iokou,&gpio);
	
}


void SDA_IN(void)
{
	GPIO_InitTypeDef gpio;
	
	gpio.GPIO_Pin = SDA;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(iokou,&gpio);
	
}


void IIC_Start(void)
{
	SDA_OUT();
	SDA_H;
	SCL_H;
	delay_us(1);
	SDA_L;
	delay_us(2);
	SCL_L; 
	
}

void IIC_Stop(void)
{
	SDA_OUT();
	SCL_L;
	SDA_L;
	delay_us(1);
	SCL_H;
	delay_us(1);
	SDA_H;
	delay_us(1);
	
	
}

//成功为0
//失败为1
unsigned char IIC_Wait_Ack(void)
{
	unsigned short int errtime=0;
	SDA_IN();
	SDA_H;
	delay_us(1);
	SCL_H;
	delay_us(1);
	while(Read_SDA)
	{
		errtime++;
		if(errtime>1000)
		{
			IIC_Stop();
			return 1;
		}
		
	}
	SCL_L;
	return 0;
	
}

void IIC_Ack(void)
{
	SCL_L;
	SDA_OUT();
	SDA_L;
	delay_us(1);
	SCL_H;
	delay_us(2);
	SCL_L;
}

void IIC_NAck(void)
{
	SCL_L;
	SDA_OUT();
	SDA_H;
	delay_us(1);
	SCL_H;
	delay_us(2);
	SCL_L;
	
}

void IIC_Send_Byte(u8 TxMag)
{
	u8 t;
	SDA_OUT();
	SCL_L;
	for(t=0;t<8;t++)
	{
		if(TxMag&0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}
		TxMag<<=1;
		delay_us(1);
		SCL_H;
		delay_us(1);
		SCL_L;
		delay_us(1);
	}
}

unsigned char IIC_Read_Byte(void)
{
	unsigned char i,receive = 0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		SCL_L;
		delay_us(1);
		SCL_H;
		receive<<=1;
		if(Read_SDA)receive++;
		
		delay_us(1);
	}
	
	return receive;
}
