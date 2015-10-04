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

namespace dobby {

class Device {
public:
	Device();
	virtual ~Device();


	virtual void config(JsonObject& object){};

	/**
	 * name of type as presented in mqtt messages etc., e.g. shutter, heater
	 * type names are all lowercase, forbidden: '+','#','/'
	 */
	virtual String typeName()=0;

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
