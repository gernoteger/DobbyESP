## Local build configuration
## Parameters configured here will override default and ENV values.
## Uncomment and change examples:

# get board info inside
include ./Makefile-board.mk


#Add your source directories here separated by space
MODULES = app

#TODO: has to go into Makefile!!
SMING_HOME = $(abspath ./sming/Sming)

##########################################################
# build environment
##########################################################
																																# # use /c/Espressif instead of c:/Espressif on Windows!				
ESP_HOME = /c/Espressif
NGINX_HOME = /c/tools/nginx-1.9.4
# rBoot settings
# TODO
BOOT_BIG_FLASH = 1

# rBoot environment

#TODO: fix thosepaths
ESPTOOL2      ?= $(abspath raburton_esp8266/esptool2/esptool2)
#SPIFFY = spiffy/build/spiffy
SPIFFY =C:/tools/mingw64/bin/spiffy


COM_PORT = COM4

#COM_SPEED_ESPTOOL = $(COM_SPEED) ???
COM_SPEED_ESPTOOL = 230400
#TODO form elsewhere!!
flashimageoptions=-fs 32m

##########################################################
#project specific; right here??
##########################################################


# spiffs input directory and output size
SPIFF_FILES = web/build
# size in k??
SPIFF_SIZE  = 204800
#SPIFF_SIZE  = 458752
SPIFF_START = 0x100000
