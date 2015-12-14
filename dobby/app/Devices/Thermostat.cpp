/*
 * TemperatureController.cpp
 *
 *  Created on: 30.09.2015
 *      Author: gernot
 */

#include "user_config.h"


#include <Debug.h>

#include "Node.h"

#include "ADC.h"

#include "Thermostat.h"

namespace dobby {

Thermostat::Thermostat(String id):Device(id){
	Debug.printf("Thermostat::Thermostat()\r\n");

	timer.setCallback(TimerDelegate(&Thermostat::run,this));
	adc.setInput(ADC_TOUT);
	setHeatingOn(false);
	setControlInterval(1000); //assume sth safe..
}

Thermostat::~Thermostat() {
	// TODO Auto-generated destructor stub
}


void Thermostat::setControlInterval(uint32 intervalMillis) {
	timer.setIntervalMs(intervalMillis);
}

void Thermostat::setTargetReading(uint16 targetReading,uint16 hysteresis) {
	readingOn=targetReading+hysteresis/2;
	readingOff=targetReading-hysteresis/2;
}

void Thermostat::start() {
	Device::start();
	pinMode(heater_gpio, OUTPUT);
	adc.setInput(ADC_TOUT);

	Debug.println("TC starting...");


	timer.setIntervalMs(controlIntervalMillis);
	timer.start();
}

void Thermostat::stop() {
	Debug.println("TC stopping...");
	timer.stop();
	setHeatingOn(false);
}

void Thermostat::run() {
	uint16 curReading=adc.read();

	Debug.printf("TempController reading=%u readingOn=%u readingOff=%u\r\n",curReading,readingOn,readingOff);
// lower reading means higher temperature

	//modelling hysteresis..
	boolean newHeating=isHeating;
	if(curReading<readingOff){
		newHeating=false;
	}else if(curReading>readingOn){
		newHeating=true;
	}

	if(newHeating != isHeating){
		setHeatingOn(newHeating); // only do when status changes, since it will also trigger actions...

		publish("heating",isHeating?"1":"0",true); // use numbers for better reporting /graphing

//		MQTTMessageHandler& mqtt=Node::node().getMqttClient();
//		Debug.printf("mqtt.isConfigured=%u\r\n",mqtt.isConfigured());
//
//		mqtt.sendHeaterStatusMessage(isHeating);
		//Node::node().getMqttClient().sendHeaterStatusMessage(isHeating);
	}

}

void Thermostat::setHeatingOn(bool heating) {
	this->isHeating=heating;
	digitalWrite(heater_gpio,heating?1:0);
}

void dobby::Thermostat::addCommandDescriptions(Vector<String>& commands) {
	//TODO: just dummys
	commands.add("controller"); 	//on/off
	commands.add("setTargetValue");	//Number...
}

void Thermostat::addSignalDescriptions(Vector<String>& signals) {
}

void Thermostat::load(JsonObject& object) {
	//"heater_gpio": 12, "sensor": "ntc", "hyteresis":10, "controlIntervalMillis":1000
	heater_gpio=object["heater_gpio"];
	controlIntervalMillis=object["controlIntervalMillis"];
	hysteresis=object["hysteresis"];
//	uint16 tragetReading;
	//TODO: sensor type?? pos/neg

	if(object["targetValue"].is<long>()){
		setTargetReading(object["targetValue"]);
	}



}

void Thermostat::handleCommand(const String command,
		const String message) {
	Debug.println(" Switch::handleCommand");
	Device::handleCommand(command,message); // mainly debugging

	//TODO: Error Handling is poor..
	if(command=="setTargetValue"){
		uint16 val=message.toInt(); //TODO: what if 0??
		setTargetReading(val,hysteresis);
	}else if (command=="setHysteresis"){
		hysteresis=message.toInt();
	}else{
		invalidCommand(command,message,"command unknown");
	}
}

String Thermostat::usage() {
		return "setTargetValue\r\nsetHysteresis";
}


}  // namespace dobby

