/*
 * PushButton.h
 *
 *  Created on: 18.10.2015
 *      Author: gernot
 */

#ifndef APP_DEVICES_PUSHBUTTON_H_
#define APP_DEVICES_PUSHBUTTON_H_

#include <SmingCore/SmingCore.h>
#include "Libraries/Bounce/Bounce.h"

#include "Device.h"

namespace dobby {

class PushButton: public Device {
public:
	PushButton(String& id);
	PushButton(const String&,int gpio);

	~PushButton();

	virtual String getTypeName() const { return PushButton::typeName(); }

	static String typeName(){ return "pushbutton"; }

	/**
	 * start device.
	 */
	virtual void start();

	virtual void load(JsonObject& object);

	/**
	 * @brief action when buttion is going up
	 */
	virtual void buttonUpAction();
	virtual void buttonDownAction();


protected:
	virtual String usage(){return "sends: pressed,long|normal,released##TODO: define";}

private:
	void updateBouncer();


	int gpio=-1;
	int debounceMs=20; //defaults to 5 ms debounce
	Timer timer=Timer();

	Bounce * bouncer=NULL;
};

} /* namespace dobby */

#endif /* APP_DEVICES_PUSHBUTTON_H_ */
