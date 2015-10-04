/*
 * Node.h
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#ifndef APP_NODE_H_
#define APP_NODE_H_

#include <SmingCore/SmingCore.h>
#include "Device.h"
#include "Network.h"

namespace dobby {

/**
 * @brief the central class describing a phyical ESP8266 node
 */
class Node {
public:
	/**
	 * initialize for default values
	 */
	Node();
	~Node();

	/**
	 * load from file
	 */
	void load();

	/**
	 * save to file
	 */
	void save();

	/**
	 * startup this node
	 */
	void init();

	String id();

	/**
	 * access the global node Singleton
	 * @return
	 */
	static Node& node();


	void networkConnectOk();
	void networkConnectFailed();
	void networkConnectionLost();


	void startFTP();

	/**
	 * access the devices list..
	 * @return
	 */

	Vector<Device&>& devices();


	Network net;
private:
	String _id="";
	String passphrase=""; 	// the global passphrase for all services..
	FTPServer ftp;
};


} /* namespace dobby */

#endif /* APP_NODE_H_ */
