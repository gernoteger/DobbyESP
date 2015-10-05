//////////////////////////////////////////////////
// rBoot sming sample project.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include <user_interface.h>
#include <Debug.h>
#include <ctype.h>

#include "buildinfo.h"
#include "Version.h"

#include "update.h"
#include "IOHandler.h"
#include "CommandLine.h"

#include "Node.h"



using namespace dobby;

static bool state = true;





BssList networks;
String network, password;


void mount_spiffs(){

	int slot = rboot_get_current_rom();

#ifndef DISABLE_SPIFFS
	if (slot == 0) {
#ifdef RBOOT_SPIFFS_0
		debugf("trying to mount spiffs at %x, length %d", RBOOT_SPIFFS_0 + 0x40200000, SPIFF_SIZE);
		spiffs_mount_manual(RBOOT_SPIFFS_0 + 0x40200000, SPIFF_SIZE);
#else
		debugf("trying to mount spiffs at %x, length %d", 0x40300000, SPIFF_SIZE);
		spiffs_mount_manual(0x40300000, SPIFF_SIZE);
#endif
	} else {
#ifdef RBOOT_SPIFFS_1
		debugf("trying to mount spiffs at %x, length %d", RBOOT_SPIFFS_1 + 0x40200000, SPIFF_SIZE);
		spiffs_mount_manual(RBOOT_SPIFFS_1 + 0x40200000, SPIFF_SIZE);
#else
		debugf("trying to mount spiffs at %x, length %d", 0x40500000, SPIFF_SIZE);
		spiffs_mount_manual(0x40500000, SPIFF_SIZE);
#endif
	}
#else
	debugf("spiffs disabled");
#endif

}


/**
 * @brief Global entry point for the application.
 * initializes the basic system and fires up al needed servers
 */
void init() {
	IO.init();
	IO.setDiagnosticLed(true);

	Serial.begin(SERIAL_BAUD_RATE);
	Serial.systemDebugOutput(true);



	//int slot = rboot_get_current_rom();
	update_check_rboot_config();

	mount_spiffs();

	CommandLine::startDebug();
	

	Serial.println("This is ROM "+Version::version());
	Serial.println("Type 'help' and press enter for instructions.");
	Serial.println();
	
	//Serial.setCallback(serialCallBack);

	Serial.commandProcessing(true);			// also tell the commands about serial...

	// Run server on system ready: TODO: when call this?

	Node::node().init(); // Init node & start execution
}
