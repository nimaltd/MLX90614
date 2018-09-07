
#include "MLX90615.h"
#include "MLX90615Conf.h"
#include "i2c.h"
#include <string.h>


#if (_MLX90615_USE_FREERTOS==1)
#include "cmsis_os"
#define	Mlx90615_Delay(x)		osDelay(x)
#else
#define	Mlx90615_Delay(x)		HAL_Delay(x)
#endif


Mlx90615_t	Mlx90615;
//##################################################################################################
uint8_t crc8Msb(uint8_t poly, uint8_t* data, int size)
{
	uint8_t crc = 0x00;
	int bit;
	while (size--)
	{
		crc ^= *data++;
		for (bit = 0; bit < 8; bit++)
		{
			if (crc & 0x80)
				crc = (crc << 1) ^ poly;
			else
				crc <<= 1;
		}
	}
	return crc;
}
//##################################################################################################
void	Mlx90615_ReadConfig(void)
{
	uint8_t	Data[2];
	HAL_I2C_Mem_Read(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,0x12,I2C_MEMADD_SIZE_8BIT,Data,2,100);
	memcpy(&Mlx90615,Data,2);
	HAL_I2C_Mem_Read(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,0x13,I2C_MEMADD_SIZE_8BIT,Data,2,100);
	Mlx90615.Emissivity = (Data[0]|(Data[1]<<8))/16384.0f;
}
//##################################################################################################
bool	Mlx90615_Init(void)
{
	while(HAL_GetTick()<600)
		Mlx90615_Delay(10);
	if(HAL_I2C_IsDeviceReady(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,1,20)!=HAL_OK)
		return false;
	Mlx90615_ReadConfig();	
	return true;
}
uint8_t crc;
//################################################################################################
bool	Mlx90615_ReadTempTarget(float *Temp)
{	
	uint8_t	Data[3];
	if(HAL_I2C_Mem_Read(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,0x27,I2C_MEMADD_SIZE_8BIT,Data,3,100)!=HAL_OK)
		return false;
	double tempFactor = 0.02; 
	double tempData = 0x0000; 
	tempData = (double)(((Data[1] & 0x007F) << 8) | Data[0]);
	tempData = (tempData * tempFactor);
	*Temp = (float)(tempData - 273.15);
	return true;
}
//##################################################################################################
bool	Mlx90615_ReadTempAmbient(float	*Temp)
{	
	uint8_t	Data[3];
	if(HAL_I2C_Mem_Read(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,0x26,I2C_MEMADD_SIZE_8BIT,Data,3,100)!=HAL_OK)
		return false;
	double tempFactor = 0.02; 
	double tempData = 0x0000; 
	tempData = (double)(((Data[1] & 0x007F) << 8) | Data[0]);
	tempData = (tempData * tempFactor);
	*Temp = (float)(tempData - 273.15);
	return true;	
}
//##################################################################################################
void	Mlx90615_EnterSleep(void)
{
	uint8_t Data[2]={0xC6,0x6D};
	HAL_I2C_Master_Transmit(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,Data,2,100);
}
//##################################################################################################
void	Mlx90615_ExitSleep(void)
{
	
	GPIO_InitTypeDef	gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_OD;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Pin = _MLX90615_SCL_PIN;
	
	HAL_GPIO_Init(_MLX90615_SCL_GPIO,&gpio);
		
	HAL_GPIO_WritePin(_MLX90615_SCL_GPIO,_MLX90615_SCL_PIN,GPIO_PIN_RESET);
	Mlx90615_Delay(50);
	HAL_GPIO_WritePin(_MLX90615_SCL_GPIO,_MLX90615_SCL_PIN,GPIO_PIN_SET);
	Mlx90615_Delay(300);	
	
	gpio.Mode = GPIO_MODE_AF_OD;
	gpio.Alternate = _MLX90615_SCL_ALTERNATE_PIN;
	HAL_GPIO_Init(_MLX90615_SCL_GPIO,&gpio);
}
//##################################################################################################
bool	_Mlx90615_SetEmissivity(uint16_t	Emissivity)
{
	uint8_t  buffer[6] = {0};
	uint8_t *write = buffer + 0;
	uint8_t *cmd = buffer + 1;
	uint8_t *data = buffer + 2;
	int dev = _MLX90615_I2C_ADDRESS;

	*write = dev;
	*cmd = 0x13;
	*(data++) = Emissivity & 0xff;
	*(data++) = (Emissivity >> 8) & 0xff;
	*data = crc8Msb(0x07, (uint8_t*)buffer, 4);

	if(crc8Msb(0x07,(uint8_t*)buffer, 5))
	 return false;
	if(HAL_I2C_Mem_Write(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,0x13,I2C_MEMADD_SIZE_8BIT,&buffer[2],3,100)!=HAL_OK)
		return false;
	Mlx90615_Delay(100);	
	buffer[0]=0;
	buffer[1]=0;
	HAL_I2C_Mem_Read(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,0x13,I2C_MEMADD_SIZE_8BIT,buffer,2,100);
	Mlx90615.Emissivity = (buffer[0]|(buffer[1]<<8))/16384.0f;
	return true;
	
}
//##################################################################################################
bool	Mlx90615_SetEmissivity(float	Emissivity)
{
	if(_Mlx90615_SetEmissivity(0)==true)
	{
		if(Emissivity>1.0)
			Emissivity = 1;
		if(Emissivity<0)
			Emissivity=0;
		return _Mlx90615_SetEmissivity(Emissivity*16384.0);		
	}
	else
		return false;
}
//##################################################################################################
bool	_Mlx90615_SetIIR(uint8_t	IIR_1_to_7)
{
	uint8_t  buffer[6] = {0};
	uint8_t *write = buffer + 0;
	uint8_t *cmd = buffer + 1;
	uint8_t *data = buffer + 2;
	int dev = _MLX90615_I2C_ADDRESS;
	uint8_t xdata[2];
	memcpy(xdata,&Mlx90615,2);
	
	xdata[1]&=0x8F;
	xdata[1]|=IIR_1_to_7<<5;	
	*write = dev;
	*cmd = 0x12;
	*(data++) = xdata[0];
	*(data++) = xdata[1];
	*data = crc8Msb(0x07, (uint8_t*)buffer, 4);

	if(crc8Msb(0x07,(uint8_t*)buffer, 5))
	 return false;
	if(HAL_I2C_Mem_Write(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,0x12,I2C_MEMADD_SIZE_8BIT,&buffer[2],3,100)!=HAL_OK)
		return false;
	Mlx90615_Delay(100);	
	buffer[0]=0;
	buffer[1]=0;
	HAL_I2C_Mem_Read(&_MLX90615_I2C,_MLX90615_I2C_ADDRESS,0x12,I2C_MEMADD_SIZE_8BIT,buffer,2,100);
	memcpy(&Mlx90615,buffer,2);
	return true;
	
}
//##################################################################################################

