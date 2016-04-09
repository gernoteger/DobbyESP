/*
 * Node.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#include "user_config.h"

#undef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG

#include <SmingCore/SmingCore.h>

//#include "webserver.h"
#include "MessageConnection.h"



#include "Devices/Device.h"
#include "Devices/Switch.h"
#include "Devices/PushButton.h"
#include "Devices/Thermostat.h"

#include "logging.h"
#include "Logger.h"

#include "GELFAppender.h"

#include "Version.h"
#include "Node.h"


#define APP_SETTINGS_FILE ".settings.conf" // leading point for security reasons :)

namespace dobby {

Node::Node() {
	LOG_DEBUG("Node::Node()");

	passphrase="123";	// factory default if not configured
	setId("dobby-"+String(system_get_chip_id(),16));
}


Node::~Node() {
	// TODO Auto-generated destructor stub
}


/**
 * the global one..
 */
Node* _node=NULL;

void Node::init() {
	LOGHEAP();

	load();

	// test Graylog..#
	GELF_add_appender("192.168.1.100",55056); // wirbel testing
	GELF_add_appender("192.168.1.1",12201); // logger

	add(_updater);

	net.start();
	for(int i=0;i<devices.count();i++){
		devices.valueAt(i)->start();
	}
	cmd.registerCommands();
	cmd.startDebug();
	mqtt.start(); // will call subscribeDevices();
}


void Node::subscribeDevices() {
	LOG_DEBUG("subscribing core devices...");
	//publish core devices
	mqtt.subscribe(net);
	mqtt.subscribe(_updater);
	//mqtt.subscribe(*this);
	//TODO: others...

	LOGHEAP();
	LOG_DEBUG("subscribing devices...");

	//all the devices..
	for(int i=0;i<devices.count();i++){
		Device* device=devices.valueAt(i);
		LOG_DEBUG("subcribing device "+device->id());

		mqtt.subscribe(*device);
		//register with systems
		device->sendRegistryMessage();
	}

	LOGHEAP();
	LOG_DEBUG("subscribeDevices done.");
}

#define DOBBY_PRODUCT_BATH1
//#define DOBBY_PRODUCT_TESTBED

void Node::initDevices(){
	LOG_DEBUG("initializing devices...");

#ifdef DOBBY_PRODUCT_BATH1

	/*
	  "devices": [
		{"id":"floor","type": "thermostat",
			"heater_gpio": 12, "sensor": "ntc", "hyteresis": 60, "controlIntervalMillis":2000
		},
		{"id":"light_button","type": "pushbutton", "gpio": 13,
			"toggle": [
				{ "id": "light"},{ "id": "fan"}
			]
		},
		{"id":"fan_button","type": "pushbutton", "gpio": 14},
	 	{"id":"light","type": "switch", "gpio": 4},
	 	{"id":"fan","type": "switch", "gpio": 5}
*/

//	String id=device["id"].asString();
//
//	LOG_DEBUG("-----"+D::typeName()+": "+id);
//	Device * dev=new D(id);
//	//TODO:OutOfMemory handling here!
//	dev->load(device);
//	this->devices[id]=dev;

	add(new PushButton("fan_button",14));

	PushButton * light_button=new PushButton("light_button",13);
	//TODO: add actions to light_button
	add(light_button);

	add(new Switch("light",4));
	add(new Switch("fan",5));

	add(new Thermostat("floor",12,60.0,60000L));

;
#endif

	LOG_DEBUG("device initialized.");
}

void Node::add(Device& device){
	devices[device.id()]=&device;
}

void Node::add(Device * device){
	devices[device->id()]=device;
}

template<class D> void Node::loadDevice(JsonObject& device){
	String id=device["id"].asString();

	LOG_DEBUG("-----"+D::typeName()+": "+id);
	Device * dev=new D(id);
	//TODO:OutOfMemory handling here!
	dev->load(device);
	this->devices[id]=dev;
}

/**
 * load from file or create defaults..
 * TODO: not very efficient on RAM!!
 */
void Node::load()
{
	Logger::logheap("Node::load 0");

	DynamicJsonBuffer jsonBuffer;

	LOG_DEBUG("Node::load()");

	if (fileExist(APP_SETTINGS_FILE))
	{
		LOG_DEBUG("Node::load(): found file");

		LOGHEAP();

		int size = fileGetSize(APP_SETTINGS_FILE);
		char* jsonString = new char[size + 1];
		fileGetContent(APP_SETTINGS_FILE, jsonString, size + 1);

		LOG_DEBUGF("%s",jsonString);

		LOGHEAP();

		JsonObject& root = jsonBuffer.parseObject(jsonString);
		if(root==JsonObject::invalid()){
			LOG_DEBUG("Json Parsing failed.");
		}else{
			LOGHEAP();

			setId(root["id"].toString());

			LOG_DEBUG("Node::load(): loading net");
			LOGHEAP();

			net.loadFromParent(root);
			LOG_DEBUG("Node::load(): loading mqtt");
			LOGHEAP();

			mqtt.loadFromParent(root);

			LOG_DEBUG("Node::load(): loaded mqtt");
			LOGHEAP();


			//TODO: move into mqtt
			//mqtt.configure("192.168.1.1",1883);


			// load devices
			initDevices();

			/*
			JsonArray& devices=root["devices"];

			Debug.printf("found %d devices.\r\n",devices.size());

			for(JsonArray::iterator it=devices.begin();it!=devices.end();++it){
				JsonObject& device=*it;

				LOG_DEBUG("initializing device "+device.toJsonString());
				String type=device["type"].asString();
				if(type==Switch::typeName()){
					loadDevice<Switch>(device);
				}else if(type==PushButton::typeName()){
					LOG_DEBUG("Pushbutton:");
					loadDevice<PushButton>(device);
				}else if(type==Thermostat::typeName()){
					LOG_DEBUG("Thermostat:");
					loadDevice<Thermostat>(device);
				}else{
					LOG_DEBUG("##Error: unknown device type '"+type+"'");
				}
			}
			 */
			LOGHEAP();

			LOG_DEBUG("Node::load() done.");
		}

		delete[] jsonString;
	}else{
		// save default settings!
		save();

	}
}

void Node::save() //TODO: this crashes???
{
	LOG_INFO("saving...");

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	root["id"]=id().c_str();

	LOG_DEBUG("saving network");
	net.saveToParent(root);

	LOG_DEBUG("saving mqtt");
	if(mqtt.isConfigured()){//TODO: check if this is clever...
		mqtt.saveToParent(root);
	}

	root.printTo(Debug);
	LOG_DEBUG("saving to file");

	//TODO: add direct file stream writing
	fileSetContent(APP_SETTINGS_FILE, root.toJsonString());

	LOG_INFO("save done.");
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


Updater& Node::updater() {
	return _updater;
}

/*
 * device or NULL
 */
MessageEndpoint * Node::device(String id) {
	int i=devices.indexOf(id);
	if(i<0) return NULL;
	return devices.valueAt(i);
}


///@name Application Logic
///@{

void Node::networkConnectOk() {
	Logger::logheap("Node::networkConnectOk start");

	net.disableAccessPoint();

	startTelnetServer();
	//messageHandler.start();

	Logger::logheap("Node::networkConnectOk 1");

	startFTP();
	//dobby::startWebServer();

	Logger::logheap("Node::networkConnectOk 2");


	mqtt.start();
	// check mqtt
	if(!mqtt.isConfigured()){
		// assume host is identical to gateway
		IPAddress gw=WifiStation.getNetworkGateway();
		if(!gw.isNull()){
			mqtt.configure(gw.toString(),1883);
			mqtt.start();
			//TODO: is connect synchronous?
			if(mqtt.isConnected()){ // successful, save default? config!
				save();
			}
		}
	}
	Logger::logheap("Node::networkConnectOk done.");
}

void Node::networkConnectFailed() {
	// .. some you code for device configuration ..
	LOG_INFO("Node::networkConnectFailed(): enabling access point");
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


//void Node::statusQueryReceived() {
//	LOG_INFO("AppController::statusQueryReceived(: not yet implemented"); //TODO: implement
//}
//
//
//
//void Node::updateButtonPressed() {
//	LOG_INFO("AppController::updateButtonPressed(: not yet implemented"); //TODO: implement
//}

String Node::info() {
//Serial.printf("\r\nSDK: v%s\r\n", system_get_sdk_version());
	String nl="\r\n";
	String info="Node: "+Node::node().id()+nl+
			"Version: "+Version::version()+nl+
			"git ref: "+Version::gitref()+nl+
			"built at: "+Version::buildtime()+nl+
			"SDK: "+system_get_sdk_version()+nl+
			"Free Heap: "+system_get_free_heap_size()+nl+
			"CPU Frequency: "+system_get_cpu_freq()+nl+
			"System Chip ID: "+system_get_chip_id()+nl+
			"SPI Flash ID: "+String(spi_flash_get_id(),16)+nl+//TODO: hex!!
			"IP: "+WifiStation.getIP().toString()+" MAC: "+WifiStation.getMAC();

}

///*
// * test with
// */
//void Node::otaCommandReceived() {
//	LOG_INFO("AppController::otaCommandReceived(: not yet implemented"); //TODO: implement
//}

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

