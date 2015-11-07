/*
 * MessageEndpoint.cpp
 *
 *  Created on: 07.11.2015
 *      Author: gernot
 */

#include "user_config.h"

#include <SmingCore/SmingCore.h>
#include <Debug.h>

#include "Node.h"
#include "MessageConnection.h"

#include "MessageEndpoint.h"

namespace dobby {

MessageEndpoint::MessageEndpoint() {
	// TODO Auto-generated constructor stub

}

MessageEndpoint::~MessageEndpoint() {
	// TODO Auto-generated destructor stub
}


Vector<String>& MessageEndpoint::commands() const {
	//TODO:crap
}

Vector<String>& MessageEndpoint::signalDescriptions() const{
	//TODO:crap
}


String MessageEndpoint::topicPrefix() {
	return Node::node().id()+"/"+id();
}

/*
 * sends regsitry message of the form ../hello <usage>
 */
void MessageEndpoint::sendRegistryMessage(){
	//register
	publish("hello",usage());
}

void MessageEndpoint::handleCommand(const String command,const String message) {
	Debug.printf("Device::handleCommand: '%s':'%s'",command.c_str(),message.c_str());
}

void MessageEndpoint::invalidCommand(const String command,const  String message,const String errorMessage) {
	if(command=="usage") return;

	Debug.printf("######invalidCommand: '%s':'%s' %s",command.c_str(),message.c_str(),errorMessage.c_str());
	publish("usage",usage());
}

bool MessageEndpoint::publish(const String signal,const  String message, bool retained) {
	MessageConnection& mqtt=Node::node().getMqttClient();
	Debug.printf("mqtt.isConfigured=%u\r\n",mqtt.isConfigured());

	String topic=topicPrefix()+"/"+signal;

	Debug.println("Device::publish: '"+topic+"':'"+message+"'");

	mqtt.publish(topic,message,retained);
	//
	//		mqtt.sendHeaterStatusMessage(isHeating);
			//Node::node().getMqttClient().sendHeaterStatusMessage(isHeating);
}

} /* namespace dobby */
