
#include "mlx9061x.h"
#include "mlx9061xConfig.h"
#include <string.h>
#if _MLX9061X_FREERTOS == 0
#define mlx9061x_delay(x)   HAL_Delay(x)
#else
#include "cmsis_os.h"
#define mlx9061x_delay(x)   osDelay(x)
#endif
//###################################################################################################
#if _MLX9061X_90614 == 1 
// RAM
#define MLX9061X_RAWIR1     0x04
#define MLX9061X_RAWIR2     0x05
#define MLX9061X_TA         0x06
#define MLX9061X_TOBJ1      0x07
#define MLX9061X_TOBJ2      0x08
// EEPROM
#define MLX9061X_TOMAX      0x20
#define MLX9061X_TOMIN      0x21
#define MLX9061X_PWMCTRL    0x22
#define MLX9061X_TARANGE    0x23
#define MLX9061X_EMISS      0x24
#define MLX9061X_CONFIG     0x25
#define MLX9061X_ADDR       0x0E
#define MLX9061X_ID1        0x3C
#define MLX9061X_ID2        0x3D
#define MLX9061X_ID3        0x3E
#define MLX9061X_ID4        0x3F
#endif
#if _MLX9061X_90615 == 1 
// RAM
#define MLX9061X_RAWIR1     0x05
#define MLX9061X_TA         0x06
#define MLX9061X_TOBJ1      0x07
// EEPROM
#define MLX9061X_PWTMIN     0x10
#define MLX9061X_PWTRANG    0x11
#define MLX9061X_CONFIG     0x12
#define MLX9061X_EMISS      0x13
#define MLX9061X_ID1        0x1E
#define MLX9061X_ID2        0x1F
#endif
//###################################################################################################
uint8_t mlx9061x_crc8(uint8_t *data, uint8_t len)
{
  uint8_t crc = 0;
  while (len--)
  {
    uint8_t inbyte = *data++;
    for (uint8_t i = 8; i; i--)
    {
      uint8_t carry = (crc ^ inbyte) & 0x80;
      crc <<= 1;
      if (carry)
        crc ^= 0x7;
      inbyte <<= 1;
    }
  }
  return crc;
}
//###################################################################################################
bool mlx9061x_read16(uint8_t address, uint16_t *data)
{
  uint8_t d[3];
  if(HAL_I2C_Mem_Read(&_MLX9061X_I2C, _MLX9061X_I2C_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, d, 3, 100) != HAL_OK)
    return false;
  *data = d[0] | (d[1] << 8);
  return true;  
}
//###################################################################################################
bool mlx9061x_write16(uint8_t address, uint16_t data)
{
  uint8_t d[5];
  d[0] = _MLX9061X_I2C_ADDRESS;
  d[1] = address;
  #if _MLX9061X_90614 == 1 
  if(address < 0x20)  //  write in ram
  #endif
  #if _MLX9061X_90615 == 1 
  if(address < 0x10)  //  write in ram
  #endif
  {
    d[2] = data & 0x00FF;
    d[3] = data >> 8;
    d[4] = mlx9061x_crc8(d, 4);
    if(HAL_I2C_Mem_Write(&_MLX9061X_I2C, _MLX9061X_I2C_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, &d[2], 3, 100) != HAL_OK)
      return false;
    return true;  
  }  
  else  //  write in eeprom
  {
    d[2] = 0;
    d[3] = 0;
    d[4] = mlx9061x_crc8(d, 4);
    if(HAL_I2C_Mem_Write(&_MLX9061X_I2C, _MLX9061X_I2C_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, &d[2], 3, 100) != HAL_OK)
      return false;
    mlx9061x_delay(10);
    d[2] = data & 0x00FF;
    d[3] = data >> 8;
    d[4] = mlx9061x_crc8(d, 4);    
    mlx9061x_delay(10);
    if(HAL_I2C_Mem_Write(&_MLX9061X_I2C, _MLX9061X_I2C_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, &d[2], 3, 100) != HAL_OK)
      return false;
    mlx9061x_delay(10);
    uint16_t comp = 0;
    if(mlx9061x_read16(address, &comp) == false)
      return false;
    if(comp != data)
      return false;
    return true;
  }
}
//###################################################################################################
bool mlx9061x_init(void)
{
  if(HAL_GetTick() < 50)
    mlx9061x_delay(1);
  if(HAL_I2C_IsDeviceReady(&_MLX9061X_I2C, _MLX9061X_I2C_ADDRESS, 2, 100) != HAL_OK)
    return false;  
  return true;
}
//###################################################################################################
bool mlx9061x_readTargetC(float *temp)
{
  if(temp == NULL)
    return false;
  uint16_t  tmp;
  if(mlx9061x_read16(MLX9061X_TOBJ1, &tmp) == false)
    return false;
  *temp = (float)tmp * 0.02f;
  *temp -= 273.15f;
  return true;
}
//###################################################################################################
bool mlx9061x_readTargetF(float *temp)
{
  if(temp == NULL)
    return false;
  if(mlx9061x_readTargetC(temp) == false)
    return false;
  *temp =  (*temp * 9.0f / 5.0f) + 32.0f;
  return true;
}
//###################################################################################################
bool mlx9061x_readAmbientC(float *temp)
{
  if(temp == NULL)
    return false;
  uint16_t  tmp;
  if(mlx9061x_read16(MLX9061X_TA, &tmp) == false)
    return false;
  *temp = (float)tmp * 0.02f;
  *temp -= 273.15f;
  return true;
}
//###################################################################################################
bool mlx9061x_readAmbientF(float *temp)
{
  if(temp == NULL)
    return false;
  if(mlx9061x_readAmbientC(temp) == false)
    return false;
  *temp =  (*temp * 9.0f / 5.0f) + 32.0f;
  return true;
}
//###################################################################################################
bool mlx9061x_readEmissivity(float *emissivity)
{
  if(emissivity == NULL)
    return false;
  uint16_t tmp;
  if(mlx9061x_read16(MLX9061X_EMISS, &tmp) == false)
    return false;
  #if _MLX9061X_90614 == 1 
  *emissivity = (float)tmp / 65535.0f;
  #endif
  #if _MLX9061X_90615 == 1 
  *emissivity = (float)tmp / (16384.0f;
  #endif
  return true;
}
//###################################################################################################
bool mlx9061x_writeEmissivity(float emissivity)
{
  uint16_t tmp;
  if(emissivity > 1.0f)
    emissivity = 1.0f;
  #if _MLX9061X_90614 == 1 
  tmp = (uint16_t)(0xFFFF * emissivity);
  #endif
  #if _MLX9061X_90615 == 1 
  tmp = (uint16_t)(0x4000 * emissivity);
  #endif
  if(mlx9061x_write16(MLX9061X_EMISS, tmp) == false)
    return false;
  return true;
}
