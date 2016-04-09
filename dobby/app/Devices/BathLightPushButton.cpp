/*
 * BathLightPushbutton.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: gernot
 */

#include "BathLightPushButton.h"

namespace dobby {

BathLightPushButton::BathLightPushButton(const String& id,int gpio): PushButton(id, gpio){

}

BathLightPushButton::~BathLightPushButton() {
	// TODO Auto-generated destructor stub
}

void BathLightPushButton::buttonUpAction(){
	if(lightSwitch){
		lightSwitch->handleCommand("switch","toggle");
	}
	PushButton::buttonUpAction();
}


}
