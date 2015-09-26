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
#include "IOHandler.h"

extern String nodeId();


MessageHandler::MessageHandler(){

}

MessageHandler::~MessageHandler(){

}

/**
 * start message handler; need Networking infrastructure to be set up
 */
void MessageHandler::start() {


	if(mqtt!=NULL){
		String name="esp8266-"+nodeId();
		if(mqtt->connect(name)){
			mqtt->subscribe("main/status/#");
			mqtt->subscribe("main/commands/#");
		}else{
			Debug.println("######## Error connecting to MQTT Client!");
		}
		Debug.println("================= ");
		Debug.printf("connected mqtt with client id '%s'\r\n",name.c_str());
	}else {
		Debug.println("######## Error opening MQTT Client!");
	}
}

void MessageHandler::stop() {
	Debug.println("MessageHandler::stop() not implemented");
}

void MessageHandler::sendTestMessage1() {
	Debug.println("Let's publish message now!");
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

/**
 * configure connection params for the message handler; doesn't need to be connected then!
 * @param serverHost
 * @param serverPort
 */
void MessageHandler::configure(String serverHost, int serverPort) {
	//TODO: can't disconnect all if reconfigured
	if(mqtt){
		debugf("mqtt client already configured!");
		return;
	}
	mqtt=new MqttClient(serverHost, serverPort, MqttStringSubscriptionCallback(&MessageHandler::onMessageReceived,this));
}

void MessageHandler::onMessageReceived(String topic, String message)
{
	//TODO. find hanlers for that

	Debug.print(topic);
	Debug.print(":\r\n\t"); // Prettify alignment for printing
	Debug.println(message);

	if(topic=="main/commands/led/1"){
		IO.setDiagnosticLed(toOnOff(message));
	}

}

/**
 * check connections & revive if needed
 *
 */
void MessageHandler::check() {
	if (mqtt->getConnectionState() != eTCS_Connected)
		start(); // Auto reconnect
}
