#include "stm32f10x.h"
#include "I2C.h"
#include "USART.h"
#include "Delay.h"


#define	SMPLRT_DIV					0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG							0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		 			0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	 			0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	 			0x3B
#define	ACCEL_XOUT_L 				0x3C
#define	ACCEL_YOUT_H				0x3D
#define	ACCEL_YOUT_L				0x3E
#define	ACCEL_ZOUT_H				0x3F
#define	ACCEL_ZOUT_L	 			0x40
#define	TEMP_OUT_H					0x41
#define	TEMP_OUT_L					0x42
#define	GYRO_XOUT_H		 			0x43
#define	GYRO_XOUT_L		 			0x44	
#define	GYRO_YOUT_H		 			0x45
#define	GYRO_YOUT_L		 			0x46
#define	GYRO_ZOUT_H		 			0x47
#define	GYRO_ZOUT_L					0x48
#define	PWR_MGMT_1		 			0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			 			0x75	//IIC地址寄存器(默认数值0x68，只读)

#define MPU6050_ADDRESS   MPU6050_DEFAULT_ADDRESS
#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     0xD0   //MPU6050_ADDRESS_AD0_LOW  ((MPU6050_ADDRESS_AD0_LOW<<1)&0xFE) or  ((MPU6050_ADDRESS_AD0_HIGH<<1)&0xFE)

#define I2C_MST_CTRL        0x24  
#define I2C_SLV0_ADDR       0x25  //指定从机的IIC地址
#define I2C_SLV0_REG        0x26 	//指定从机的寄存器地址 
#define I2C_SLV0_CTRL       0x27
#define I2C_SLV0_DO         0x63   //该寄存器的内容会写入到从机设备中
#define USER_CTRL           0x6A    //用户使能FIFO缓存区    I2C主机模式和主要I2C接口
#define INT_PIN_CFG         0x37  

I2C iic(1,false);
USART usart1(1,115200,true);
int main()
{
	u8 temp[15];
		//向命令队列里添加命令
	u8 IIC_Write_Temp;
	Delay::Ms(2000);
	
	IIC_Write_Temp=0;
	iic.AddCommand(MPU6050_ADDRESS,PWR_MGMT_1,&IIC_Write_Temp,1,0,0);
	IIC_Write_Temp=2;
	iic.AddCommand(MPU6050_ADDRESS,INT_PIN_CFG,&IIC_Write_Temp,1,0,0);
	IIC_Write_Temp=7;
	iic.AddCommand(MPU6050_ADDRESS,SMPLRT_DIV,&IIC_Write_Temp,1,0,0);
	IIC_Write_Temp=7;
	iic.AddCommand(MPU6050_ADDRESS,USER_CTRL,&IIC_Write_Temp,1,0,0);
	IIC_Write_Temp=6;
	iic.AddCommand(MPU6050_ADDRESS,CONFIG,&IIC_Write_Temp,1,0,0);
	IIC_Write_Temp=0x18;
	iic.AddCommand(MPU6050_ADDRESS,GYRO_CONFIG,&IIC_Write_Temp,1,0,0);//+-2000 °/s
	IIC_Write_Temp=9;
	iic.AddCommand(MPU6050_ADDRESS,ACCEL_CONFIG,&IIC_Write_Temp,1,0,0);//+-4g   5Hz
	iic.StartCMDQueue();
	
	while(1)
	{
		iic.AddCommand(0xd0,0x75,0,0,temp,1);
		iic.AddCommand(MPU6050_ADDRESS,ACCEL_XOUT_H,0,0,&temp[1],14);//获取IMU加速度、角速度、IMU温度计数值
		if(!iic.StartCMDQueue())
		{
			usart1<<"...\n\n\n\n\n\n\n\n";
			iic.Init();
			temp[0]=0;
		}		
		Delay::Ms(10);
		usart1<<temp[0]<<"\t"<<temp[1]<<"\t"<<temp[2]<<"\t"<<temp[3]<<"\n";
	}
}

