/*
 * Node.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#include <SmingCore/SmingCore.h>

//#include "AppSettings.h"
#include "telnet.h"
//#include "webserver.h"
#include "MessageHandler.h"


#include "Node.h"


#define APP_SETTINGS_FILE ".settings.conf" // leading point for security reasons :)

namespace dobby {

Node::Node() {
	passphrase="123";	// factory default if not configured
}


Node::~Node() {
	// TODO Auto-generated destructor stub
}


/**
 * the global one..
 */
Node* _node=NULL;

void Node::init() {
	load();
	net.start();
}


void Node::load()
{
	DynamicJsonBuffer jsonBuffer;
	if (fileExist(APP_SETTINGS_FILE))
	{
		int size = fileGetSize(APP_SETTINGS_FILE);
		char* jsonString = new char[size + 1];
		fileGetContent(APP_SETTINGS_FILE, jsonString, size + 1);
		JsonObject& root = jsonBuffer.parseObject(jsonString);

		_id=root["id"].toString();

		JsonObject& network = root[net.typeName()];
		net.config(network);

		delete[] jsonString;
	}else{
	}
	if(!_id) _id="dobby-"+String(system_get_chip_id(),16);

}

void Node::save()
	{
//		DynamicJsonBuffer jsonBuffer;
//		JsonObject& root = jsonBuffer.createObject();
//
//		JsonObject& network = jsonBuffer.createObject();
//		root["network"] = network;
//		network["ssid"] = ssid.c_str();
//		network["password"] = password.c_str();
//
//		network["dhcp"] = dhcp;
//
//		// Make copy by value for temporary string objects
//		network.addCopy("ip", ip.toString());
//		network.addCopy("netmask", netmask.toString());
//		network.addCopy("gateway", gateway.toString());
//
//		//TODO: add direct file stream writing
//		fileSetContent(APP_SETTINGS_FILE, root.toJsonString());
	}


String Node::id() {
	return _id;
}

Node& Node::node() {
	if(_node==NULL){
		//setup the node
		//TODO: read from settings file! will contain no devices!
		_node=new Node();
		if(_node==NULL){
			//this must NEVER happen:
			debugf("###ERROR: PANIC! couldn't create node! Restarting...");
			delay(1000); //wait 1000ms..
			System.restart();
		}
	}
	return * _node;
}

Vector<Device&>& Node::devices() {
}


///@name Application Logic
///@{

void Node::networkConnectOk() {
	net.disableAccessPoint();

	startTelnetServer();
	//messageHandler.start();

	startFTP();
	//dobby::startWebServer();
}

void Node::networkConnectFailed() {
	// .. some you code for device configuration ..
	net.enableAccessPoint();
}

void Node::networkConnectionLost() {
}

void Node::startFTP()
{
//	if (!fileExist("index.html"))
//		fileSetContent("index.html", "<h3>Please connect to FTP and upload files from folder 'web/build' (details in code)</h3>");

	// Start FTP server
	ftp.listen(21);
	ftp.addUser("admin", passphrase); // FTP account
}

///@}

} /* namespace dobby */


