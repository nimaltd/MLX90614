#ifndef _MLX9061X_H
#define _MLX9061X_H

#include "i2c.h"
#include <stdbool.h>

/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw
  
  Version:    2.0.0
  
  
  Reversion History:
  
  (2.0.0)
  rerite and add mlx90614
  
  (1.0.0)
  First release.
*/


/*
			Material              Emissivity
Alumel (Unoxidized)         0.10 - 0.25
Alumel (Oxidized)           0.60
Aluminum (Polished)         0.10 - 0.05
Alumi	num (Oxidized)        0.10 - 0.40
Aluminum (Rough)            0.10 - 0.30
Aluminum (Anodized)         0.60 - 0.95
Aluminum Oxide              0.40
Asbestos                    0.95
Asphalt                     0.90 - 1.00
Basalt                      0.70
Bismuth                     0.50
Brass (Polished)            0.05
Brass (Oxidized)            0.50 - 0.60
Brass (Burnished)           0.30
Carbon (Unoxidized)         0.40 - 0.90
Carbon (Filament)           0.50
Carbon (Soot)               0.50 - 0.95
Carbon (Coke)               0.95 - 1.00
Carbon (Graphite)           0.70 - 0.80
Carborundum                 0.80 - 0.90
Ceramic                     0.90 - 0.95
Clay (Fired)                0.95
Concrete                    0.95
Chrome (Oxidized)           0.60 - 0.85
Chromium                    0.10
Cobalt                      0.20
Columbium (Polished)        0.20
Columbium (Oxidized)        0.70
Copper (Polished)           0.10
Copper (Oxidized)           0.20 - 0.80
Electrical terminal blocks  0.60
Enamel                      0.90
Foods                       0.85 - 1.00
Formica                     0.95
Glass (Convex D)            0.80
Glass (Nonex)               0.80
Glass (Plate)               0.90 - 0.95
Glass (Fused quartz)        0.75
Glass (Pyrex,lead and soda) 0.95	
Gold                        0.05
Granite (Polished)          0.85		
Granite (Rough)             0.90
Granite (Natural)           0.95
Gravel                      0.90 - 0.95
Gypsum                      0.85 - 0.95
Haynes Alloy                0.30 - 0.80
Human Skin                  0.99
Inconel (Polished)          0.15
Inconel (Oxidized)          0.70 - 0.95
Inconel (Sandblasted)       0.30 - 0.66
Iron (Oxidized)             0.50 - 0.95
Iron (Rusted)               0.50 - 0.70
Iron (Wrought, dull)        0.90
Iron Oxide                  0.85
Lacquer (Colored on Al)     0.75 - 0.90
Lacquer (Colored)           0.95
Lacquer (Clear on Al)       0.10
Lacquer (Clear on Cu)       0.65
Lead (Polished)             0.05 - 0.10
Lead (Oxidized)             0.30 - 0.65
Lead (Rough)                0.40
Limestone                   0.95 - 1.00
Magnesium Oxide             0.55
Molybdenum (Polished)       0.05
Molybdenum (Oxidized)       0.20 - 0.80
Monel (Oxidized)            0.45 - 0.85
Mullite                     0.80 - 0.85
Nichrome (Clean)            0.65
Nichrome (Oxidized)         0.60 - 0.85
Nickel (Polished)           0.10
Nickel (Oxidized)           0.20 - 0.95
Nickel Oxide                0.60
Oil (Animal/vegetable)      0.95 - 1.00
Oil (Mineral)               0.90 - 1.00
Oil (0.001” thick)          0.25
Oil (0.002” thick)          0.46
Oil (0.005” thick)          0.70
Paint (Aluminum paint)      0.50
Paint (Bronze paint)        0.80
Paint (on metal)            0.60 - 0.90
Paint (on plastic or wood)  0.80 - 0.95
Paint (Gold enamel)         0.40	
Paint (Clear silicone)      0.65 - 0.80
Paper                       0.85 - 1.00	
Plaster                     0.90	
Plastic                     0.95 - 1.00
Platinum                    0.05
Polyester                   0.75 - 0.85
Polyethylene                0.10
Quartz                      0.90
Roofing Paper               0.90
Rubber (Hard glossy)        0.95
Rubber (Soft rough)         0.85
Sand                        0.80 - 0.90
Sandstone                   0.70
Shale                       0.70
Silica (Powder)             0.35 - 0.60
Silica (Glazed)             0.85
Silica (Unglazed)           0.75
Silicone Carbide            0.80 - 0.95
Silver                      0.05
Soil (Dry)                  0.90 - 0.95	
Soil (Wet)                  0.95 - 1.00
Slate                       0.70 - 0.80
Stainless Steel (Polished)  0.10 - 0.15
Stainless Steel (Oxidized)  0.45 - 0.95
Steel (Unoxidized)          0.10
Steel (Oxidized)            0.70 - 0.95
Steel (Cold Rolled)         0.70 - 0.90
Steel (Ground sheet)        0.40 - 0.60
Steel (Rough surface)       0.95
Tantalum (Unoxidized)       0.20
Tantalum (Oxidized)         0.60
Textiles (Carpet)           0.85 - 1.00
Textiles (Close weave)      0.70 - 0.95
Textiles (Cotton)           0.80
Textiles (Leather)          0.95 - 1.00
Textiles (Silk)             0.80
Textiles (Died black)       0.98
Tin (Unoxidized)            0.05 - 0.10
Tungsten (Unoxidized)       0.05
Tungsten (Filament)         0.30
Water (Liquid)              0.90 - 0.95
Water (Ice)                 0.95 - 1.00
Water (Snow)                0.80 - 1.00
Wood (Planed)               0.80 - 0.95
Wood (Sawdust)              0.75
Zinc (Polished)             0.05
Zinc (Oxidized)             0.10
Zinc (Galvanized)           0.20 - 0.30
*/

bool  mlx9061x_init(void);
bool  mlx9061x_readTargetC(float *temp);
bool  mlx9061x_readTargetF(float *temp);
bool  mlx9061x_readAmbientC(float *temp);
bool  mlx9061x_readAmbientF(float *temp);
bool  mlx9061x_readEmissivity(float *emissivity);
bool  mlx9061x_writeEmissivity(float emissivity);

#endif
