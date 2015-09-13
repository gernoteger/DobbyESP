
# Known Problems:

## won't compile:


	c:/Espressif/ESP8266_SDK/include/mem.h:8:21: error: implicit declaration of function 'pvPortMalloc' [-Werror=implicit-function-declaration]
	 #define os_malloc   pvPortMalloc
	                     ^
	c:/dev/ESP8266/sming/DobbyESP/sming/Sming/rboot/appcode/rboot-api.c:44:19: note: in expansion of macro 'os_malloc'
	  buffer = (uint8*)os_malloc(SECTOR_SIZE);

Solution: ???: add esp
add #include <esp_systemapi.h> to Sming/rboot/appcode/rboot-api.c
Strange!!
