/*
 * PushButton.h
 *
 *  Created on: 18.10.2015
 *      Author: gernot
 */

#ifndef APP_DEVICES_PUSHBUTTON_H_
#define APP_DEVICES_PUSHBUTTON_H_

#include <SmingCore/SmingCore.h>

#include "Device.h"

namespace dobby {

class PushButton: public Device {
public:
	PushButton();
	~PushButton();

	virtual String getTypeName(){ return PushButton::typeName(); }
	static String typeName(){ return "pushbutton"; }

	/**
	 * start device.
	 */
	virtual void start();

private:

};

} /* namespace dobby */

#endif /* APP_DEVICES_PUSHBUTTON_H_ */
