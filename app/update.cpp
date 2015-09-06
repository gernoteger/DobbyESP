/**
 * all routines for ota update go here
 */


#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include <rboot-smingota.h>

#include <AppSettings.h>


//static const uint8 ota_ip[] = {192,168,1,12}; // rasp2
//static const uint8 ota_ip[] = {192,168,1,100}; // wirbel


#define FIRWARE_PATH "update/fw"
#define FILES_PATH "update/web"
#define UPDATES_PORT 80

static void ICACHE_FLASH_ATTR OtaUpdate_CallBack(bool result, uint8 rom_slot) {

	if(result == true) {
		// success
		if (rom_slot == FLASH_BY_ADDR) {
			Serial.println("Write successful.");
		} else {
			// set to boot new rom and then reboot
			Serial.printf("Firmware updated, rebooting to rom %d...\r\n", rom_slot);
			rboot_set_current_rom(rom_slot);
			System.restart();
		}
	} else {
		// fail
		Serial.println("Firmware update failed!");
	}
}

static void  update_app1(Stream & messages) {

	// start the upgrade process
	if (rboot_ota_start((ota_callback)OtaUpdate_CallBack)) {
		messages.println("Updating...");
	} else {
		messages.println("Updating failed!\r\n");
	}
}

void  update_app() {

	// start the upgrade process
	if (rboot_ota_start((ota_callback)OtaUpdate_CallBack)) {
		Serial.println("Updating...");
	} else {
		Serial.println("Updating failed!\r\n");
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



