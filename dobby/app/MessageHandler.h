/*
 * MessageHandler.h
 *
 *  Created on: 26.09.2015
 *      Author: gernot
 */

#ifndef APP_MESSAGEHANDLER_H_
#define APP_MESSAGEHANDLER_H_


#include <user_config.h>
#include <Delegate.h>

#include "Configurable.h"

namespace dobby {

/**
 * sets up the MQTT Message Hanler: sends & receives messages!
 */
class MessageHandler:  public Configurable {

public:
	MessageHandler();
	~MessageHandler();

	/**
	 * set name for config
	 * @return
	 */
	String typeName(){return "mqtt";}

	void load(JsonObject& object);
	void save(JsonObject& object);

	/**
	 * configure with actual values
	 *
	 * @param serverHost
	 * @param serverPort
	 */
	void configure(String serverHost,int serverPort);
	void start();	///< configure mqtt handler an start
	void stop();	///< stop it..
	void check();	///< check connection  & revive if needed


	/**
	 * publish message safely. If not safe, do nothing.
	 *
	 * @param topic
	 * @param message
	 * @param retained
	 * @return true if successful
	 */
	bool publish(String topic, String message, bool retained = false);

	/**
	 * publish message safely. If not safe, do nothing.
	 *
	 * @param topic
	 * @param message
	 * @param QoS
	 * @param retained
	 * @return true if successful
	 */
	bool publishWithQoS(String topic, String message, int QoS, bool retained = false);

	/**
	 * check if connected & ready to send!
	 * @return
	 */
	bool isConnected();

	bool isConfigured(){return mqtt!=NULL;}

	void sendTestMessage1(); 		///< send a test message
	void sendUserButtonMessage(); 	///< user button pressed
	void sendHeaterStatusMessage(bool isOn);	///< tell about heater state

	void printStatus(Print * out);

protected:
	void onMessageReceived(String topic, String message);
	/**
	 * encode message to on/off: 1 is true (on), everything else is false
	 */
	bool toOnOff(String message){ return message=="1"; }
private:

	MqttClient * mqtt=NULL;

	// values below rredundant, but needed due to bad MqttClient API
	String server;
	int port;

};

}  // namespace dobby

#endif /* APP_MESSAGEHANDLER_H_ */
