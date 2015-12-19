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

void Thermostat::setTargetTemperature(float _targetReading,float _hysteresis /* =1.0*/) {
	targetTemperature=_targetReading;
	hysteresis=_hysteresis;
}

float Thermostat::readingToCelsius(uint16 reading){

	// TODO: this is some simple, arbitrary linear interpolation; NTC needs something better!!
	//TODO: cap if out of bounds
	// its a NTC: high readings-low temp!
	float minTemp=15.0;
	float maxTemp=30.0;

	// part of temperatore range
	float dt=(float)((float)reading-(float)READING_MIN)/(float)(READING_MAX-READING_MIN);

	float val=maxTemp-(maxTemp-minTemp)*dt;

	return val;
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

	Debug.printf("TempController curTemperature=%f targetTemperature=%f hysteresis=%f\r\n",curReading,targetTemperature,hysteresis);
// lower reading means higher temperature


	float curTemperature=readingToCelsius(curReading);


	//publish measurements here; TODO: maybe only if changed??
	publish("temperature",String(curTemperature),true);

	if(mode==AUTO){
		//modelling hysteresis..
		float hh=hysteresis/2.0;
		float tOff=targetTemperature+hh;
		float tOn=targetTemperature-hh;

		boolean newHeating=isHeating;
		if(curTemperature>tOff){
			newHeating=false;
		}else if(curTemperature<tOn){
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

	if(object["targetValue"].is<float>()){
		float f=object["targetValue"];
		setTargetTemperature(f);
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
	if(command=="setTargetTemperature"){
		float val=message.toFloat(); //TODO: what if 0??
		setTargetTemperature(val,hysteresis);
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
		setTargetTemperature(targetTemperature,hysteresis);
	}else if (command=="setControlInterval"){
		uint32 intervalMillis=message.toInt(); //TODO: what if 0??
		setControlInterval(intervalMillis);
	}else if (command=="triggerReading"){
		//TODO: manipulate mode??mode=
		run();
	}else if (command=="testConversion"){
		uint16 read=message.toInt();
		float val=readingToCelsius(read);

		Debug.printf("reading %u -> %f\r\n",read,val);
	}else{
		invalidCommand(command,message,"command unknown");
	}
}

String Thermostat::usage() {
		return "do:\\r\n"
				"setTargetTemperature\r\n"
				"setHysteresis\r\n"
				"setMode on|auto|off\r\n";
				"setControlInterval <millis>\r\n"
				"testConversion <reading>\r\n"
				"triggerReading\r\n"
				;
}


}  // namespace dobby

