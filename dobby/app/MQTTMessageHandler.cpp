/**
 * @file messageHandler.cpp
 *
 * @brief Handler Messages
 *
 *  Created on: 26.09.2015
 *      Author: gernot
 */

#include <user_config.h>

#include <SmingCore/SmingCore.h>

#include <Delegate.h>
#include "queue.h"

#include "user_interface.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "proto.h"

//using tuanp's version
#include "mqtt.h"

#include "Debug.h"
#include "Node.h"
#include "Device.h"

#include "MQTTMessageHandler.h"



namespace dobby {

MQTTMessageHandler::MQTTMessageHandler(){
	Debug.println("MessageHandler::MessageHandler()");
}

MQTTMessageHandler::~MQTTMessageHandler(){

}

typedef Delegate<void(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)> MQTTDataCallbackDelegate;
typedef Delegate<void(uint32_t *arg)> MQTTCallbackDelegate;


/**
 * just for compile tests
 */
void MQTTMessageHandler::init1(){
	mqttClient.user_data=this; // set to me
	MQTT_InitConnection(&mqttClient, (uint8_t* )server.c_str(), port, 0); //DEFAULT_SECURITY=0
    //MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

    //MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);

	MQTT_InitClient(&mqttClient,  (uint8_t* )Node::node().id().c_str(), (uint8_t* )user.c_str(),(uint8_t* )pwd.c_str(), 120, 1);
    //MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);

    //MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);


 //   MQTT_OnConnected(&mqttClient, MQTTCallbackDelegate(&MQTTMessageHandler::mqttConnectedCb,this));
	MQTT_OnConnected(&mqttClient,staticOnConnected);

//    MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
//    MQTT_OnPublished(&mqttClient, mqttPublishedCb);
//    MQTT_OnData(&mqttClient, mqttDataCb);
}

void MQTTMessageHandler::staticOnConnected(uint32_t *args){
	MQTT_Client* client = (MQTT_Client*)args;
	MQTTMessageHandler * handler=(MQTTMessageHandler*) client->user_data;
	handler->mqttConnectedCb();
}

void MQTTMessageHandler::staticDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len){
	MQTT_Client* client = (MQTT_Client*)args;
	MQTTMessageHandler * handler=(MQTTMessageHandler*) client->user_data;
	handler->mqttConnectedCb();
}

void  MQTTMessageHandler::mqttConnectedCb(){

}

void  MQTTMessageHandler::mqttDataCb(const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{

}

/**
 * start message handler; need Networking infrastructure to be set up
 */
void MQTTMessageHandler::start() {
	Debug.println("MessageHandler::start()");

	if(mqtt!=NULL){
		String name=Node::node().id();

		if(mqtt->connect(name)){ //TODO: bad API:
			Node::node().subscribeDevices();
			Debug.println("================= ");
			Debug.printf("connected mqtt with client id '%s'\r\n",name.c_str());
		}else{
			Debug.println("######## Error connecting to MQTT Client!");
		}
	}else {
		Debug.println("######## Error opening MQTT Client!");
	}
}

void MQTTMessageHandler::stop() {
	Debug.println("MessageHandler::stop() not implemented");
}

void MQTTMessageHandler::sendTestMessage1() {
	Debug.println("sendTestMessage1");
	publish("main/dobby", "Hello friends, from Internet of things :)"); // or publishWithQoS
}

void MQTTMessageHandler::sendUserButtonMessage() {
	Debug.println("sendUserButtonMessage");
	publish("main/dobby/userbutton", "pressed"); // or publishWithQoS
}

void MQTTMessageHandler::sendHeaterStatusMessage(bool isOn) {
	publish("main/dobby/heater",isOn?"1":"0",true); // retained or publishWithQoS
}
/**
 * print status message
 * @param out
 */
void MQTTMessageHandler::printStatus(Print* out) {
	String status;
	if(!mqtt){
		out->println("MessageHandler notg yet  initialized!");
		return;
	}
	switch(mqtt->getConnectionState()){
		case eTCS_Ready: status="eTCS_Ready";break;
		case eTCS_Connecting: status="eTCS_Connecting";break;
		case eTCS_Connected: status="eTCS_Connected";break;
		case eTCS_Successful: status="eTCS_Successful";break;
		case eTCS_Failed: status="eTCS_Failed";break;
	}
	out->printf("MQTT ConnectionState = %s\r\n",status.c_str());
}

/**
 * configure connection params for the message handler; doesn't need to be connected then!
 * @param serverHost
 * @param serverPort
 */
void MQTTMessageHandler::configure(String serverHost, int serverPort) {
	Debug.printf("MQTTMessageHandler::configure(%s,%d)\r\n",serverHost.c_str(),serverPort);
	//TODO: can't disconnect all if reconfigured
	if(mqtt){
		debugf("mqtt client already configured!");
		return;
	}
	this->server=serverHost;
	this->port=serverPort;

	mqtt=new MqttClient(serverHost, serverPort, MqttStringSubscriptionCallback(&MQTTMessageHandler::onMessageReceived,this));
}


void MQTTMessageHandler::onMessageReceived(String topic, String message)
{
	//TODO. find hanlers for that

	Debug.print(topic);
	Debug.print(":\r\n\t"); // Prettify alignment for printing
	Debug.println(message);

	if(topic=="main/commands/led/1"){
		Node::node().diagnosticLedCommandReceived(toOnOff(message));
	}

}

/**
 * check connections & revive if needed
 *
 */
void MQTTMessageHandler::check() {
	if (!isConnected()){
		start(); // Auto reconnect
	}
}

bool MQTTMessageHandler::isConnected() {
	if(mqtt==NULL) return false;
	return mqtt->getConnectionState()==eTCS_Connected;
}

bool MQTTMessageHandler::publish(String topic, String message, bool retained) {
	if(isConnected()) return mqtt->publish(topic,message,retained);
	return false;
}

bool MQTTMessageHandler::publishWithQoS(String topic, String message, int QoS,
		bool retained) {
	if(isConnected()) return mqtt->publishWithQoS(topic,message,QoS,retained);
	return false;
}

bool MQTTMessageHandler::subscribe(Device& device) {
	// just wildcard them
	String topic=deviceTopicPrefix(device)+"/#";
	Debug.printf("subcribing topic '%s' for device '%s'\r\n",topic.c_str(),device.id().c_str());
	if(isConnected()){
		mqtt->subscribe(topic);
	}else{
		Debug.println("couldn't subscribe");
	}
}

String MQTTMessageHandler::deviceTopicPrefix(Device& device) {
	return Node::node().id()+"/"+device.id();
}

void MQTTMessageHandler::load(JsonObject& object) {
	server=object["server"].asString();
	port=object["port"];
	//TODO: safety checks go here...
	if(server!=NULL && port!=0){
		configure(server,port);
	}
}

void MQTTMessageHandler::save(JsonObject& object) {
	if(isConfigured()){
		Debug.printf("saving mqtt: server='%s' port=%d\r\n",server.c_str(),port);
		object.addCopy("server", server);
		object.add("port").set(port);
	}
}

}  // namespace dobby


