/*
 * MessageHandler.h
 *
 *  Created on: 26.09.2015
 *      Author: gernot
 */

#ifndef APP_MESSAGECONNECTION_H_
#define APP_MESSAGECONNECTION_H_


#include "user_config.h"
#include <Delegate.h>


#include "mqtt.h"



#include "Configurable.h"
#include "Device.h"

//namespace dobby {

using namespace dobby;

/**
 * sets up the MQTT Message Hanler: sends & receives messages!
 */
class MessageConnection:  public Configurable {

public:
	MessageConnection();
	virtual ~MessageConnection();

	/**
	 * set name for config
	 * @return
	 */
	virtual String getTypeName() const { return MessageConnection::typeName(); }

	void load(JsonObject& object);
	void save(JsonObject& object);

	/**
	 * configure with actual values
	 *
	 * @param serverHost
	 * @param serverPort
	 */
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
	 * subscribe all topics for a device
	 * @param device
	 * @return true if successful
	 */
	bool subscribe(MessageEndpoint& device);

	/**
	 * create device prefix
	 * @param device
	 * @return deviceTopicPrefix w/o trailing /
	 */
	String deviceTopicPrefix(Device& device);

	/**
	 * check if connected & ready to send!
	 * @return
	 */
	bool isConnected();

	bool isConfigured(){return configured;}

	void sendTestMessage1(); 		///< send a test message
	void sendUserButtonMessage(); 	///< user button pressed
	void sendHeaterStatusMessage(bool isOn);	///< tell about heater state

	void printStatus(Print * out);

	//TODO: could allow modification of host, config, lwt...
	void configure(String host="192.168.1.1",unsigned int port=1883,String user="",String password="",uint32_t keepAliveTimeSeconds=120, uint8_t cleanSession=1);

protected:
	void onMessageReceived(String topic, String message);
	// low level callbacks...
	void onConnected();
	void onDisconnected();
	void onPublished();
	void onData(String& topic,String& data);

	static void staticOnConnected(uint32_t *args);
	static void staticOnDisconnected(uint32_t *args);
	static void staticOnPublished(uint32_t *args);
	static void staticDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len);


	/**
	 * encode message to on/off: 1 is true (on), everything else is false
	 */
	bool toOnOff(String message){ return message=="1"; }

private:
	static String typeName(){return "mqtt";}

	MQTT_Client mqttClient;
	bool configured=false;
};

//}  // namespace dobby

#endif /* APP_MESSAGECONNECTION_H_ */
