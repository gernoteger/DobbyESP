###################################################################
# additions to the standard makefile. included by Makefile
###################################################################

EXTRA_CFLAGS =

EXTRA_CFLAGS += -DSPIFF_SIZE=$(SPIFF_SIZE)
EXTRA_CFLAGS += -DSPIFF_START=$(SPIFF_START)


# adjustments for my project

EXTRA_CFLAGS += -DSPIFF_SIZE=$(SPIFF_SIZE)
EXTRA_CFLAGS += -DSPIFF_START=$(SPIFF_START)


# Version & Date information from git + timestamp
# do not use --short, since it's for future reference!
BUILD_GITREF := $(shell  git rev-parse HEAD )
#TODO: get timezone right!!
BUILD_TIME :=  $(shell date  +%Y-%m-%dT%H:%M:%S)

# insert variables as compile options
EXTRA_CFLAGS += -DBUILD_GITREF=\"$(BUILD_GITREF)\"
EXTRA_CFLAGS += -DBUILD_TIME=\"$(BUILD_TIME)\"

