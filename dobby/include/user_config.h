#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#ifdef __cplusplus
extern "C" {

#define BOOL bool

#endif



	//TODO: this will have to be redifined elsewhere
#define ROM_0_URL  "http://192.168.1.100:8081/rom0.bin"
//#define ROM_1_URL  "http://192.168.1.100:8081/rom1.bin"
#define SPIFFS_URL "http://192.168.1.100:8081/spiff_rom.bin"

//Test Gernot: skip SSL parts
#define MQTT_NO_SSL

#define MQTT_BUF_SIZE		1024
#define MQTT_RECONNECT_TIMEOUT 5
#define PROTOCOL_NAMEv311


//##TODO: hack??
//#define BOOL uint8_t

// UART config
#define SERIAL_BAUD_RATE 115200

// ESP SDK config
#define LWIP_OPEN_SRC
#define USE_US_TIMER



// Default types
#define __CORRECT_ISO_CPP_STDLIB_H_PROTO
#include <limits.h>
#include <stdint.h>

// Override c_types.h include and remove buggy espconn
#ifndef _C_TYPES_H_
#define _C_TYPES_H_

// Updated, compatible version of c_types.h
// Just removed types declared in <stdint.h>
#include <espinc/c_types_compatible.h>
#endif

#ifndef _NO_ESPCON_
#define _NO_ESPCON_
#endif



// System API declarations
#include <esp_systemapi.h>

// C++ Support
#include <esp_cplusplus.h>
// Extended string conversion for compatibility
#include <stringconversion.h>
// Network base API
#include <espinc/lwip_includes.h>

// Beta boards
//#define BOARD_ESP01

#ifdef __cplusplus
}
#endif

#endif
