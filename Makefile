#####################################################################
#### Please don't change this file. Use Makefile-user.mk instead ####
#####################################################################
# Including user Makefile.
# Should be used to set project-specific parameters
include ./Makefile-user.mk

# Important parameters check.
# We need to make sure SMING_HOME and ESP_HOME variables are set.
# You can use Makefile-user.mk in each project or use enviromental variables to set it globally.
 
ifndef ESP_HOME
$(error ESP_HOME is not set. Please configure it in Makefile-user.mk)
endif

# Include main Sming Makefile
#include $(SMING_HOME)/Makefile-project.mk# top level makefile; triggers hierarchical build for everythings

# rBoot settings
# TODO

# rBoot environment
ESPTOOL2      ?= $(abspath esptool2/esptool2)

export PATH := $(ESP_HOME)/xtensa-lx106-elf/bin:$(PATH)
XTENSA_TOOLS_ROOT := $(ESP_HOME)/xtensa-lx106-elf/bin

# XTENSA_BINDIR needs trailing slash or can be blank if already in $PATH
XTENSA_BINDIR := $(XTENSA_TOOLS_ROOT)/
SDK_BASE = $(ESP_HOME)/sdk

# call targets; -e needed to export variables

RBOOTTOOLS_HOME = $(abspath ./raburton_esp8266)
#rboot clean
rboot_clean:
	$(MAKE) -C $(RBOOTTOOLS_HOME) clean

rboot:
	$(MAKE) -C $(RBOOTTOOLS_HOME) -e esptool2 rboot
