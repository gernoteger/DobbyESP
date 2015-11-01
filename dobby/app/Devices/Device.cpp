/*
 * Device.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#include "user_config.h"

#include <SmingCore/SmingCore.h>
#include <Debug.h>

#include "MQTTMessageHandler.h"
#include "Node.h"

#include "Device.h"

namespace dobby {

Device::Device(String _id) {
	this->_id=_id;
}

Device::~Device() {
	// TODO Auto-generated destructor stub
}


Vector<String>& Device::commands() const {
	//TODO:crap
}

Vector<String>& Device::signalDescriptions() const{
	//TODO:crap
}


/*
 * sends regsitry message of the form ../hello <usage>
 */
void Device::sendRegistryMessage(){
	//register
	publish("hello",usage());
}

void Device::handleCommand(const String command,const String message) {
	Debug.printf("Device::handleCommand: '%s':'%s'",command.c_str(),message.c_str());
}

void Device::invalidCommand(const String command,const  String message,const String errorMessage) {
	if(command=="usage") return;

	Debug.printf("######invalidCommand: '%s':'%s' %s",command.c_str(),message.c_str(),errorMessage.c_str());
	publish("usage",usage());
}

bool Device::publish(const String signal,const  String message, bool retained) {
	MQTTMessageHandler& mqtt=Node::node().getMqttClient();
	Debug.printf("mqtt.isConfigured=%u\r\n",mqtt.isConfigured());

	String topic=mqtt.deviceTopicPrefix(*this)+"/"+signal;

	Debug.println("Device::publish: '"+topic+"':'"+message+"'");

	mqtt.publish(topic,message,retained);
	//
	//		mqtt.sendHeaterStatusMessage(isHeating);
			//Node::node().getMqttClient().sendHeaterStatusMessage(isHeating);
}

void Device::start() {
	Debug.println("Device::start():"+id());
}

void Device::stop() {
	Debug.println("Device::stop():"+id());
}

} /* namespace dobby */

