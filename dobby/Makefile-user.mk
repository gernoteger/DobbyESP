## Local build configuration
## Parameters configured here will override default and ENV values.
## Uncomment and change examples:

# local environment from above
include ../Makefile-local.mk

# get board info inside
include ./Makefile-board.mk
include ./Makefile-extras.mk



BUILD_VERSION=0.0.0-SNAPSHOT

# forcing Version
out/build/app/Version.o: FORCE
#$(BUILD_BASE)/app/Version.o: FORCE

FORCE:
	@echo "$(BUILD_BASE)"
	
#Add your source directories here separated by space
MODULES = app app/Devices app/mqtt

#TODO: has to go into Makefile!!
#SMING_HOME = $(abspath sming/Sming)
# must be proper windows full pathname!
SMING_HOME = c:/dev/ESP8266/sming/DobbyESP/sming/Sming

##########################################################
# build environment
##########################################################
																																# # use /c/Espressif instead of c:/Espressif on Windows!				


##### overridable rBoot options ####
## use rboot build mode
RBOOT_ENABLED = 1

# TODO
RBOOT_BIG_FLASH = 1

# rBoot options, overwrite then in the projects Makefile-user.mk
RBOOT_TWO_ROMS   = 0
RBOOT_ROM_0      = rom0
RBOOT_ROM_1      = rom1
RBOOT_SPIFFS_0   = 0x100000
RBOOT_SPIFFS_1   = 0x300000
RBOOT_LD_0 = rom0.ld
RBOOT_LD_1 = rom1.ld


# rBoot environment

#TODO: fix those paths
ESPTOOL2      ?= $(abspath ../raburton_esp8266/esptool2/esptool2)

##########################################################
COM_PORT = COM4

#COM_SPEED_ESPTOOL = $(COM_SPEED) ???
COM_SPEED_ESPTOOL = 230400
#TODO form elsewhere!!
#flashimageoptions=-fs 32m

##########################################################
#project specific; right here??
##########################################################


# spiffs input directory and output size
SPIFF_FILES = web/build
# size in k??
SPIFF_SIZE  = 204800
#SPIFF_SIZE  = 458752
#SPIFF_START = 0x100000


