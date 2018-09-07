#ifndef	_MLX90615CONF_H_
#define	_MLX90615CONF_H_

#define	_MLX90615_USE_FREERTOS				0
#define	_MLX90615_I2C									hi2c1
#define	_MLX90615_I2C_ADDRESS					0xB6
#define	_MLX90615_SCL_GPIO						GPIOA	
#define	_MLX90615_SCL_PIN							GPIO_PIN_9		
#define	_MLX90615_SCL_ALTERNATE_PIN		GPIO_AF4_I2C1		// use for exit sleep mode

#endif
