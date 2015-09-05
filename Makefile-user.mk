## Local build configuration
## Parameters configured here will override default and ENV values.
## Uncomment and change examples:

#Add your source directories here separated by space
MODULES = app


##########################################################
# build environment
##########################################################
																																# # use /c/Espressif instead of c:/Espressif on Windows!				
ESP_HOME = /c/Espressif
COM_PORT = COM4

# esptools for rBoot
#ESPTOOL2 = C:/dev/ESP8266/sming/rburton_esp8266/esptool2/esptool2.exe
SPIFFY = C:/tools/mingw64/bin/spiffy.exe
