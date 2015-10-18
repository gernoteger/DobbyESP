/*
 * Device.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */
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

} /* namespace dobby */

Vector<String>& dobby::Device::commands() {
	//TODO:crap
}

Vector<String>& dobby::Device::signalDescriptions() {
	//TODO:crap
}

void dobby::Device::handleCommand(String command, String message) {
	Debug.printf("Device::handleCommand: '%s':'%s'",command.c_str(),message.c_str());
}

bool dobby::Device::publish(String signal, String message, bool retained) {
	MQTTMessageHandler& mqtt=Node::node().getMqttClient();
	Debug.printf("mqtt.isConfigured=%u\r\n",mqtt.isConfigured());

	String topic=Node::node().id()+"/"+id()+"/"+signal;

	Debug.printf("Device::publish: '%s':'%s'",topic.c_str(),message.c_str());

	mqtt.publish(topic,message,retained);
	//
	//		mqtt.sendHeaterStatusMessage(isHeating);
			//Node::node().getMqttClient().sendHeaterStatusMessage(isHeating);
}
