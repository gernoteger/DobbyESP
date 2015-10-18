/*
 * Switch.h
 *
 *  Created on: 18.10.2015
 *      Author: gernot
 */

#ifndef APP_DEVICES_SWITCH_H_
#define APP_DEVICES_SWITCH_H_

#include <SmingCore/SmingCore.h>

#include "Device.h"

namespace dobby {

/**
 * a simple outgoing switch; is an Actuator
 */
class Switch: public Device {
public:
	Switch(String& id);
	~Switch();

	virtual String getTypeName(){ return Switch::typeName(); }
	static String typeName(){ return "switch"; }

	/**
	 * start device.
	 */
	virtual void start();
};

} /* namespace dobby */

#endif /* APP_DEVICES_SWITCH_H_ */
