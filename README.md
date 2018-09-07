# MLX90615 library
<br />
I hope use it and enjoy.
<br />
I use Stm32f030 and Keil Compiler and Stm32CubeMX wizard.
 <br />
Please Do This ...
<br />
<br />
1) Enable i2c on CubeMx.
<br />
2) Select "General peripheral Initalizion as a pair of '.c/.h' file per peripheral" on project settings.
<br />
3) Config your Mlx90615Conf.h file.
<br />
4) call  Mlx90615_Init() on your app.
<br />
5) Call Mlx90615_ReadTempTarget(&temp) or Mlx90615_ReadTempAmbient(&temp).
