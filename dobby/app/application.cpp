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

#include <AppSettings.h>

#include "buildinfo.h"
#include "Version.h"

#include "update.h"
#include "telnet.h"
#include "commands.h"
#include "webserver.h"
#include "MessageHandler.h"
#include "IOHandler.h"
#include "networking.h"

#include "Node.h"

#define LED_PIN1 4 // GPIO4
#define LED_PIN2 5 // GPIO5


using namespace dobby;

static bool state = true;



//void networkScanCompleted(bool succeeded, BssList list);
void checkConnections();


FTPServer ftp;


BssList networks;
String network, password;



void startFTP()
{
	if (!fileExist("index.html"))
		fileSetContent("index.html", "<h3>Please connect to FTP and upload files from folder 'web/build' (details in code)</h3>");

	// Start FTP server
	ftp.listen(21);
	ftp.addUser("me", "123"); // FTP account
}



///**
// * call back for network scan TODO: what for??
// * @param succeeded
// * @param list
// */
//void networkScanCompleted(bool succeeded, BssList list)
//{
//	debugf("networkScanCompleted");
//	if (succeeded)
//	{
//		for (int i = 0; i < list.count(); i++)
//			if (!list[i].hidden && list[i].ssid.length() > 0)
//				networks.add(list[i]);
//	}
//	networks.sort([](const BssInfo& a, const BssInfo& b){ return b.rssi - a.rssi; } );
//
//	checkConnections();
//}


/**
 * to be called periodically, and when I expect them to be ruined (e.g.network scan!)
 */
void checkConnections(){
	messageHandler.check();
}

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

	startDebug();

	//only on spifffs for all slots..
	//always sane spiffs!
	//spiffs_mount_manual(0x40300000, 0x70000);
	//TODO: this is wrong! need to use SPIFF_SIZE from build!
	//TODO: disable watchdog???
//	{
//		uint32 spiffstart=0x40200000+SPIFF_START;
//		Serial.printf("mounting spiff at %x size=%d (%x)",spiffstart,SPIFF_SIZE,SPIFF_SIZE);
//		spiffs_mount_manual(spiffstart, SPIFF_SIZE);
//	}
	/*
	if (slot == 0) {
		spiffs_mount_manual(0x40300000, 0x70000);
	} else {
		spiffs_mount_manual(0x40500000, 0x70000);
	}
	*/
	// disable access point



	WifiAccessPoint.enable(false);
	

	Serial.println("This is ROM "+dobby::Version::version());
	Serial.println("Type 'help' and press enter for instructions.");
	Serial.println();
	
	//Serial.setCallback(serialCallBack);
	Serial.commandProcessing(true);			// also tell the commands about serial...
	registerCommands();

	// Run server on system ready: TODO: when call this?
	//System.onReady(startServers);

	/**
	 * Setup connectivity: AccessPoint for (emergency) Config + station for real work
	 */
	AppSettings.load(); //requires SPIFFS

	// conigure early; can't be modified w/o leaks!
	messageHandler.configure("192.168.1.1",1883);

	//nw_init();

	Node::node().init(); // Init node & start execution
}