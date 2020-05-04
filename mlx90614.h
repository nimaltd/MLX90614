#ifndef _MLX90614_H
#define _MLX90614_H


/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw
  
  Version:    1.0.0
  
  
  Reversion History:
  
  (1.0.0)
  First release.
*/

#include "i2c.h"
#include <stdbool.h>


typedef enum
{
	MLX90614_UNIT_RAW,
	MLX90614_UNIT_K,
	MLX90614_UNIT_C,
	MLX90614_UNIT_F

} MLX90614_UNIT_t;

typedef struct
{
  uint8_t   IIR : 3;
  uint8_t   RepeatSensorTestON: 1;
  uint8_t   SelectObjAmb: 2;
  uint8_t   DualIRSensor: 1;
  uint8_t   NegativeSignOfKs: 1;
  uint8_t   FIR: 3;
  uint8_t   GAIN: 3;
  uint8_t   NegativeSignOfKt2: 1;
  uint8_t   DisableSensorTest: 1;
  
}MLX90614_CONFIG_REG_t;

typedef struct
{
  MLX90614_UNIT_t unit;
	int16_t rawEmissivity, rawAmbient, rawObject1, rawObject2, rawMax, rawMin; 
  float emissivity, ambient, object1, object2, max, min; 
	uint16_t id[4];
  MLX90614_CONFIG_REG_t configReg;
    
}MLX90614_t;

bool mlx90614_init(void);
void mlx90614_setUnit(MLX90614_UNIT_t MLX90614_UNIT_);
bool mlx90614_readID(int16_t *id);
bool mlx90614_getEmissivity(float *emissivity);
bool mlx90614_setEmissivity(float emissivity);
bool mlx90614_setMax(float maxTemp);
bool mlx90614_setMin(float minTemp);
bool mlx90614_getMax(float *maxTemp);
bool mlx90614_getMin(float *minTemp);
bool mlx90614_getAmbient(float *ambientTemp);
bool mlx90614_getObject1(float *objectTemp);
bool mlx90614_getObject2(float *objectTemp);


#endif
