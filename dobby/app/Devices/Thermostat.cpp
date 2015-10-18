/*
 * TemperatureController.cpp
 *
 *  Created on: 30.09.2015
 *      Author: gernot
 */
#include <Debug.h>

#include "Node.h"

#include "ADC.h"
#include "IOHandler.h"

#include "Thermostat.h"
#include "MessageHandler.h"

namespace dobby {

Thermostat::Thermostat(uint32 intervalMillis) {
	Debug.printf("Thermostat::Thermostat(%u)\r\n",intervalMillis);

	timer.setCallback(TimerDelegate(&Thermostat::run,this));
	adc.setInput(ADC_TOUT);
	setHeatingOn(false);
	setControlInterval(intervalMillis);
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
	Debug.println("TC starting...");
	timer.start(true);
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
		MessageHandler& mqtt=Node::node().getMqttClient();
		Debug.printf("mqtt.isConfigured=%u\r\n",mqtt.isConfigured());

		mqtt.sendHeaterStatusMessage(isHeating);
		//Node::node().getMqttClient().sendHeaterStatusMessage(isHeating);
	}

}

void Thermostat::setHeatingOn(bool heating) {
	this->isHeating=heating;
	IO.setHeater(isHeating);
}

}  // namespace dobby



