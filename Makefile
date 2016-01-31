#####################################################################
#### use this to make sming itself								 ####
#####################################################################
# Including user Makefile.
# Should be used to set project-specific parameters
include ./Makefile-local.mk

# Important parameters check.
# We need to make sure SMING_HOME and ESP_HOME variables are set.
# You can use Makefile-user.mk in each project or use enviromental variables to set it globally.
 

ifndef ESP_HOME
$(error ESP_HOME is not set. Please configure it in Makefile-user.mk)
endif

export SMING_HOME := sming/Sming
export EXTRA_CFLAGS=-DDISABLE_SPIFFS_AUTO

# sming builds with switches
clean:
	@$(MAKE) -C $(SMING_HOME) clean
	@$(MAKE) -C dobby clean
	@$(MAKE) -C spiffy clean

sming:
	@$(MAKE) -C $(SMING_HOME) all
	
dobby:
	@$(MAKE) -C dobby all

spiffy:
	@$(MAKE) -C spiffy all

# funktioniert nicht!!	
# esp_mqtt is not used at this level!!
#esp-mqtt-all:
#	@$(MAKE) -C esp_mqtt all
#	
#esp-mqtt-clean:
#	@$(MAKE) -C esp_mqtt clean
	
	
all: sming dobby

.PHONY: clean sming dobby all
