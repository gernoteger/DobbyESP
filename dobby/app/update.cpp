/**
 * all routines for ota update go here
 */


#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include <AppSettings.h>

#include "update.h"
#include "telnet.h" //TODO: unclean-> restructure API

//static const uint8 ota_ip[] = {192,168,1,12}; // rasp2
//static const uint8 ota_ip[] = {192,168,1,100}; // wirbel


#define UPDATES_PORT 80

//TODO: get from make??
#define FILES_COUNT 3
String FILES[FILES_COUNT]={"index.html","settings.html","system.html"};


rBootHttpUpdate* otaUpdater = 0;
Print * otaMessages=NULL;

Timer restartTimer;

void do_restart_cb(){
	System.restart();
}
/**
 * initiate a restart & inform
 * @param msg
 */
void do_restart(Print * msg){
	if(msg) msg->println("disconnecting...");
	stopTelnetServer();//TODO: shutdown all networks
	if(msg) msg->println("restarting...");

	restartTimer.initializeMs(1000,do_restart_cb).startOnce();
}

void OtaUpdate_CallBack(bool result) {
	if(!otaMessages)otaMessages=&Serial;
	otaMessages->println("In callback...");
	if(result == true) {
		// success
		uint8 slot;
		slot = rboot_get_current_rom();
		if (slot == 0) slot = 1; else slot = 0;
		// set to boot new rom and then reboot
		otaMessages->printf("Firmware updated. Reboot to rom %d\r\n", slot);
		rboot_set_current_rom(slot);



		do_restart(otaMessages);
	} else {
		// fail
		otaMessages->println("Firmware update failed!");
	}
}

/**
 * update and reboot if successful
 */
void ICACHE_FLASH_ATTR update_app(Print * messages, bool includeFiles)
{
	uint8 slot;
	rboot_config bootconf;

	otaMessages=messages;
	messages->println("Updating...");

	// need a clean object, otherwise if run before and failed will not run again
	if (otaUpdater) delete otaUpdater;
	otaUpdater = new rBootHttpUpdate();
	if(!otaUpdater){
		messages->println("####ERROR: Could not create rBootHttpUpdate; aborting.");
		return;
	}
	// select rom slot to flash
	bootconf = rboot_get_config();
	slot = bootconf.current_rom;
	if (slot == 0) slot = 1; else slot = 0;

#ifndef RBOOT_TWO_ROMS
	// flash rom to position indicated in the rBoot config rom table
	otaUpdater->addItem(bootconf.roms[slot], ROM_0_URL);
#else
	// flash appropriate rom
	if (slot == 0) {
		otaUpdater->addItem(bootconf.roms[slot], ROM_0_URL);
	} else {
		otaUpdater->addItem(bootconf.roms[slot], ROM_1_URL);
	}
#endif

#ifndef DISABLE_SPIFFS

	if(includeFiles){
		// use user supplied values (defaults for 4mb flash in makefile)
		if (slot == 0) {
			otaUpdater->addItem(RBOOT_SPIFFS_0, SPIFFS_URL);
		} else {
			otaUpdater->addItem(RBOOT_SPIFFS_1, SPIFFS_URL);
		}
	}
#endif

	// request switch and reboot on success
	//otaUpdater->switchToRom(slot);
	// and/or set a callback (called on failure or success without switching requested)
	otaUpdater->switchToRom(NO_ROM_SWITCH); // TODO: needed??
	otaUpdater->setCallback(OtaUpdate_CallBack);

	// start update
	otaUpdater->start();
}


//TODO: doesn't work yet!!
class UpdateClient : protected HttpClient
{
public:
	UpdateClient()
	{
		updateCount=0;
	}

	void updateFile(const String url)
	{
		downloadFile(url,HttpClientCompletedDelegate(&UpdateClient::processed, this));
	}

	void processed(HttpClient& client, bool successful)
	{
		Serial.printf("file downloaded: succes=%u\r\n",successful?1:0);
		close();
	}

	void processedNextFile(HttpClient& client, bool successful)
	{
		Serial.printf("file downloaded: succes=%u\r\n",successful?1:0);
		if(updateCount++ < FILES_COUNT){
			updateNextFile();
		}else{
			close();
		}

	}


	void updateNextFile(){
		// skip hidden files
		String url=filesurl+FILES[updateCount];
		Serial.printf("updating file[%d]='%s'\r\n",updateCount,FILES[updateCount].c_str());

		if(isProcessing()){
			close();
		}
		downloadFile(url,HttpClientCompletedDelegate(&UpdateClient::processedNextFile, this));
	}

	/**
	 * update all files in directory not starting with '.'
	 */
	void updateFiles(){

		updateCount=0;

		// start chain of events..
		updateNextFile();
	}
private:
	uint16 updateCount=0;
	//String  files[] ={"index.html","settings.html","system.html"};
	const char *filesurl = "http://192.168.1.100/update/web/";
};

UpdateClient updater;

/**
 * download a file to spiffs
 * TODO: will change to event list!!
 */

// keep static; TODO why?
HttpClient downloadclient;


void  ICACHE_FLASH_ATTR update_downloadFile(const String & fname,const String url){
	Serial.printf("downloading %s from %s\r\n",fname.c_str(),url.c_str());

	if(downloadclient.isProcessing()){
		Serial.println("be patient...");
		return;
	}

	downloadclient.reset();
	/**
	 * call withhout HttpClientCompletedDelegate for now
	 */

	//TODO: HttpClient::downloadFile is buggy: when 404, will delete file!
	//BUG: HttpClient::downloadFile is buggy: when 404, will delete file!
	//downloadclient.downloadFile(url,HttpClientCompletedDelegate(&file_downloaded, downloadclient));
}

/**
 * update all files. Target algorithm: first compare, then update - if they don't change, its faster to download twice than flash once!
 * for now: only update files I edit..
 */
void ICACHE_FLASH_ATTR update_files(Print * out){
	out->println("##TODO: not yet implemented 2");

	String file("system.html");
	String url("http://192.168.1.100/update/web/system.html");

	//TODO: how to allocate strings effectively??
	//update_downloadFile(file,url);
	//updater.updateFile(url);
	updater.updateFiles();
}


/**
 * update files
 */
void ICACHE_FLASH_ATTR update_switch_roms(Print * messages){
	messages->println("switching...");
	uint8 before, after;
	before = rboot_get_current_rom();
	if (before == 0) after = 1; else after = 0;
	messages->printf("Swapping from rom %d to rom %d.\r\n", before, after);
	rboot_set_current_rom(after);
	messages->printf("Restarting...\r\n\r\n");
	do_restart(messages);

}

void ICACHE_FLASH_ATTR update_print_config(Print * printer){

	printer->println("--rboot--");

#ifdef BOOT_BIG_FLASH
	printer->println("BOOT_BIG_FLASH ON");
#else
	printer->println("BOOT_BIG_FLASH OFF");
#endif

	printer->printf("config starting at %x\r\n",BOOT_CONFIG_SECTOR * SECTOR_SIZE);

	rboot_config bootconf = rboot_get_config();

	printer->printf("magic=%x\r\n",bootconf.magic);
	printer->printf("version=%u\r\n",bootconf.version);
	printer->printf("mode=%u\r\n",bootconf.mode);
	printer->printf("current_rom=%u\r\n",bootconf.current_rom);
	printer->printf("gpio_rom=%u\r\n",bootconf.gpio_rom);
	printer->printf("count=%u\r\n",bootconf.count);

	for(uint8 i=0;i<MAX_ROMS;i++){
		printer->printf("roms[%u]=%x\r\n",i,bootconf.roms[i]);
	}
#ifdef BOOT_CONFIG_CHKSUM
	printer->printf("chksum=%u\r\n",bootconf.chksum);
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

			update_print_config(& Serial);
		}
		//TODO: what about GPIO mode?
	}

}
