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

private:

	MqttClient * mqtt=NULL;
};
