##########################################################
# board  specific for ESP-12/7


# BOOT = none
# BOOT = old - boot_v1.1
# BOOT = new - boot_v1.2+
#BOOT=none
# APP = 0 - eagle.flash.bin + eagle.irom0text.bin
# APP = 1 - user1.bin
# APP = 2 - user2.bin
#APP=0


# configure Board

SPI_SIZE=4M
#SPI_MODE=QIO

# SPI_MODE: QIO, QOUT, DIO, DOUT
#SPI_MODE?=QIO
#COM_SPEED = 400000
# COM_SPEED	= 74880
# COM_SPEED: ?? 230400

# for flashing..
#COM_SPEED = 230400

# for serial IO
#COM_SPEED_SERIAL = 115200

##########################################################
# rBoot
##########################################################

# wie krieg ich das in CFLAGS?
#RBOOT_BUILD_SMING = 1
