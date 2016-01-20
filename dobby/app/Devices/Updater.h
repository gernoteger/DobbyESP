/*
 * Switch.h
 *
 *  Created on: 18.10.2015
 *      Author: gernot
 */

#ifndef APP_DEVICES_UPDATER_H_
#define APP_DEVICES_UPDATER_H_

#include <SmingCore/SmingCore.h>

#include "Device.h"

namespace dobby {

/**
 * device for handling update commands
 */
class Updater: public Device {
public:
	Updater();
	~Updater();

	virtual void load(JsonObject& object);

	virtual String getTypeName() const{ return Updater::typeName(); }
	static String typeName() { return "updater"; }

	/**
	 * start device.
	 */
	virtual void start();

	void handleCommand(const String command,const String message);

protected:
	virtual String usage();

private:
	void publishInfo();
	Timer infoTimer;

	// only default configuration for development
	String appUpdateUrl;
	String filesUpdateUrl;

};

} /* namespace dobby */

#endif /* APP_DEVICES_UPDATER_H_ */
