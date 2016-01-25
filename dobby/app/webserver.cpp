/**
 * @file webserver.cpp
 * @brief Webserver for maintainance,.
 *
 *
 * webserver.cpp
 *
 *  Created on: 26.09.2015
 *      Author: gernot
 */

//#define USE_WEBSERVER
#ifdef USE_WEBSERVER

#include <user_config.h>
#include <SmingCore/SmingCore.h>


#include "webserver.h"
#include "Version.h"


extern String network, password;
extern BssList networks;

namespace dobby {

Timer connectionTimer;

HttpServer server;


void makeConnection()
{
	WifiStation.enable(true);
	WifiStation.config(network, password);

//	AppSettings.ssid = network;
//	AppSettings.password = password;
//	AppSettings.save();
//
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
//		AppSettings.dhcp = request.getPostParameter("dhcp") == "1";
//		AppSettings.ip = request.getPostParameter("ip");
//		AppSettings.netmask = request.getPostParameter("netmask");
//		AppSettings.gateway = request.getPostParameter("gateway");
//		debugf("Updating IP settings: %d", AppSettings.ip.isNull());
//		AppSettings.save();
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


	vars["buildref"]=Version::gitref();
	vars["buildtime"]=Version::buildinfo();


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

void startWebServer()
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

}  // namespace dobby

#endif //startWebServer

