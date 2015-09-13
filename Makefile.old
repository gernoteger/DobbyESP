#####################################################################
#### Please don't change this file. Use Makefile-user.mk instead ####
#####################################################################
# Including user Makefile.
# Should be used to set project-specific parameters
include ./Makefile-user.mk

# Important parameters check.
# We need to make sure SMING_HOME and ESP_HOME variables are set.
# You can use Makefile-user.mk in each project or use enviromental variables to set it globally.
 
ifndef SMING_HOME
$(error SMING_HOME is not set. Please configure it in Makefile-user.mk)
endif

ifndef ESP_HOME
$(error ESP_HOME is not set. Please configure it in Makefile-user.mk)
endif

EXTRA_CFLAGS =

EXTRA_CFLAGS += -DSPIFF_SIZE=$(SPIFF_SIZE)
EXTRA_CFLAGS += -DSPIFF_START=$(SPIFF_START)

# Include main Sming Makefile??
# top level makefile; triggers hierarchical build for everythings
#include $(SMING_HOME)/Makefile-project.mk
#include Makefile-rboot.mk
include $(SMING_HOME)/Makefile-rboot.mk


# adjustments for my project

CFLAGS += -DSPIFF_SIZE=$(SPIFF_SIZE)
CFLAGS += -DSPIFF_START=$(SPIFF_START)


# Version & Date information from git + timestamp
BUILD_GITREF := $(shell  git rev-parse --short HEAD )
#TODO: get timezone right!!
BUILD_TIME :=  $(shell date  +%Y-%m-%dT%H:%M:%S)

# insert variables as compile options
CFLAGS += -DBUILD_GITREF=\"$(BUILD_GITREF)\"
CFLAGS += -DBUILD_TIME=\"$(BUILD_TIME)\"


#spiffy

SPIFFY_HOME ?= $(abspath spiffy)

SPIFFY_BUILDDIR = $(SPIFFY_HOME)/build
$(SPIFFY_BUILDDIR):
	@mkdir -p $@

spiffy: $(SPIFFY_BUILDDIR)
	@echo "SPIFFY"
	$(MAKE) -C $(SPIFFY_HOME)

spiffy_clean:
	$(MAKE) -C $(SPIFFY_HOME) clean


# rBoot settings
# TODO
BOOT_BIG_FLASH ?= 1

# rBoot environment
ESPTOOL2      ?= $(abspath esptool2/esptool2)
SPIFFY = $(SPIFFY_BUILDDIR)/spiffy

export PATH := $(ESP_HOME)/xtensa-lx106-elf/bin:$(PATH)
XTENSA_TOOLS_ROOT := $(ESP_HOME)/xtensa-lx106-elf/bin

# XTENSA_BINDIR needs trailing slash or can be blank if already in $PATH
XTENSA_BINDIR := $(XTENSA_TOOLS_ROOT)/
SDK_BASE = $(ESP_HOME)/sdk

# call targets; -e needed to export variables

RBOOTTOOLS_HOME = $(abspath ./raburton_esp8266)
RBOOT_HOME = $(RBOOTTOOLS_HOME)/rboot
#rboot clean
rboot_clean:
	$(MAKE) -C $(RBOOTTOOLS_HOME) clean

rboot: 
	$(MAKE) -C $(RBOOTTOOLS_HOME) -e esptool2 rboot
	
# make as phony target, and copy unconditionally for now!
rboot_sources:
# source files from rboot
	cp $(RBOOT_HOME)/appcode/rboot-bigflash.c app
	cp $(RBOOT_HOME)/appcode/rboot-api.c app
	cp $(RBOOT_HOME)/appcode/rboot-api.h include
	cp $(RBOOT_HOME)/rboot.h include

#all: spiffy rboot

# nginx startup

#cat := $(if $(filter $(OS),Windows_NT),type,cat)
#variable := $(shell cat filename)

NGINX ?= $(NGINX_HOME)/nginx

NGINX_CONF = ./nginx/conf/nginx.conf
NGINX_UPDATECONF = ./nginx/conf/update.conf
NGINX_MODULE = ./nginx

#TODO
FIRMWARE_DIR=$(abspath ./out/firmware)
WEB_DIR=$(abspath ./web/build)

$(NGINX_UPDATECONF): $(NGINX_UPDATECONF).tpl
	echo $@
	sed  -e 's:\[FIRMWARE_DIR\]:$(FIRMWARE_DIR):g' -e 's:\[WEB_DIR\]:$(WEB_DIR):g'  $@.tpl > $@
#	echo "$$(NGINMX_UF_CONTENT)" > $@
#	@echo hello > $@
	$(files $@,hi)
	
	
	
	
nginx-start: $(NGINX_UPDATECONF)
	cd $(NGINX_MODULE)
	$(NGINX) -c $(NGINX_CONF) -p $(NGINX_MODULE) &
	
nginx-stop:
	$(NGINX) -p $(NGINX_MODULE) -s stop


.PHONY: all spiffy rboot rboot_clean spiffy_clean nginx-start nginx-stop rboot_sources
