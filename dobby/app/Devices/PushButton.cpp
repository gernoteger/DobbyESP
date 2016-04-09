/*
 * PushButton.cpp
 *
 *  Created on: 18.10.2015
 *      Author: gernot
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <user_interface.h>

#include <Delegate.h>
#include <Debug.h>

#include "PushButton.h"

namespace dobby {

ICACHE_FLASH_ATTR PushButton::PushButton(String& id): Device(id) {
	// TODO Auto-generated constructor stub

}

ICACHE_FLASH_ATTR PushButton::PushButton(const String& id,int gpio) : Device(id) {
	this->gpio=gpio;
}

ICACHE_FLASH_ATTR PushButton::~PushButton() {
	// TODO Auto-generated destructor stub
}

/*
 * start a debouncing button...
 */
ICACHE_FLASH_ATTR void PushButton::start() {
	Device::start();


	pinMode(gpio, CHANGE); //TODO. which one??
	//pinMode(gpio, INPUT); //TODO. which one??

	bouncer=new Bounce(gpio,debounceMs);

	TimerDelegate cb=Delegate<void()>(&PushButton::updateBouncer,this);

	// attach interrupts to bouncer..
	attachInterrupt(gpio, cb, CHANGE);
	//actually, it needs a timer...
	timer.initializeMs(5,cb).start();

}

ICACHE_FLASH_ATTR void PushButton::load(JsonObject& object) {
	//  	{"id":"light_button","type": "pushbutton", "gpio": 4},

	gpio=object["gpio"];
}

ICACHE_FLASH_ATTR void PushButton::buttonUpAction(){
	publish("button","released");
}

ICACHE_FLASH_ATTR void PushButton::buttonDownAction(){
	publish("button","pressed");
}

/**
 * act on rising edge after interrupt
 */
ICACHE_FLASH_ATTR void PushButton::updateBouncer() {
	bouncer->update();
	//TODO: imlement long-pressed!!
	//Debug.println("PushButton: update detected");
	if(bouncer->risingEdge()){
		Debug.println("PushButton: risingEdge");
		buttonUpAction();
	}else if(bouncer->fallingEdge()){
		Debug.println("PushButton: fallingEdge");
		buttonDownAction();
	}
}


//---------------------------------
} /* namespace dobby */


