/**
 * my app...
 */





#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include <FileSystem.h>
#include <user_interface.h>
#include <Debug.h>
#include <ctype.h>

#include "buildinfo.h"
#include "Version.h"

#include "update.h"
#include "IOHandler.h"
#include "CommandLine.h"

#include "Logger.h"
#include "Node.h"


//#define LOG_LEVEL -1

#include "logging.h"

#include "GELFAppender.h"

// das geht nicht.....
//#undef LOG_LEVEL
//#define LOG_LEVEL LOG_LEVEL_OFF

using namespace dobby;

//static bool state = true;
//BssList networks;
//String network, password;


void mount_spiffs(){

#ifndef DISABLE_SPIFFS
	int slot = rboot_get_current_rom();
	u32_t spiff_start;

//	if (slot == 0) {
//#ifdef RBOOT_SPIFFS_0
//		spiff_start=RBOOT_SPIFFS_0 + 0x40200000;
//#else
//		spiff_start=0x40300000;
//#endif
//	} else {
//#ifdef RBOOT_SPIFFS_1
//		spiff_start=RBOOT_SPIFFS_1 + 0x40200000;
//#else
//		//spiffs_mount_manual(0x40500000, SPIFF_SIZE);
//		spiff_start=0x40300000;
//#endif
//	}
	spiff_start=0x40300000;
	debugf("trying to mount spiffs at %x, length %d", spiff_start, SPIFF_SIZE);
	spiffs_mount_manual(spiff_start, SPIFF_SIZE);

#else
	debugf("spiffs disabled");
#endif

}

/**
 * debugging: dump out settings!
 */
void dumpSettings(){
	debugf("dumpSettings() START...");
	String settings=fileGetContent(".settings.conf");

	debugf("settings length=%d bytes",settings.length());
	Serial.print(settings);

	debugf("dumpSettings()done.");
}

/**
 * @brief Global entry point for the application.
 * initializes the basic system and fires up al needed servers
 */
void init() {
	int initHeap = system_get_free_heap_size();

#if LOG_LEVEL>LOG_LEVEL_OFF
	init_logging();
#endif

	LOG_INFO("init_logging() done.");

	IO.init();
	IO.setDiagnosticLed(true);

	Serial.begin(SERIAL_BAUD_RATE);
	Serial.systemDebugOutput(true);

	debugf("heap when init(): %d",initHeap);


	LOG_INFO("==== Startup: Init ====");	// for testing of log system!

	int slot = rboot_get_current_rom();
	const char * buildtime=Version::buildtime().c_str();

	debugf("current rom=%d at %s",slot,buildtime);



	update_check_rboot_config();

	LOG_INFO("update_check_rboot_config() finished");

	mount_spiffs();

	//dumpSettings();//TODO:debugging!

	CommandLine::startDebug();

	LOG_INFO("This is ROM "+Version::version());
	LOG_INFO("Type 'help' and press enter for instructions.");
	LOG_INFO("");
	

	LOGHEAP();
	Serial.println();

	// test Graylog..#
	GELF_add_appender("192.168.1.100",55056); // wirbel testing
	GELF_add_appender("192.168.1.1",12201); // logger

	LOG_INFO("==== with_graylog!==");

	//Serial.setCallback(serialCallBack);

	Serial.commandProcessing(true);			// also tell the commands about serial...

	// Run server on system ready: TODO: when call this?
	LOGHEAP();

	Node::node().init(); // Init node & start execution

	LOGHEAP();
	LOG_INFO("==== Init completed ====");	// for testing of log system!
	LOGHEAP();
}
