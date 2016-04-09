/*
 * BathLightPushbutton.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: gernot
 */

#include "BathLightPushButton.h"

namespace dobby {

ICACHE_FLASH_ATTR BathLightPushButton::BathLightPushButton(const String& id,int gpio): PushButton(id, gpio){

}

ICACHE_FLASH_ATTR BathLightPushButton::~BathLightPushButton() {
	// TODO Auto-generated destructor stub
}

ICACHE_FLASH_ATTR void BathLightPushButton::buttonUpAction(){
	if(lightSwitch){
		lightSwitch->handleCommand("switch","toggle");
	}
	PushButton::buttonUpAction();
}


}
