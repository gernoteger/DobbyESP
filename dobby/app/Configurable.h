/*
 * Configurable.h
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#ifndef APP_CONFIGURABLE_H_
#define APP_CONFIGURABLE_H_

#include <SmingCore/SmingCore.h>

namespace dobby {

class Configurable {
public:

	/**
	 * load config from JSON
	 * @param object
	 */
	virtual void load(JsonObject& object){};

	/**
	 * save config to json
	 * @param object
	 */
	virtual void save(JsonObject& object){};

	/**
	 * name of type as presented in mqtt messages etc., e.g. shutter, heater
	 * type names are all lowercase, forbidden: '+','#','/'
	 */
	virtual String getTypeName()=0;

	void loadFromParent(JsonObject& parent);
	void saveToParent(JsonObject& parent);
};

} /* namespace dobby */



#endif /* APP_CONFIGURABLE_H_ */
