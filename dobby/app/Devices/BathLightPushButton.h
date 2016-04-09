/*
 * BathLightPushbutton.h
 *
 *  Created on: Apr 9, 2016
 *      Author: gernot
 */

#ifndef APP_DEVICES_BATHLIGHTPUSHBUTTON_H_
#define APP_DEVICES_BATHLIGHTPUSHBUTTON_H_

#include "Switch.h"
#include "PushButton.h"

namespace dobby {

/**
 * Pushbuttton for my bath
 */
class BathLightPushButton: public PushButton {
public:
	BathLightPushButton(const String& id,int gpio);

	virtual ~BathLightPushButton();

	// define may own actions
	virtual void buttonUpAction();

	void ICACHE_FLASH_ATTR setLightSwitch(Switch * lightSwitch) {
		this->lightSwitch = lightSwitch;
	}

private:
	Switch * lightSwitch=null;
};
}

#endif /* APP_DEVICES_BATHLIGHTPUSHBUTTON_H_ */
