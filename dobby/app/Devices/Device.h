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
#include "MessageEndpoint.h"

namespace dobby {

class Device: public virtual Configurable, public virtual MessageEndpoint {
public:
	Device(String _id);
	virtual ~Device();

	/**
	 * start device on node locally. No Network releated actions allowed yet. Override if needed.
	 */
	virtual void start();

	/**
	 * stop device. Override if needed
	 */
	virtual void stop();

	/**
	 * get type Name as used
	 * @return
	 */
	virtual String getTypeName() const=0;


};

} /* namespace dobby */

#endif /* APP_DEVICE_H_ */
