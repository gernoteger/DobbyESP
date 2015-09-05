/**
 * all routines for ota update go here
 */


#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include <rboot-ota.h>

#include <AppSettings.h>


//static const uint8 ota_ip[] = {192,168,1,12}; // rasp2
static const uint8 ota_ip[] = {192,168,1,100}; // wirbel

#define HTTP_HEADER "Connection: keep-alive\r\n\
Cache-Control: no-cache\r\n\
User-Agent: rBoot-SmingSample/1.0\r\n\
Accept: */*\r\n\r\n"
/**/

#define FIRWARE_PATH "update/fw"
#define FILES_PATH "update/web"
#define UPDATES_PORT 80

static void OtaUpdate_CallBack(void *arg, bool result) {

	char msg[40];
	rboot_ota *ota = (rboot_ota*)arg;

	if(result == true) {
		// success, reboot
		os_sprintf(msg, "Firmware updated, rebooting to rom %d...\r\n", ota->rom_slot);
		Serial.println(msg);
		rboot_set_current_rom(ota->rom_slot);
		System.restart();
	} else {
		// fail, cleanup
		Serial.println("Firmware update failed!\r\n");
		os_free(ota->request);
		os_free(ota);
	}
}


/**
 * update application
 * output messages to stream
 */
void update_app(Stream & messages){

	uint8 slot;
	rboot_ota *ota;

	//TODO: get settings from AppSettings, resolve ip


	// create the update structure
	ota = (rboot_ota*)os_zalloc(sizeof(rboot_ota));
	os_memcpy(ota->ip, ota_ip, 4);
	ota->port = UPDATES_PORT;
	ota->callback = (ota_callback)OtaUpdate_CallBack;
	ota->request = (uint8 *)os_zalloc(512);

	// select rom slot to flash
	//uncomment these lines to enable multiple different roms
	// (only needed if not using big flash support with 1mb slots)
	slot = rboot_get_current_rom();
	if (slot == 0) slot = 1; else slot = 0;
	ota->rom_slot = slot;

	// actual http request
	os_sprintf((char*)ota->request,
		"GET /%s HTTP/1.1\r\nHost: " IPSTR "\r\n" HTTP_HEADER,
		// comment out next line if not using big flash
		FIRWARE_PATH "/" "rom0.bin",
		// and uncomment this one:
		//(slot == 0 ? "rom0.bin" : "rom1.bin"),
		IP2STR(ota->ip));

	// start the upgrade process
	if (rboot_ota_start(ota)) {
		messages.println("Updating...\r\n");
	} else {
		messages.println("Updating failed!\r\n\r\n");
		os_free(ota->request);
		os_free(ota);
	}
}


/**
 * update files
 */
void update_files(Stream & messages){
	messages.println("##TODO: not yet implemented");
}


/**
 * update files
 */
void update_switch_roms(Stream & messages){
	messages.println("switching...");
	uint8 before, after;
	before = rboot_get_current_rom();
	if (before == 0) after = 1; else after = 0;
	Serial.printf("Swapping from rom %d to rom %d.\r\n", before, after);
	rboot_set_current_rom(after);
	Serial.printf("Restarting...\r\n\r\n");
	System.restart();

}



