# MLX90615 library
<br />
I hope use it and enjoy.
<br />
I use Stm32f030 and Keil Compiler and Stm32CubeMX wizard.
 <br />
Please Do This ...
<br />
<br />
1) Enable i2c standard mode on CubeMx.
<br />
2) Select "General peripheral Initalizion as a pair of '.c/.h' file per peripheral" on project settings.
<br />
3) Config your Mlx9061xConf.h file.
<br />
4) call  mlx9061x_init() on your app.
<br />
5) Call mlx9061x_readTargetC(&temp).
