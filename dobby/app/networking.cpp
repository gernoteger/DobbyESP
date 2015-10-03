/*
 * networking.cpp
 *
 *  Created on: 02.10.2015
 *      Author: gernot
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "telnet.h"
#include "webserver.h"
#include "MessageHandler.h"
#include "AppSettings.h"

#include "networking.h"

extern void startFTP();

// Will be called when system initialization was completed
void startServers()
{
	debugf("startServers");

	// scanning will reset the connections!!
	//WifiStation.startScan(networkScanCompleted);

	startTelnetServer();
	messageHandler.start();

	startFTP();
	startWebServer();
}

// Will be called when WiFi station was connected to AP
void connectOk()
{
	Serial.println("I'm CONNECTED");

	nw_disableAccessPoint();
	// Run MQTT client
	startServers();

	// Start timed actions..
	//procTimer.initializeMs(20 * 1000, publishMessage).start(); // every 20 seconds
}

// Will be called when WiFi station timeout was reached
void connectFail()
{
	Serial.println("I'm NOT CONNECTED. Need help :(");

	// .. some you code for device configuration ..
	nw_enableAccessPoint();
}

void nw_init(){
	WifiStation.enable(true);
	if (AppSettings.exist())
	{
		WifiStation.config(AppSettings.ssid, AppSettings.password);
		if (!AppSettings.dhcp && !AppSettings.ip.isNull())
			WifiStation.setIP(AppSettings.ip, AppSettings.netmask, AppSettings.gateway);
	}

	//how to tell if I loose connections??
	WifiStation.waitConnection(connectOk,20,connectFail);
}

/**
 * enable access point for failsafe function
 */
void nw_enableAccessPoint(){
	// Start AP for configuration; will open at http://192.168.4.1/
	//WifiAccessPoint.setIP(IPAddress("192.168.5.1"));
	WifiAccessPoint.enable(true);
	WifiAccessPoint.config(AppSettings.nodeId(),"", AUTH_OPEN);
}
void nw_disableAccessPoint(){
	WifiAccessPoint.enable(false);
}
