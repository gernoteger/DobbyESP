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

class Device: public Configurable{
public:
	Device(String _id);
	virtual ~Device();

	/**
	 * start device. Override if needed
	 */
	virtual void start(){}

	/**
	 * stop device. Override if needed
	 */
	virtual void stop(){}

	/**
	 * get type Name as used
	 * @return
	 */
	virtual String getTypeName()=0;

	///@name mqtt registration
	///@{
	/**
	 * list all commands understood by this device. must be destroyed by caller.
	 * @return
	 */
	Vector<String>& commands();

	/**
	 * descriptions for help message
	 * @return
	 */
	Vector<String>& signalDescriptions();

	/**
	 * handle command with specified message
	 * @param command
	 * @param message
	 */
	virtual void handleCommand(String command,String message);


	///@}

	/**
	 * return id for device. MUST be set!!!
	 * @return
	 */
	String id(){ return _id;}

protected:

	///@name mqtt registration
	///@{

	virtual void addCommandDescriptions(Vector<String>& commands){};
	virtual void addSignalDescriptions(Vector<String>& signals){};

	/**
	 * publish a signal for this device; the full topic will be composed by this.
	 *
	 * @param signal
	 * @param message
	 * @param retained default=true
	 * @return
	 */
	bool publish(String signal, String message, bool retained = true);
	///@}

	//expect static: static String typeName();
private:
	String _id;
};

} /* namespace dobby */

#endif /* APP_DEVICE_H_ */
