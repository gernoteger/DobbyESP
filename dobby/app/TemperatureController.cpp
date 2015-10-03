/*
 * TemperatureController.cpp
 *
 *  Created on: 30.09.2015
 *      Author: gernot
 */
#include <Debug.h>
#include "ADC.h"
#include "IOHandler.h"

#include "TemperatureController.h"
#include "MessageHandler.h"

TemperatureController::TemperatureController(uint32 intervalMillis) {
	timer.setCallback(TimerDelegate(&TemperatureController::run,this));
	adc.setInput(ADC_TOUT);
	setHeatingOn(false);
	setControlInterval(intervalMillis);
}

TemperatureController::~TemperatureController() {
	// TODO Auto-generated destructor stub
}


void TemperatureController::setControlInterval(uint32 intervalMillis) {
	timer.setIntervalMs(intervalMillis);
}

void TemperatureController::setTargetReading(uint16 targetReading,uint16 hysteresis) {
	readingOn=targetReading+hysteresis/2;
	readingOff=targetReading-hysteresis/2;
}

void TemperatureController::start() {
	Debug.println("TC starting...");
	timer.start(true);
}

void TemperatureController::stop() {
	Debug.println("TC stopping...");
	timer.stop();
	setHeatingOn(false);
}

void TemperatureController::run() {
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
		messageHandler.sendHeaterStatusMessage(isHeating);
	}

}

void TemperatureController::setHeatingOn(bool heating) {
	this->isHeating=heating;
	IO.setHeater(isHeating);
}



