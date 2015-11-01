/*
 * Switch.cpp
 *
 *  Created on: 18.10.2015
 *      Author: gernot
 */
#include <SmingCore/SmingCore.h>

#include "Debug.h"
#include "Switch.h"

namespace dobby {

Switch::Switch(String& id) : Device(id) {
	// TODO Auto-generated constructor stub

}

Switch::~Switch() {
	// TODO Auto-generated destructor stub
}

/*
 * start: setup  IOs
 */
void Switch::start() {
	Device::start();
	pinMode(gpio, OUTPUT);
}

void Switch::load(JsonObject& object) {
	//  	{"id":"light","type": "switch", "gpio": 4},

	gpio=object["gpio"];
}

void Switch::handleCommand(String command, String message) {
	Debug.println(" Switch::handleCommand");
	Device::handleCommand(command,message); // mainly debugging

	if(command=="switch"){
		if(message=="on"){
			digitalWrite(gpio, 1);
		}else if(message=="off"){
			digitalWrite(gpio, 0);
		}else if(message=="toggle"){
			int last=digitalRead(gpio);
			digitalWrite(gpio, !last);
		}else{
			invalidCommand(command,message,"message unknown");
		}
	}else{
		invalidCommand(command,message,"command unknown");
	}

}

String Switch::usage(){
	return "do/switch on|1|off|0|toggle";
}

} /* namespace dobby */

