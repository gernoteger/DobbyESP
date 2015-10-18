# ESP12 Schematics

| PIN LEFT          |                   | PIN Right 		   |
|------------------:|-------------------|----------------------|
|REST				|[] ########### []  |TXT(GPIO03)           |
|ADV/TOUT			|[] ########### []  |RXD(GPIO01)           |
|CH_PD				|[] ########### []  |GPIO5                 |
|GPIO16/USER/WAKE	|[] ########### []  |GPIO4                 |
|GPIO14/HSPICLK		|[] ########### []  |GPIO0/FLASH           |
|GPIO12/HSPIQ		|[] ########### []  |GPIO02                |
|GPIO13/RXD2/HSPID	|[] ########### []  |GPIO15/TXD2/HSPICS    |
|VCC				|[] ########### []  |#GND                  |

# External connections

GPIO0: for boot_ must be grounded when programming
CH_PD: must be VCC
GPIO15: pulled low; after reboot, has HIGH level, but shown as input!!! What's that for??

CH_PD  -- [R10k] -- VCC
GPIO15 -- [R10k] -- GND


So usable pins are:
GGPIO0 must be high during start,internal pullup, Low: start flash
GPIO02 must be high during start,internal pullup
GPIO04
GPIO05
GPIO12
GPIO13
GPIO14
GPIO15 (must be low during start: SPI!), can be reprogrammed => good for switches??
GPIO16 (used by rboot)

all in all, 9 usable pins, 6 fully functional, 3 restricted

as a rule: actors go to GPIOs 4,5,12,13,14, maybe 16
Pushbuttons: GPIO 0,2,15