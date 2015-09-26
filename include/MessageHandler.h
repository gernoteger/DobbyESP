/*
 * MessageHandler.h
 *
 *  Created on: 26.09.2015
 *      Author: gernot
 */


#include <user_config.h>
#include <Delegate.h>



/**
 * sets up the MQTT Message Hanler: sends & receives messages!
 */
class MessageHandler {

public:
	MessageHandler();
	~MessageHandler();

	void configure(String serverHost,int serverPort);
	void start();	///< configure mqtt handler an start
	void stop();	///< stop it..
	void check();	///< check connection  & revive if needed


	void sendTestMessage1(); 	///< send a test message
	void printStatus(Print * out);

protected:
	void onMessageReceived(String topic, String message);
	/**
	 * encode message to on/off: 1 is true (on), everything else is false
	 */
	bool toOnOff(String message){ return message=="1"; }
private:

	MqttClient * mqtt=NULL;
};
