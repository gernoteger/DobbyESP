/*
 * Device.h
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#ifndef APP_DEVICE_H_
#define APP_DEVICE_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "Configurable.h"

namespace dobby {

class Device: public Configurable {
public:
	Device();
	virtual ~Device();

	/**
	 * start device. Override if needed
	 */
	virtual void start(){}

	/**
	 * stop device. Override if needed
	 */
	virtual void stop(){}

};

} /* namespace dobby */

#endif /* APP_DEVICE_H_ */
