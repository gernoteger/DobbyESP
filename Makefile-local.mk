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

    #ESP_HOME		= c:/Espressif
    #need explicit path here
    DOBBY_HOME      := /home/gernot/dev/geger.at/DobbyESP/
    
    SDK_BASE      	= ${ESP_HOME}/sdk
    XTENSA_BINDIR 	= ${ESP_HOME}/xtensa-lx106-elf/bin

 
    COM_PORT = /dev/ttyUSB0

    # spiffy should come from inside
    SPIFFY = ${DOBBY_HOME}/spiffy/build/spiffy
    #SPIFFY = spiffy/build/spiffy

endif
