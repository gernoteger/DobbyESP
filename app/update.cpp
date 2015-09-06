/**
 * all routines for ota update go here
 */


#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include <rboot-smingota.h>

#include <AppSettings.h>

#include "update.h"

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

static void  ICACHE_FLASH_ATTR update_app1(Stream & messages) {

	// start the upgrade process
	if (rboot_ota_start((ota_callback)OtaUpdate_CallBack)) {
		messages.println("Updating...");
	} else {
		messages.println("Updating failed!\r\n");
	}
}

void  ICACHE_FLASH_ATTR update_app() {

	// start the upgrade process
	if (rboot_ota_start((ota_callback)OtaUpdate_CallBack)) {
		Serial.println("Updating...");
	} else {
		Serial.println("Updating failed!\r\n");
	}
}




/**
 * download a file to spiffs
 * TODO: will change to event list!!
 */

// keep static; TODO why?
HttpClient downloadclient;

void  ICACHE_FLASH_ATTR update_downloadFile(const String & fname,const String url){
	Serial.printf("downloading %s from %s\r\n",fname.c_str(),url.c_str());


	downloadclient.reset();
	/**
	 * call withhout HttpClientCompletedDelegate for now
	 */

	//TODO: HttpClient::downloadFile is buggy: when 404, will delete file!
	//BUG: HttpClient::downloadFile is buggy: when 404, will delete file!
	downloadclient.downloadFile(url);
}

/**
 * update all files. Target algorithm: first compare, then update - if they don't change, its faster to download twice than flash once!
 * for now: only update files I edit..
 */
void ICACHE_FLASH_ATTR update_files(Stream & messages){
	messages.println("##TODO: not yet implemented");

	String file("system.html");
	String url("http://192.168.1.100/update/web/system.html");

	//TODO: how to allocate strings effectively??
	update_downloadFile(file,url);
}


/**
 * update files
 */
void ICACHE_FLASH_ATTR update_switch_roms(Stream & messages){
	messages.println("switching...");
	uint8 before, after;
	before = rboot_get_current_rom();
	if (before == 0) after = 1; else after = 0;
	Serial.printf("Swapping from rom %d to rom %d.\r\n", before, after);
	rboot_set_current_rom(after);
	Serial.printf("Restarting...\r\n\r\n");
	System.restart();

}

void ICACHE_FLASH_ATTR update_print_config(){

	 Serial.println("--rboot--");

#ifdef BOOT_BIG_FLASH
	 Serial.println("BOOT_BIG_FLASH ON");
#else
	 Serial.println("BOOT_BIG_FLASH OFF");
#endif

	Serial.printf("config starting at %x\r\n",BOOT_CONFIG_SECTOR * SECTOR_SIZE);

	rboot_config bootconf = rboot_get_config();

	Serial.printf("magic=%x\r\n",bootconf.magic);
	Serial.printf("version=%u\r\n",bootconf.version);
	Serial.printf("mode=%u\r\n",bootconf.mode);
	Serial.printf("current_rom=%u\r\n",bootconf.current_rom);
	Serial.printf("gpio_rom=%u\r\n",bootconf.gpio_rom);
	Serial.printf("count=%u\r\n",bootconf.count);

	for(uint8 i=0;i<MAX_ROMS;i++){
		Serial.printf("roms[%u]=%x\r\n",i,bootconf.roms[i]);
	}
#ifdef BOOT_CONFIG_CHKSUM
	Serial.printf("chksum=%u\r\n",bootconf.chksum);
#endif


}

/**
 * check rboot config, and adopt rom positions if needed; should happen in build process!!
 */
void ICACHE_FLASH_ATTR update_check_rboot_config(){
	rboot_config conf = rboot_get_config();
	// assume we have 2 roms
	if(conf.count!=2){
		Serial.printf("rboot_config: wrong number of arguments: %u expected: 2\r\n",conf.count);
	}

	if(conf.count>=2){
		/**
		 * default: 2 roms, rom0=0x2000, rom1=0x82000
		 */
		//TODO: get from Makefile!
		uint32 rom0=0x002000;
		uint32 rom1=0x202000;
		//uint32 spiffs=0x100000; //only docu

		// write flash adress of roms!
		if(conf.roms[0]!=rom0 || conf.roms[1]!=rom1){
			Serial.printf("updating rboot config: rom0=%x rom1=%x\r\n",rom0,rom1);
			conf.roms[0]=rom0;
			conf.roms[1]=rom1;

			rboot_set_config(&conf);

			update_print_config();
		}
		//TODO: what about GPIO mode?
	}

}
