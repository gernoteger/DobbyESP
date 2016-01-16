#####################################################################
#### call the makefiles.. 										####
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


# Include main Sming Makefile
ifeq ($(RBOOT_ENABLED), 1)
include $(SMING_HOME)/Makefile-rboot.mk
else
include $(SMING_HOME)/Makefile-project.mk
endif


sming-clean:
	@echo "cleaning sming"
	@echo "make is '$(MAKE)'"
	@echo "SMING_HOME is '$(SMING_HOME)'"
	@$(MAKE) -C $(SMING_HOME) -f $(SMING_HOME)/Makefile clean

sming-all:
	@echo "cleaning sming"
	@echo "make is '$(MAKE)'"
	@echo "SMING_HOME is '$(SMING_HOME)'"
	@$(MAKE) -C $(SMING_HOME) -f $(SMING_HOME)/Makefile all
	