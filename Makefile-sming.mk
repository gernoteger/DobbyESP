##########################################################################
#
# this is a makefile to call sming basic make  make with my user options
# usage:
#    make -f Makefile-sming.mk all|clean
#
##########################################################################
#
###################################################################
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

# Include main Sming Makefile

# needed to get all variables to sub
export 
unexport MODULES

test:
	@echo test
	
test1:
	@echo test1
	
# call targets; -e needed to export variables
clean:
	$(MAKE) -f $(SMING_HOME)/Makefile -C $(SMING_HOME) -e clean
	
all:
	$(MAKE) -C $(SMING_HOME) -e all

.PHONY: clean all test