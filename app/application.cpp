//////////////////////////////////////////////////
// rBoot sming sample project.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <rboot-smingota.h>

#include <user_interface.h>


#include <AppSettings.h>
#include "update.h"
#include <ctype.h>

#define LED_PIN1 4 // GPIO4
#define LED_PIN2 5 // GPIO5

#define VERSION "0.3"


static bool state = true;

/**
 * avoid errors in eclipse...
 */
#ifndef BUILD_GITREF
#define BUILD_GITREF "##NO GIT REF"
#endif

#ifndef BUILD_TIME
#define BUILD_TIME "##NO BUILD TIME"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "##NO GIT REF"
#endif

#ifndef WIFI_PWD
#define WIFI_PWD "##NO WIFI_PWD"
#endif


/**
 * update & send messages to Serial
 */
static void OtaUpdate() {
	//update_app(Serial);
	update_app();
}

/**
 * update & send messages to Serial
 */
static void OtaUpdateFiles() {
	update_files(Serial);
}


void Switch() {
	update_switch_roms(Serial);
}



void ShowInfo() {
    //Serial.printf("\r\nSDK: v%s\r\n", system_get_sdk_version());
    Serial.printf("Free Heap: %d\r\n", system_get_free_heap_size());
    Serial.printf("CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
    Serial.printf("System Chip ID: 0x%x\r\n", system_get_chip_id());
    Serial.printf("SPI Flash ID: 0x%x\r\n", spi_flash_get_id());
    //Serial.printf("SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
    update_print_config();

}

void serialCallBack(Stream& stream, char arrivedChar, unsigned short availableCharsCount) {
	state=!state;
	digitalWrite(LED_PIN2, state);

	if (arrivedChar == '\n') {
		char str[availableCharsCount];
		for (int i = 0; i < availableCharsCount; i++) {
			str[i] = stream.read();
			if (str[i] == '\r' || str[i] == '\n') {
				str[i] = '\0';
			}
		}
		
		if (!strcmp(str, "connect")) {
			// connect to wifi
			WifiStation.config(WIFI_SSID, WIFI_PWD);
			WifiStation.enable(true);
		} else if (!strcmp(str, "ip")) {
			Serial.printf("ip: %s mac: %s\r\n", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str());
		} else if (!strcmp(str, "ota")) {
			OtaUpdate();
		} else if (!strcmp(str, "otafiles")) {
			OtaUpdateFiles();
		} else if (!strcmp(str, "otafs")) {
			//OtaUpdateFiles();
		} else if (!strcmp(str, "switch")) {
			Switch();
		} else if (!strcmp(str, "restart")) {
			System.restart();
		} else if (!strcmp(str, "ls")) {
			Vector<String> files = fileList();
			Serial.printf("filecount %d\r\n", files.count());
			for (unsigned int i = 0; i < files.count(); i++) {
				Serial.println(files[i]);
			}
		} else if (!strncmp(str, "cat ",3)) {
			Vector<String> files = fileList();
			if (files.count() > 0) {
				Serial.printf("dumping file %s:\r\n", files[0].c_str());
				Serial.println(fileGetContent(files[0]));
			} else {
				Serial.println("Empty spiffs!");
			}
		} else if (!strcmp(str, "info")) {
			ShowInfo();
		} else if (!strcmp(str, "help")) {
			Serial.println();
			Serial.println("available commands:");
			Serial.println("  help - display this message");
			Serial.println("  ip - show current ip address");
			Serial.println("  connect - connect to wifi");
			Serial.println("  restart - restart the esp8266");
			Serial.println("  switch - switch to the other rom and reboot");
			Serial.println("  ota - perform ota update, switch rom and reboot");
			Serial.println("  otafiles - update all files over web from src");
			//Serial.println("  otafs - update files system (spiff_rom.bin) over web");
			Serial.println("  info - show esp8266 info");
			Serial.println("  ls - list files in spiffs");
			Serial.println("  cat <filename>- show first file in spiffs");
			Serial.println();
		} else {
			Serial.println("unknown command");
		}
	}
}


HttpServer server;
FTPServer ftp;

BssList networks;
String network, password;
Timer connectionTimer;

void ICACHE_FLASH_ATTR onIndex(HttpRequest &request, HttpResponse &response)
{
	TemplateFileStream *tmpl = new TemplateFileStream("index.html");
	auto &vars = tmpl->variables();
	response.sendTemplate(tmpl); // will be automatically deleted
}

void ICACHE_FLASH_ATTR onIpConfig(HttpRequest &request, HttpResponse &response)
{
	if (request.getRequestMethod() == RequestMethod::POST)
	{
		AppSettings.dhcp = request.getPostParameter("dhcp") == "1";
		AppSettings.ip = request.getPostParameter("ip");
		AppSettings.netmask = request.getPostParameter("netmask");
		AppSettings.gateway = request.getPostParameter("gateway");
		debugf("Updating IP settings: %d", AppSettings.ip.isNull());
		AppSettings.save();
	}

	TemplateFileStream *tmpl = new TemplateFileStream("settings.html");
	auto &vars = tmpl->variables();

	bool dhcp = WifiStation.isEnabledDHCP();
	vars["dhcpon"] = dhcp ? "checked='checked'" : "";
	vars["dhcpoff"] = !dhcp ? "checked='checked'" : "";

	if (!WifiStation.getIP().isNull())
	{
		vars["ip"] = WifiStation.getIP().toString();
		vars["netmask"] = WifiStation.getNetworkMask().toString();
		vars["gateway"] = WifiStation.getNetworkGateway().toString();
	}
	else
	{
		vars["ip"] = "192.168.1.77";
		vars["netmask"] = "255.255.255.0";
		vars["gateway"] = "192.168.1.1";
	}

	response.sendTemplate(tmpl); // will be automatically deleted
}

/**
 * show system information
 */
void onSystem(HttpRequest &request, HttpResponse &response)
{
	if (request.getRequestMethod() == RequestMethod::POST)
	{
//		AppSettings.dhcp = request.getPostParameter("dhcp") == "1";
//		AppSettings.ip = request.getPostParameter("ip");
//		AppSettings.netmask = request.getPostParameter("netmask");
//		AppSettings.gateway = request.getPostParameter("gateway");
//		debugf("Updating IP settings: %d", AppSettings.ip.isNull());
//		AppSettings.save();
	}

	TemplateFileStream *tmpl = new TemplateFileStream("system.html");
	auto &vars = tmpl->variables();


	vars["buildref"]=BUILD_GITREF;
	vars["buildtime"]=VERSION " " BUILD_TIME;


	String rom(rboot_get_current_rom());
	vars["bootrom"]=rom;

	bool dhcp = WifiStation.isEnabledDHCP();
	vars["dhcpon"] = dhcp ? "checked='checked'" : "";
	vars["dhcpoff"] = !dhcp ? "checked='checked'" : "";

	if (!WifiStation.getIP().isNull())
	{
		vars["ip"] = WifiStation.getIP().toString();
		vars["netmask"] = WifiStation.getNetworkMask().toString();
		vars["gateway"] = WifiStation.getNetworkGateway().toString();
	}
	else
	{
		vars["ip"] = "192.168.1.77";
		vars["netmask"] = "255.255.255.0";
		vars["gateway"] = "192.168.1.1";
	}

	response.sendTemplate(tmpl); // will be automatically deleted
}

void ICACHE_FLASH_ATTR onFile(HttpRequest &request, HttpResponse &response)
{
	String file = request.getPath();
	if (file[0] == '/')
		file = file.substring(1);

	if (file[0] == '.')
		response.forbidden();
	else
	{
		response.setCache(86400, true); // It's important to use cache for better performance.
		response.sendFile(file);
	}
}

void onAjaxNetworkList(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	json["status"] = (bool)true;

	bool connected = WifiStation.isConnected();
	json["connected"] = connected;
	if (connected)
	{
		// Copy full string to JSON buffer memory
		json.addCopy("network", WifiStation.getSSID());
	}

	JsonArray& netlist = json.createNestedArray("available");
	for (int i = 0; i < networks.count(); i++)
	{
		if (networks[i].hidden) continue;
		JsonObject &item = netlist.createNestedObject();
		item.add("id", (int)networks[i].getHashId());
		// Copy full string to JSON buffer memory
		item.addCopy("title", networks[i].ssid);
		item.add("signal", networks[i].rssi);
		item.add("encryption", networks[i].getAuthorizationMethodName());
	}

	response.setAllowCrossDomainOrigin("*");
	response.sendJsonObject(stream);
}

void makeConnection()
{
	WifiStation.enable(true);
	WifiStation.config(network, password);

	AppSettings.ssid = network;
	AppSettings.password = password;
	AppSettings.save();

	network = ""; // task completed
}

void onAjaxConnect(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	String curNet = request.getPostParameter("network");
	String curPass = request.getPostParameter("password");

	bool updating = curNet.length() > 0 && (WifiStation.getSSID() != curNet || WifiStation.getPassword() != curPass);
	bool connectingNow = WifiStation.getConnectionStatus() == eSCS_Connecting || network.length() > 0;

	if (updating && connectingNow)
	{
		debugf("wrong action: %s %s, (updating: %d, connectingNow: %d)", network.c_str(), password.c_str(), updating, connectingNow);
		json["status"] = (bool)false;
		json["connected"] = (bool)false;
	}
	else
	{
		json["status"] = (bool)true;
		if (updating)
		{
			network = curNet;
			password = curPass;
			debugf("CONNECT TO: %s %s", network.c_str(), password.c_str());
			json["connected"] = false;
			connectionTimer.initializeMs(1200, makeConnection).startOnce();
		}
		else
		{
			json["connected"] = WifiStation.isConnected();
			debugf("Network already selected. Current status: %s", WifiStation.getConnectionStatusName());
		}
	}

	if (!updating && !connectingNow && WifiStation.isConnectionFailed())
		json["error"] = WifiStation.getConnectionStatusName();

	response.setAllowCrossDomainOrigin("*");
	response.sendJsonObject(stream);
}

/**
 * just for testing
 */
void ICACHE_FLASH_ATTR onTest(HttpRequest &request, HttpResponse &response)
{
	response.sendString("Hello, World!<br/>");
	response.sendString("files:");


	Vector<String> files=fileList();
	for(int i=0;i<files.size();i++){

		response.sendString(files[i]);
		response.sendString("\r\n");

		//ebugf("file %d=%s",i,files[i]);
	}

}

/**
 * trigger OTA
 */
void ICACHE_FLASH_ATTR onOta(HttpRequest &request, HttpResponse &response)
{
	Serial.println("OTA triggered from Web");

	response.sendString("OTA inited<br/>");

	//update_app(Serial);

	//##ever getting here??
	response.sendString("OTA done.");
}

void ICACHE_FLASH_ATTR startWebServer()
{
	server.listen(80);
	server.addPath("/", onIndex);
	server.addPath("/test", onTest);
	server.addPath("/ota", onOta);
	server.addPath("/ipconfig", onIpConfig);
	server.addPath("/system", onSystem);
	server.addPath("/ajax/get-networks", onAjaxNetworkList);
	server.addPath("/ajax/connect", onAjaxConnect);
	server.setDefaultHandler(onFile);
}

void ICACHE_FLASH_ATTR startFTP()
{
	if (!fileExist("index.html"))
		fileSetContent("index.html", "<h3>Please connect to FTP and upload files from folder 'web/build' (details in code)</h3>");

	// Start FTP server
	ftp.listen(21);
	ftp.addUser("me", "123"); // FTP account
}

// Will be called when system initialization was completed
void ICACHE_FLASH_ATTR startServers()
{
	startFTP();
	startWebServer();
}

void networkScanCompleted(bool succeeded, BssList list)
{
	if (succeeded)
	{
		for (int i = 0; i < list.count(); i++)
			if (!list[i].hidden && list[i].ssid.length() > 0)
				networks.add(list[i]);
	}
	networks.sort([](const BssInfo& a, const BssInfo& b){ return b.rssi - a.rssi; } );
}




void init() {
	pinMode(LED_PIN1, OUTPUT);
	pinMode(LED_PIN2, OUTPUT);

	digitalWrite(LED_PIN1, true);
	digitalWrite(LED_PIN2, state);

	Serial.begin(SERIAL_BAUD_RATE);
	Serial.systemDebugOutput(true);


	//int slot = rboot_get_current_rom();
	update_check_rboot_config();

	//only on spifffs for all slots..
	//always sane spiffs!
	//spiffs_mount_manual(0x40300000, 0x70000);
	//TODO: this is wrong! need to use SPIFF_SIZE from build!
	//TODO: disable watchdog???
	{
		uint32 spiffstart=0x40200000+SPIFF_START;
		Serial.printf("mounting spiff at %x size=%d (%x)",spiffstart,SPIFF_SIZE,SPIFF_SIZE);
		spiffs_mount_manual(spiffstart, SPIFF_SIZE);
	}
	/*
	if (slot == 0) {
		spiffs_mount_manual(0x40300000, 0x70000);
	} else {
		spiffs_mount_manual(0x40500000, 0x70000);
	}
	*/
	// disable access point



	WifiAccessPoint.enable(false);
	
	Serial.println("This is ROM " VERSION);
	Serial.println("Type 'help' and press enter for instructions.");
	Serial.println();
	
	Serial.setCallback(serialCallBack);
	

	/**
	 * Setuip connectivity: AccessPoint for (emergency) Config + station for real work
	 */
	AppSettings.load();

	WifiStation.enable(true);
	if (AppSettings.exist())
	{
		WifiStation.config(AppSettings.ssid, AppSettings.password);
		if (!AppSettings.dhcp && !AppSettings.ip.isNull())
			WifiStation.setIP(AppSettings.ip, AppSettings.netmask, AppSettings.gateway);
	}

	WifiStation.startScan(networkScanCompleted);
	//TODO: AccessPoint always on, or only if not connected to primary net?

	// Start AP for configuration; will open at http://192.168.4.1/
	//WifiAccessPoint.setIP(IPAddress("192.168.5.1"));
	WifiAccessPoint.enable(true);
	WifiAccessPoint.config("Sming Configuration","", AUTH_OPEN);


	// Run WEB server on system ready
	System.onReady(startServers);

}
