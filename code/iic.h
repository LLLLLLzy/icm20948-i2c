#ifndef __IIC_INIT__
#define __IIC_INIT__


void IIC_Init(void);
void SDA_OUT(void);
void SDA_IN(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(unsigned char TxMag);
unsigned char IIC_Read_Byte(void);

#endif




