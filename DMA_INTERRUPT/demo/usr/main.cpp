#include "stm32f10x.h"
#include "I2C.h"
#include "USART.h"
#include "Delay.h"
I2C iic;
USART usart1(1,115200);
int main()
{
	u8 temp=0;
	while(1)
	{
		iic.AddCommand(0xd0,0x75,0,0,&temp,1);
		if(!iic.StartCMDQueue(3))
		{
			usart1<<"...\n\n\n\n\n\n\n\n";
			iic.Init();
			temp=0;
		}		
		Delay::Ms(10);
		usart1<<temp<<"\n";
	}
}

