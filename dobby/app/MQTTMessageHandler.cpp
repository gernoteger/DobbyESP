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
#include "Logger.h"

#include "MQTTMessageHandler.h"



//namespace dobby {

MQTTMessageHandler::MQTTMessageHandler(){
	Debug.println("MessageHandler::MessageHandler()");
	mqttClient.user_data=NULL;
}

MQTTMessageHandler::~MQTTMessageHandler(){

}

//typedef Delegate<void(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)> MQTTDataCallbackDelegate;
//typedef Delegate<void(uint32_t *arg)> MQTTCallbackDelegate;


/**
 * configure with all needed parameters; adds sensible defaults so at least it wouldn't crash!
 */
void MQTTMessageHandler::configure(String host,unsigned int port,String user,String password,uint32_t keepAliveTimeSeconds, uint8_t cleanSession){
	Logger::logheap("MQTTMessageHandler::configure start");
	if(isConfigured()){
		Debug.println("already configured");
		return;//TODO: do it better??
	}
	configured=true;

	mqttClient.user_data=this; // set to me
	Debug.printf("user_data=%x\r\n",mqttClient.user_data);

	MQTT_InitConnection(&mqttClient, (uint8_t* )host.c_str(), port, 0); //DEFAULT_SECURITY=0
    //MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

    //MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);

	Logger::logheap("MQTTMessageHandler::configure 1");

	MQTT_InitClient(&mqttClient,  (uint8_t* )Node::node().id().c_str(), (uint8_t* )user.c_str(),(uint8_t* )password.c_str(),keepAliveTimeSeconds,cleanSession);
    //MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);

	Logger::logheap("MQTTMessageHandler::configure 2");

	//TODO: create some default, so it won't crash; allow overriding!
	//What's  the encoding? stick to utf-8/ASCII
	const char * lwtTopic="/lwt";
	const char * lwtMessage="offline";

    MQTT_InitLWT(&mqttClient, (uint8_t* )lwtTopic, (uint8_t* )lwtMessage, 0, 0);

	//   MQTT_OnConnected(&mqttClient, MQTTCallbackDelegate(&MQTTMessageHandler::mqttConnectedCb,this));
	// setup callbacks
	MQTT_OnConnected(&mqttClient,staticOnConnected);
	MQTT_OnDisconnected(&mqttClient, staticOnDisconnected);
	MQTT_OnPublished(&mqttClient, staticOnPublished);
	MQTT_OnData(&mqttClient, staticDataCb);

	Logger::logheap("MQTTMessageHandler::configure done");
}

void MQTTMessageHandler::staticOnConnected(uint32_t *args){
	MQTT_Client* client = (MQTT_Client*)args;
	MQTTMessageHandler * handler=(MQTTMessageHandler*) client->user_data;
	handler->onConnected();
}
void MQTTMessageHandler::staticOnDisconnected(uint32_t *args){
	MQTT_Client* client = (MQTT_Client*)args;
	MQTTMessageHandler * handler=(MQTTMessageHandler*) client->user_data;
	handler->onDisconnected();
}
void MQTTMessageHandler::staticOnPublished(uint32_t *args){
	MQTT_Client* client = (MQTT_Client*)args;
	MQTTMessageHandler * handler=(MQTTMessageHandler*) client->user_data;
	handler->onPublished();
}

void MQTTMessageHandler::staticDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len){
	MQTT_Client* client = (MQTT_Client*)args;
	MQTTMessageHandler * handler=(MQTTMessageHandler*) client->user_data;

	String topic_str(topic,topic_len);
	String data_str(data,data_len);

	handler->onData(topic_str,data_str);
}



void  MQTTMessageHandler::onConnected(){
	Debug.println("MQTT connected");
	Node::node().subscribeDevices();
}

void  MQTTMessageHandler::onDisconnected(){
	Debug.println("MQTT disconnected");
}
void  MQTTMessageHandler::onPublished(){
	Debug.println("MQTT published");
}
void  MQTTMessageHandler::onData(String& topic,String& data)
{
	Debug.println("received '"+topic+"':'"+data+"'");
}

/**
 * start message handler; need Networking infrastructure to be set up
 */
void MQTTMessageHandler::start() {
	Debug.println("MessageHandler::start()");

	if(!isConfigured()){
		Debug.println("MessageHandler::start: not configured, exiting..");
	}else{
		MQTT_Connect(&mqttClient); // result will show in callbacks..
	}
}

void MQTTMessageHandler::stop() {

	MQTT_Disconnect(&mqttClient); // result will show in callbacks..
}

void MQTTMessageHandler::sendHeaterStatusMessage(bool isOn) {
	publish("main/dobby/heater",isOn?"1":"0",true); // retained or publishWithQoS
}

void MQTTMessageHandler::sendTestMessage1() {
	Debug.println("sendTestMessage1");
	publish("main/dobby", "Hello friends, from Internet of things :)"); // or publishWithQoS
}

void MQTTMessageHandler::sendUserButtonMessage() {
	Debug.println("sendUserButtonMessage");
	publish("main/dobby/userbutton", "pressed"); // or publishWithQoS
}



/**
 * print status message
 * @param out
 */
void MQTTMessageHandler::printStatus(Print* out) {
	String status;
	if(!isConfigured()){
		out->println("MessageHandler not yet  initialized!");
		return;
	}
//	switch(mqtt->getConnectionState()){
//		case eTCS_Ready: status="eTCS_Ready";break;
//		case eTCS_Connecting: status="eTCS_Connecting";break;
//		case eTCS_Connected: status="eTCS_Connected";break;
//		case eTCS_Successful: status="eTCS_Successful";break;
//		case eTCS_Failed: status="eTCS_Failed";break;
//	}
//	out->printf("MQTT ConnectionState = %s\r\n",status.c_str());
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
	if(!isConfigured()) return false;
	 return mqttClient.connState>=MQTT_DATA;
}

bool MQTTMessageHandler::publish(String topic, String message, bool retained) {
	return publishWithQoS(topic,message,1,retained);
}

bool MQTTMessageHandler::publishWithQoS(String topic, String message, int QoS,
		bool retained) {
	if(!isConnected()) return false; // needed??
	return MQTT_Publish(&mqttClient,topic.c_str(),message.c_str(),message.length(),QoS,retained);

}

bool MQTTMessageHandler::subscribe(Device& device) {
	// just wildcard them
	String topic=deviceTopicPrefix(device)+"/#";
	Debug.printf("subcribing topic '%s' for device '%s'\r\n",topic.c_str(),device.id().c_str());
	if(isConnected()){
		return MQTT_Subscribe(&mqttClient,topic.c_str(),1);
	}else{
		Debug.println("couldn't subscribe, not connected..");
		return false;
	}
}

String MQTTMessageHandler::deviceTopicPrefix(Device& device) {
	return Node::node().id()+"/"+device.id();
}

void MQTTMessageHandler::load(JsonObject& object) {
	Logger::logheap("MQTTMessageHandler::load 0");

	String host=object["host"].asString();
	unsigned int  port=object["port"];


	//TODO: safety checks go here...
	if(host!=NULL && port!=0){
		configure(host,port); //TODO: do full config!
	}
	Logger::logheap("MQTTMessageHandler::load done.");
}

void MQTTMessageHandler::save(JsonObject& object) {
	if(isConfigured()){
		String host((const char *)mqttClient.host);

		Debug.printf("saving mqtt: host='%s' port=%u\r\n",host.c_str(),mqttClient.port);
		object.addCopy("host", host);
		object.add("port").set(mqttClient.port);
	}
}

//}  // namespace dobby


