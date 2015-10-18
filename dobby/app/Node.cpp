/*
 * Node.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#include <SmingCore/SmingCore.h>

//#include "webserver.h"
#include "MessageHandler.h"


#include "Node.h"
#include "Devices/Switch.h"
#include "Devices/PushButton.h"
#include "Devices/Thermostat.h"


#define APP_SETTINGS_FILE ".settings.conf" // leading point for security reasons :)

namespace dobby {

Node::Node() {
	Debug.println("Node::Node()");

	passphrase="123";	// factory default if not configured
	_id="dobby-"+String(system_get_chip_id(),16);
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
	cmd.registerCommands();
	cmd.startDebug();

	mqtt.start();

}


/**
 * load from file or create defaults..
 */
void Node::load()
{
	DynamicJsonBuffer jsonBuffer;

	Debug.println("Node::load()");

	if (fileExist(APP_SETTINGS_FILE))
	{
		Debug.println("Node::load(): found file");

		int size = fileGetSize(APP_SETTINGS_FILE);
		char* jsonString = new char[size + 1];
		fileGetContent(APP_SETTINGS_FILE, jsonString, size + 1);


		JsonObject& root = jsonBuffer.parseObject(jsonString);
		if(root==JsonObject::invalid()){
			Debug.println("Json Parsing failed.");
		}else{

			_id=root["id"].toString();

			Debug.println("Node::load(): loading net");

			net.loadFromParent(root);
			Debug.println("Node::load(): loading mqtt");
			mqtt.loadFromParent(root);

			Debug.println("Node::load(): loaded mqtt");

			//TODO: move into mqtt
			mqtt.configure("192.168.1.1",1883);


			// load devices
			JsonArray& devices=root["devices"];
			for(int i=0;i<devices.size();i++){
				JsonObject& device=devices[i];
				String type=device["type"].asString();
				if(type==Switch::typeName()){
					Debug.println("Switch:");
				}else if(type==PushButton::typeName()){
					Debug.println("Pushbutton:");

				}else if(type==Thermostat::typeName()){
					Debug.println("Thermostat:");

				}
			}


			Debug.println("Node::load() done.");
		}

		delete[] jsonString;
	}else{
		// save default settings!
		save();

	}
}

void Node::save()
	{
		Debug.println("saving...");

		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();

		root["id"]=_id.c_str();

		Debug.println("saving network");
		net.saveToParent(root);

		Debug.println("saving mqtt");
		if(mqtt.isConfigured()){//TODO: check if this is clever...
			mqtt.saveToParent(root);
		}

		root.printTo(Debug);
		Debug.println("saving to file");

		//TODO: add direct file stream writing
		fileSetContent(APP_SETTINGS_FILE, root.toJsonString());

		Debug.println("save done.");
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


///@name Application Logic
///@{

void Node::networkConnectOk() {
	net.disableAccessPoint();

	startTelnetServer();
	//messageHandler.start();

	startFTP();
	//dobby::startWebServer();

	// check mqtt
	if(!mqtt.isConfigured()){
		// assume host is identical to gateway
		IPAddress gw=WifiStation.getNetworkGateway();
		if(!gw.isNull()){
			mqtt.configure(gw.toString(),1883);
			mqtt.start();
			// is connect synchronous?
			if(mqtt.isConnected()){ // successful, save config!
				save();
			}
		}
	}
}

void Node::networkConnectFailed() {
	// .. some you code for device configuration ..
	Debug.println("Node::networkConnectFailed(): enabling access point");
	net.enableAccessPoint();
}

void Node::networkConnectionLost() {
}

void Node::startFTP()
{
//	if (!fileExist("index.html"))
//		fileSetContent("index.html", "<h3>Please connect to FTP and upload files from folder 'web/build' (details in code)</h3>");

	// Start FTP server
	ftp->listen(21);
	ftp->addUser("admin", passphrase); // FTP account
}


void Node::statusQueryReceived() {
	Debug.println("AppController::statusQueryReceived(: not yet implemented"); //TODO: implement
}

/**
 * user interface button pressed
 */

void Node::userButtonPressed() {
	//just implemet a debugging response: toggle diagnostic led, and send som status message
	Debug.println("AppController::statusQueryReceived(: not yet implemented"); //TODO: implement

	// toggle Led
	IO.setDiagnosticLed(!IO.getDiagnosticLed());

	// testing: access point on/off
	if(IO.getDiagnosticLed()){
		net.enableAccessPoint();
	}else{
		net.disableAccessPoint();
	}
	// send message; maybe add some info here...(status in JSon?)
	mqtt.sendUserButtonMessage();

}


void Node::updateButtonPressed() {
	Debug.println("AppController::updateButtonPressed(: not yet implemented"); //TODO: implement
}


void Node::otaCommandReceived() {
	Debug.println("AppController::otaCommandReceived(: not yet implemented"); //TODO: implement
}

/**
 * start telnet server for remote operation.
 * The TelnetServer handles connections, and hands over everything to the global commandHandler.
 * The commands are registered elsewhere with the commandHandler.
 */
void Node::startTelnetServer()
{
	telnet.listen(23);
	Serial.println("\r\n=== Telnet SERVER Port 23 STARTED ===");
	Serial.println(WifiStation.getIP());
	Serial.println("==============================\r\n");
}

/**
 * gracefully shut down telnet server
 */
void Node::stopTelnetServer()
{
	telnet.close();
}

///@}

} /* namespace dobby */


