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

class Device: public virtual Configurable{
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

	///@name mqtt registration
	///@{

	/**
	 * sends regsitry message of the form ../hello <usage>
	 */
	void sendRegistryMessage();
	/**
	 * list all commands understood by this device. must be destroyed by caller.
	 * @return
	 */
	Vector<String>& commands() const;

	/**
	 * descriptions for help message
	 * @return
	 */
	Vector<String>& signalDescriptions() const;

	/**
	 * handle command with specified message
	 * @param command
	 * @param message
	 */
	virtual void handleCommand(const String command,const String message);


	///@}

	/**
	 * log & complain about invalid command
	 * @param command
	 * @param message
	 */
	void invalidCommand(const String command,const String message,const String errorMessage);

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
	bool publish(const String signal,const String message, bool retained = true);
	///@}

	virtual String usage()=0;

	//expect static: static String typeName();
private:
	String _id;
};

} /* namespace dobby */

#endif /* APP_DEVICE_H_ */
