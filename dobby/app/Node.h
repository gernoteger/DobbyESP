/*
 * Node.h
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#ifndef APP_NODE_H_
#define APP_NODE_H_

#include <SmingCore/SmingCore.h>
#include <SmingCore/Network/TelnetServer.h>
#include <Debug.h>

#include "MessageConnection.h"
#include "MessageEndpoint.h"


#include "IOHandler.h"
#include "CommandLine.h"

#include "Devices/Thermostat.h"

#include "Network.h"
#include "Updater.h"


namespace dobby {

/**
 * @brief the central class describing a phyical ESP8266 node
 */
class Node: public virtual MessageEndpoint,public virtual Identifiable {
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


	/**
	 * access the global node Singleton
	 * @return
	 */
	static Node& node();

	/**
	 * subscribe all devices when connections are (re-)made
	 */
	void subscribeDevices();

	/**
	 * get the Device with the id
	 * @param id
	 * @return Device * or NULL if not found
	 */
	MessageEndpoint * device(String id);

	Updater& updater();

	///@name Application Logic
	///@{

	void networkConnectOk();
	void networkConnectFailed();
	void networkConnectionLost();

	void statusQueryReceived();
	void userButtonPressed();
	void updateButtonPressed();
	void otaCommandReceived();


	/**
	 * just for testng
	 */
	void diagnosticLedCommandReceived(bool state){
		IO.setDiagnosticLed(state);
	}


	void startFTP();
	void stopTelnetServer();
	void startTelnetServer();

	void mqttSendTestMessage(){
		mqtt.sendTestMessage1();
	}

	void mqttPrintStatus(CommandOutput * out){
		mqtt.printStatus(out);
	}

	void mqttConnect(){
		mqtt.start();
	}

	void thermostatStart(){

	}
	void thermostatStop(){

	}
	void thermostatSet(){

	}


	///@}



	MessageConnection& getMqttClient(){return mqtt;}

protected:
	virtual String usage(){return "##TODO: node usage docu";}

private:
	template <class D> void loadDevice(JsonObject& device);
	void add(Device& device);

	String passphrase=""; 	// the global passphrase for all services..

	// Services are handlerd here...
	CommandLine cmd=CommandLine();

	FTPServer * ftp=new FTPServer();
	TelnetServer telnet=TelnetServer();
	Network net=Network();
	MessageConnection mqtt=MessageConnection();
	Updater _updater=Updater();

	//Devices: all in one: map if id to device
	HashMap<String, Device *> devices;
};


} /* namespace dobby */

#endif /* APP_NODE_H_ */
