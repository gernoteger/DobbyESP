/*
 * Network.h
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#ifndef APP_NETWORK_H_
#define APP_NETWORK_H_

#include "Delegate.h"
#include "Device.h"

namespace dobby {

/**
 * @brief Handles all networking
 *
 * a special Device that does the Networking stuff
 */
class Network: public Device {
public:
	Network();
	virtual ~Network();

	String ssid;
	String password;

	bool dhcp = true;

	IPAddress ip;
	IPAddress netmask;
	IPAddress gateway;

	String typeName(){ return "network"; }

	void config(JsonObject& object);


	void start();

	void enableAccessPoint();
	void disableAccessPoint();

	void startmDNS();

private:
	void connectOk();
	void connectFail();
};

} /* namespace dobby */

#endif /* APP_NETWORK_H_ */
