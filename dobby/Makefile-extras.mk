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
_BUILD_GITREF := $(shell  git rev-parse HEAD )
#TODO: get timezone right!!
_BUILD_TIME :=  $(shell date  +%Y-%m-%dT%H:%M:%S)

# insert variables as compile options
EXTRA_CFLAGS += -D_BUILD_GITREF=\"$(_BUILD_GITREF)\"
EXTRA_CFLAGS += -D_BUILD_TIME=\"$(_BUILD_TIME)\"
EXTRA_CFLAGS += -DBUILD_VERSION=\"$(BUILD_VERSION)\"

EXTRA_CFLAGS += -DLOG_LEVEL=$(LOG_LEVEL)

#fixes for esp_mqtt

#EXTRA_CFLAGS += -D_C_TYPES_H_