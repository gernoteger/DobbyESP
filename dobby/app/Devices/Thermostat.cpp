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

Thermostat::Thermostat(String id):Device(id),mode(OFF){
	Debug.printf("Thermostat::Thermostat()\r\n");

	timer.setCallback(TimerDelegate(&Thermostat::run,this));
	adc.setInput(ADC_TOUT);
	setHeatingOn(false);
	setControlInterval(1000); //TODO: assume sth safe..
}

Thermostat::~Thermostat() {
	// TODO Auto-generated destructor stub
}


void Thermostat::setControlInterval(uint32 intervalMillis) {
	Debug.printf("Thermostat::setControlInterval(%u)\r\n",intervalMillis);
	controlIntervalMillis=intervalMillis;
	timer.setIntervalMs(controlIntervalMillis);

	//if(!timer.isStarted())
	timer.start();
}

void Thermostat::setTargetReading(uint16 targetReading,uint16 hysteresis) {
	readingOn=targetReading+hysteresis/2;
	readingOff=targetReading-hysteresis/2;
}

void Thermostat::start() {
	Device::start();
	pinMode(heater_gpio, OUTPUT);
	adc.setInput(ADC_TOUT);

	Debug.printf("TC starting controlIntervalMillis=%u\r\n",controlIntervalMillis);


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


	//TODO: publish measurements here; maybe only if changed??
	publish("rawValue",String(curReading),true);

	if(mode==AUTO){
		//modelling hysteresis..
		boolean newHeating=isHeating;
		if(curReading<readingOff){
			newHeating=false;
		}else if(curReading>readingOn){
			newHeating=true;
		}

		if(newHeating != isHeating){
			setHeatingOn(newHeating,true); // only do when status changes, since it will also trigger actions...
		}
	}

}

void Thermostat::setHeatingOn(bool heating,bool doPublish) {
	this->isHeating=heating;
	digitalWrite(heater_gpio,heating?1:0);

	if(doPublish){
		publish("heating",isHeating?"1":"0",true); // use numbers for better reporting /graphing
	}
}

void dobby::Thermostat::addCommandDescriptions(Vector<String>& commands) {
	//TODO: just dummys
	commands.add("mode"); 	//on/off/auto
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

// enforce settings for current mode
void Thermostat::setMode(mode_t newMode){

	switch(newMode){
		case ON:
			setHeatingOn(true,true);
			break;
		case OFF:
			setHeatingOn(false,true);
			break;
		case AUTO:
			//nothing? controller will do it
			break;
	}
	mode=newMode;

}

void Thermostat::handleCommand(const String command,
		const String message) {

	/**
	 * test with:
	 *
	 * dobby-a69c7f/floor/do/setTargetValue 500
	 * dobby-a69c7f/floor/do/setHysteresis 10
	 * dobby-a69c7f/floor/do/setControlInterval 10000
	 * dobby-a69c7f/floor/do/triggerReading
	 * dobby-a69c7f/floor/do/setMode on
	 */
	Debug.println(" Switch::handleCommand");
	Device::handleCommand(command,message); // mainly debugging

	//TODO: Error Handling is poor..
	if(command=="setTargetValue"){
		uint16 val=message.toInt(); //TODO: what if 0??
		setTargetReading(val,hysteresis);
	}else if (command=="setMode"){
		// enable mode: auto/on/off
		// on: always on
		// off: always off
		// auto: use controller parameters
		if(message=="auto"){
			setMode(AUTO);
		}else if (message=="on"){
			setMode(ON);
		}else if (message=="off"){
			setMode(OFF);
		}
	}else if (command=="setHysteresis"){
		hysteresis=message.toInt();
		setTargetReading((readingOn+readingOff)/2,hysteresis);
	}else if (command=="setControlInterval"){
		uint32 intervalMillis=message.toInt(); //TODO: what if 0??
		setControlInterval(intervalMillis);
	}else if (command=="triggerReading"){
		//TODO: manipulate mode??mode=
		run();
	}else{
		invalidCommand(command,message,"command unknown");
	}
}

String Thermostat::usage() {
		return "do:\\r\n"
				"setTargetValue\r\n"
				"setHysteresis\r\n"
				"setMode on|auto|off\r\n";
				"setControlInterval <millis>\r\n";
				"triggerReading\r\n";
}


}  // namespace dobby

