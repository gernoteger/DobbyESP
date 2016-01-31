##########################################################################
#
# This makefile hold all environment-specific settings
#
##########################################################################
# my local settings: just use like Makefile: make -f Makefile.local all
#ESPTOOL2 		= $(abspath esptool2/esptool2)

ifeq ($(OS),Windows_NT)

    SDK_BASE      	= C:/Espressif/ESP8266_SDK
    XTENSA_BINDIR 	= C:/Espressif/xtensa-lx106-elf/bin

    ESP_HOME		= c:/Espressif

    COM_PORT = COM4

    # spiffy should come from inside
    SPIFFY =C:/tools/mingw64/bin/spiffy
    #SPIFFY = spiffy/build/spiffy

else

    #ESP_HOME		= c:/Espressif => from outside!!
    #need explicit path here
    DOBBY_HOME      := /home/gernot/dev/geger.at/DobbyESP/
    
    SDK_BASE      	:= $(ESP_HOME)/sdk
    XTENSA_BINDIR 	:= $(ESP_HOME)/xtensa-lx106-elf/bin

 
    COM_PORT = /dev/ttyUSB0
    COM_SPEED_SERIAL =115200
    #for boot: 76800!!
    #COM_SPEED_SERIAL=76800

    # spiffy should come from inside
    SPIFFY = ${DOBBY_HOME}/spiffy/build/spiffy
    #SPIFFY = spiffy/build/spiffy
	KILL_TERM    ?= pkill "$(TERM)" || exit 0
	#TERMINAL     ?= python -m serial.tools.miniterm $(COM_PORT) $(COM_SPEED_SERIAL)
	# for me: python -m serial.tools.miniterm /dev/ttyUSB0 76800 &
	# gtk has problems with boot baudrate 76800
	TERMINAL     ?= gtkterm --port $(COM_PORT) --speed $(COM_SPEED_SERIAL) &
endif
