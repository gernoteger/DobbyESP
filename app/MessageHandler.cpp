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

#include "Debug.h"

#include "MessageHandler.h"


extern String nodeId();


MessageHandler::MessageHandler(){

}

MessageHandler::~MessageHandler(){

}

void MessageHandler::start() {


	mqtt=new MqttClient("192.168.1.1", 1883, MqttStringSubscriptionCallback(&MessageHandler::onMessageReceived,this));
	if(mqtt!=NULL){
		String name="esp8266-"+nodeId();
		mqtt->connect(name);
		mqtt->subscribe("main/status/#");
		Debug.printf("connected mqtt with client id '%s'\r\n",name.c_str());

		Serial.println("================= connected mqtt");
	}else {
		Serial.println("######## Error opening MQTT Client!");
	}
}

void MessageHandler::stop() {
	//if(mqtt) mqtt->close();
	// can we delete it??? Closing is not working correctly!
	//delete mqtt;
}

void MessageHandler::sendTestMessage1() {
	if (mqtt->getConnectionState() != eTCS_Connected)
		start(); // Auto reconnect

	Serial.println("Let's publish message now!");
	mqtt->publish("main/dobby", "Hello friends, from Internet of things :)"); // or publishWithQoS
}

/**
 * print status message
 * @param out
 */
void MessageHandler::printStatus(Print* out) {
	String status;

	switch(mqtt->getConnectionState()){
		case eTCS_Ready: status="eTCS_Ready";break;
		case eTCS_Connecting: status="eTCS_Connecting";break;
		case eTCS_Connected: status="eTCS_Connected";break;
		case eTCS_Successful: status="eTCS_Successful";break;
		case eTCS_Failed: status="eTCS_Failed";break;
	}
	out->printf("MQTT ConnectionState = %s\r\n",status.c_str());
}

void MessageHandler::onMessageReceived(String topic, String message)
{
	//TODO. find hanlers for that

	//Serial.printf("mqtt rcv: '%s': '%s'\r\n");

	Serial.print(topic);
	Serial.print(":\r\n\t"); // Prettify alignment for printing
	Serial.println(message);
}




