/*
 * Network.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#include "user_config.h"

#include <Debug.h>

#include "buildinfo.h"


#include "Node.h"
#include "Network.h"

namespace dobby {

Network::Network():Device("network"){


	// emergency mode: initialize defaults!
	password=WIFI_PWD;
	ssid=WIFI_SSID;
	dhcp=true;
}

Network::~Network() {
	// TODO Auto-generated destructor stub
}



// Will be called when WiFi station was connected to AP
void Network::connectOk()
{
	Serial.println("I'm CONNECTED");

	startmDNS();

	Node::node().networkConnectOk();
}

void Network::enableAccessPoint() {
	// Start AP for configuration; will open at http://192.168.4.1/
	//WifiAccessPoint.setIP(IPAddress("192.168.5.1"));
	WifiAccessPoint.enable(true);
	WifiAccessPoint.config(Node::node().id(),"", AUTH_OPEN);

	// launch services...
	//connectOk();
}

void Network::disableAccessPoint() {
	WifiAccessPoint.enable(false);
}

// Will be called when WiFi station timeout was reached
void Network::connectFail()
{
	Serial.println("I'm NOT CONNECTED. Need help :(");
	Debug.printf("ssid=%s\r\n",ssid.c_str());
	Debug.printf("password=%s\r\n",password.c_str());
	Node::node().networkConnectFailed();
}

/**
 * mDNS using ESP8266 SDK functions
 */

void Network::startmDNS() {
    struct mdns_info *info = (struct mdns_info *)os_zalloc(sizeof(struct mdns_info));
    info->host_name = (char *) Node::node().id().c_str(); // You can replace test with your own host name
    info->ipAddr = WifiStation.getIP();
    info->server_name = (char *) "192.168.1.1";  //TODO:  get that from dhcp???
    info->server_port = 5353;
    info->txt_data[0] = (char *) "version = now";
    espconn_mdns_init(info);
}

/**
 * start etworking; TODO: also launch a network watchdog!
 */
void Network::start() {
	Debug.println("Starting Network...");

	//TODO: right place??
	WifiStation.enable(true);

	WifiStation.config(ssid,password);
	if (!dhcp && !ip.isNull())
		WifiStation.setIP(ip, netmask, gateway);


	//how to tell if I loose connections??
	WifiStation.waitConnection(ConnectionDelegate(&Network::connectOk,this),20,ConnectionDelegate(&Network::connectFail,this));

}

void Network::load(JsonObject& network) {
	ssid = network["ssid"].toString();
	password = network["password"].toString();

	dhcp = network["dhcp"];

	ip = network["ip"].toString();
	netmask = network["netmask"].toString();
	gateway = network["gateway"].toString();
}

void Network::save(JsonObject& network) {
	network["ssid"] = ssid.c_str();
	network["password"] = password.c_str();

	network["dhcp"] = dhcp;

	// Make copy by value for temporary string objects
	network.addCopy("ip", ip.toString());
	network.addCopy("netmask", netmask.toString());
	network.addCopy("gateway", gateway.toString());
}

} /* namespace dobby */


