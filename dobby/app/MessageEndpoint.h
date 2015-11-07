/*
 * MessageEndpoint.h
 *
 *  Created on: 07.11.2015
 *      Author: gernot
 */

#ifndef APP_MESSAGEENDPOINT_H_
#define APP_MESSAGEENDPOINT_H_

#include "Identifiable.h"

namespace dobby {

/**
 * An source and sink for mqtt messages.
 */
class MessageEndpoint: public virtual Identifiable {

public:
	MessageEndpoint();
	virtual ~MessageEndpoint();

	///@name mqtt registration
	///@{

	String topicPrefix();

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

	///@}

	/**
	 * handle command with specified message
	 * @param command
	 * @param message
	 */
	virtual void handleCommand(const String command,const String message);

	/**
	 * log & complain about invalid command
	 * @param command
	 * @param message
	 */
	void invalidCommand(const String command,const String message,const String errorMessage);

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

};

} /* namespace dobby */

#endif /* APP_MESSAGEENDPOINT_H_ */
