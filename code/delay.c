#include "delay.h"

//Õý³£
void delay_ms(unsigned int t)
{
	 int i;
	 int a;
	 for( i=0;i<t;i++)
	 {
		  a=42000; 

 		 while(a--);
	 }
}
//ÂÔ´ó
void delay_us(unsigned int t)
{
	 int i;
	 int a;
	 for( i=0;i<t;i++)
	 {
		  a=40;  //at 168MHz 40 is ok,the higher the number the more timing precise
		  while(a--);
	 }
}
