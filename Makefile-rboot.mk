# name for the target project
TARGET = app

include ./Makefile-user.mk

#TODO: aus Sming/Makefile-project übernehmen!!
include ./Makefile-windows.mk

# default SMING_HOME, can be overridden
SMING_HOME ?= /opt/Sming/Sming
# default ESP_HOME, can be overridden
ESP_HOME ?= /opt/esp-open-sdk
# sdk path, can be overridden
SDK_BASE ?= $(ESP_HOME)/sdk
# path to esptool2, can be overridden
ESPTOOL2 ?= ../esptool2/esptool2
# path to spiffy, can be overridden
SPIFFY   ?= spiffy

ifndef ESPTOOL
$(error ESPTOOL is not set. Please configure it in Makefile-user.mk)
endif


# use wifi settings from environment or hard code them here
WIFI_SSID ?= ""
WIFI_PWD ?= ""

# spiffs input directory and output size
SPIFF_FILES ?= spiffs
SPIFF_SIZE  ?= 458752

# output directories
BUILD_BASE	= out/build
FW_BASE		= out/firmware

# which modules (subdirectories) of the project to include in compiling
MODULES ?= app
MODULES += $(subst :,,$(SMING_HOME)/appinit)
EXTRA_INCDIR    ?= include $(SMING_HOME)/include $(SMING_HOME)/ $(SMING_HOME)/system/include $(SMING_HOME)/Wiring $(SMING_HOME)/Libraries $(SMING_HOME)/SmingCore $(SDK_BASE)/../include

# libraries used in this project, mainly provided by the SDK
USER_LIBDIRS = $(SMING_HOME)/compiler/lib/ $(BUILD_BASE)
LIBS		 = microc microgcc hal phy pp net80211 lwip wpa main2 sming $(EXTRA_LIBS)

# compiler flags using during compilation of source files
CFLAGS		= -Os -g -Wpointer-arith -Wundef -Werror -Wl,-EL -nostdlib -mlongcalls -mtext-section-literals -finline-functions -fdata-sections -ffunction-sections -D__ets__ -DICACHE_FLASH -DARDUINO=106 -DRBOOT_BUILD_SMING -DDISABLE_SPIFFS
CXXFLAGS	= $(CFLAGS) -fno-rtti -fno-exceptions -std=c++11 -felide-constructors

# linker flags used to generate the main object file
LDFLAGS		= -nostdlib -u call_user_start -u Cache_Read_Enable_New -Wl,-static -Wl,--gc-sections -Wl,-Map=$(basename $@).map

# linker script used for the above linker step
LD_SCRIPT_0 = rom0.ld
LD_SCRIPT_1 = rom1.ld

# various paths from the SDK used in this project
SDK_LIBDIR	= lib
SDK_INCDIR	= include

# filenames and options for generating rom images with esptool2
FW_SECTS = .text .data .rodata
FW_USER_ARGS = -quiet -bin -boot2
FW_ROM_0 = rom0
FW_ROM_1 = rom1
FW_RBOOT = rboot

# spiffs output filename
SPIFFS = spiffs

export COMPILE := gcc
export PATH := $(ESP_HOME)/xtensa-lx106-elf/bin:$(PATH)
XTENSA_TOOLS_ROOT := $(ESP_HOME)/xtensa-lx106-elf/bin

# select which tools to use as compiler, librarian and linker
CC		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
CXX		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-g++
AR		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-ar
LD		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
OBJCOPY := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-objcopy
OBJDUMP := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-objdump

####
#### no user configurable options below here
####
SRC_DIR		:= $(MODULES)
BUILD_DIR	:= $(addprefix $(BUILD_BASE)/,$(MODULES))

SDK_LIBDIR	:= $(addprefix $(SDK_BASE)/,$(SDK_LIBDIR))
SDK_INCDIR	:= $(addprefix -I$(SDK_BASE)/,$(SDK_INCDIR))

LIBMAIN = $(addprefix $(SDK_LIBDIR)/,libmain.a)
LIBMAIN2 = $(addprefix $(BUILD_BASE)/,libmain2.a)

SRC := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c*))
OBJ := $(patsubst %.c,$(BUILD_BASE)/%.o,$(SRC))
OBJ := $(patsubst %.cpp,$(BUILD_BASE)/%.o,$(OBJ))

LIBS		:= $(addprefix -l,$(LIBS))
APP_AR		:= $(addprefix $(BUILD_BASE)/,$(TARGET)_app.a)
TARGET_OUT_0	:= $(addprefix $(BUILD_BASE)/,$(TARGET)_0.out)
TARGET_OUT_1	:= $(addprefix $(BUILD_BASE)/,$(TARGET)_1.out)

LD_SCRIPT_0	:= $(addprefix -T,$(LD_SCRIPT_0))
LD_SCRIPT_1	:= $(addprefix -T,$(LD_SCRIPT_1))

INCDIR	:= $(addprefix -I,$(SRC_DIR))
EXTRA_INCDIR	:= $(addprefix -I,$(EXTRA_INCDIR))
#MODULE_INCDIR	:= $(addsuffix /include,$(INCDIR))

USER_LIBDIRS := $(addprefix -L,$(USER_LIBDIRS))

fw_rboot_bin  := $(addprefix $(FW_BASE)/,$(FW_RBOOT).bin)
FW_ROM_0  := $(addprefix $(FW_BASE)/,$(FW_ROM_0).bin)
FW_ROM_1  := $(addprefix $(FW_BASE)/,$(FW_ROM_1).bin)

SPIFFS    := $(addprefix $(FW_BASE)/,$(SPIFFS).bin)



# Version & Date information from git + timestamp
BUILD_GITREF := $(shell  git rev-parse --short HEAD )
#TODO: usue ISO8601 format
BUILD_TIME :=  $(shell date  +%Y-%m-%dT%H:%M:%S)


CFLAGS += -DBUILD_GITREF=\"$(BUILD_GITREF)\"
CFLAGS += -DBUILD_TIME=\"$(BUILD_TIME)\"


ifneq ($(WIFI_SSID), "")
	CFLAGS += -DWIFI_SSID=\"$(WIFI_SSID)\"
endif
ifneq ($(WIFI_PWD), "")
	CFLAGS += -DWIFI_PWD=\"$(WIFI_PWD)\"
endif

ifeq ($(TWO_ROMS),1)
	# set a define so ota code can choose correct rom
	CFLAGS += -DTWO_ROMS
else
	# eliminate the second rom make target from the build
	FW_ROM_1 = $()
endif

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

define compile-objects
$1/%.o: %.c
	@echo "CC $$<"
	@$(CC) $(INCDIR) $(MODULE_INCDIR) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CFLAGS) -c $$< -o $$@	
$1/%.o: %.cpp
	@echo "C+ $$<" 
	$(CXX) $(INCDIR) $(MODULE_INCDIR) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CXXFLAGS) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

# single rom image for rBoot big flash support and 1mb slots
#all: checkdirs $(LIBMAIN2) $(FW_ROM_0) $(SPIFFS)
# dual rom images for rBoot without big flash support and/or two smaller rom slots
all: checkdirs $(LIBMAIN2) $(fw_rboot_bin) $(FW_ROM_0) $(FW_ROM_1) $(SPIFFS)
	@echo "ALL FW_ROM_0=$(FW_ROM_0) FW_ROM_1=$(FW_ROM_1) SPIFFS=$(SPIFFS)"

$(SPIFFS):
	@echo "SP $@"
	@echo files from $(SPIFF_FILES)
	$(SPIFFY) $(SPIFF_SIZE) $(SPIFF_FILES)
	@mv spiff_rom.bin $(SPIFFS)
	@echo "SP done."

$(LIBMAIN2): $(LIBMAIN)
	@echo "OC $@"
	@$(OBJCOPY) -W Cache_Read_Enable_New $^ $@

#TODO: merge with Makefile rule!!
$(fw_rboot_bin):
	@echo "CP $@"
	@cp raburton_esp8266/rboot/firmware/rboot.bin $(fw_rboot_bin)

$(FW_ROM_0): $(TARGET_OUT_0)
	@echo "E2 $@"
	@$(ESPTOOL2) $(FW_USER_ARGS) $(TARGET_OUT_0) $@ $(FW_SECTS)

$(FW_ROM_1): $(TARGET_OUT_1)
	@echo "E2 $@"
	@$(ESPTOOL2) $(FW_USER_ARGS) $(TARGET_OUT_1) $@ $(FW_SECTS)

$(TARGET_OUT_0): $(APP_AR)
	@echo "LD $@"
	@$(LD) $(USER_LIBDIRS) -L$(SDK_LIBDIR) $(LD_SCRIPT_0) $(LDFLAGS) -Wl,--start-group $(APP_AR) $(LIBS) -Wl,--end-group -o $@

$(TARGET_OUT_1): $(APP_AR)
	@echo "LD $@"
	@$(LD) $(USER_LIBDIRS) -L$(SDK_LIBDIR) $(LD_SCRIPT_1) $(LDFLAGS) -Wl,--start-group $(APP_AR) $(LIBS) -Wl,--end-group -o $@

$(APP_AR): $(OBJ)
	@echo "AR $@"
	@$(AR) cru $@ $^

checkdirs: $(BUILD_DIR) $(FW_BASE)

$(BUILD_DIR):
	@mkdir -p $@

$(FW_BASE):
	@mkdir -p $@

.PHONY: flash flash_rboot flash_rboot_app version
flash: all
	$(vecho) "Killing Terminal to free $(COM_PORT)"
	-$(Q) $(KILL_TERM)
ifeq ($(DISABLE_SPIFFS), 1)
	$(ESPTOOL) -p $(COM_PORT) -b $(COM_SPEED_ESPTOOL) write_flash $(flashimageoptions) 0x00000 $(FW_BASE)/0x00000.bin 0x09000 $(FW_BASE)/0x09000.bin
else
	$(ESPTOOL) -p $(COM_PORT) -b $(COM_SPEED_ESPTOOL) write_flash $(flashimageoptions) 0x00000 $(FW_BASE)/0x00000.bin 0x09000 $(FW_BASE)/0x09000.bin $(SPIFF_START_OFFSET) $(FW_BASE)/spiff_rom.bin
endif
	$(TERMINAL)

flash_rboot: 
	echo "Killing Terminal to free $(COM_PORT)"
	@$(KILL_TERM)
	
#	esptool.py --port <port> write_flash -fs 32m 0x00000 rboot.bin 0x02000 rom0.bin 0x100000 spiffs.rom
	$(ESPTOOL) -p $(COM_PORT) -b $(COM_SPEED_ESPTOOL) write_flash $(flashimageoptions) 0x00000 $(FW_BASE)/rboot.bin  0x02000 $(FW_BASE)/rom0.bin 0x100000 $(FW_BASE)/spiffs.bin

	$(TERMINAL)

flash_rboot_app: 
	$(vecho) "Killing Terminal to free $(COM_PORT)"
	-$(Q) $(KILL_TERM)
	
#	esptool.py --port <port> write_flash -fs 32m 0x00000 rboot.bin 0x02000 rom0.bin 0x100000 spiffs.rom
	$(ESPTOOL) -p $(COM_PORT) -b $(COM_SPEED_ESPTOOL) write_flash $(flashimageoptions) 0x02000 $(FW_BASE)/rom0.bin
# what about rboot settings?

	$(TERMINAL)

flashinit:
	echo "Flash init data default and blank data."
	$(ESPTOOL) -p $(COM_PORT) -b $(COM_SPEED_ESPTOOL) write_flash $(flashimageoptions) 0x7c000 $(SDK_BASE)/bin/esp_init_data_default.bin 0x7e000 $(SDK_BASE)/bin/blank.bin 0x4B000 $(SMING_HOME)/compiler/data/blankfs.bin

# show sw version (mainly for debugging!)
version:
	@echo "gitref: $(BUILD_GITREF)"
	@echo "BUILD_TIME: $(BUILD_TIME)"
	

clean:
	@rm -rf $(BUILD_BASE)
	@rm -rf $(FW_BASE)

$(foreach bdir,$(BUILD_DIR),$(eval $(call compile-objects,$(bdir))))
