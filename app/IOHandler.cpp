/*
 * IOHandler.cpp
 *
 *  Created on: 26.09.2015
 *      Author: gernot
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <user_interface.h>

#include <Delegate.h>
#include <Debug.h>

#include "IOHandler.h"


IOHandler IO;



IOHandler::IOHandler() {
}


void IOHandler::init() {
	// configure leds
	pinMode(LED_PIN1, OUTPUT);
	pinMode(LED_PIN2, OUTPUT);

	//configure switches

	pinMode(USER_BUTTON_PIN, CHANGE); //TODO. which one??


	attachInterrupt(USER_BUTTON_PIN, Delegate<void()>(&IOHandler::updateUserButton,this), CHANGE);
	attachInterrupt(UPDATE_BUTTON_PIN, Delegate<void()>(&IOHandler::updateUpdateButton,this), CHANGE);

	// debounce 5ms
	userButton= Bounce( USER_BUTTON_PIN,5 );

	//configura ADC
}

/**
 * switch the diagnostic led...
 * @param on
 */
void IOHandler::setDiagnosticLed(bool on) {
	digitalWrite(LED_PIN2, on);
}

bool IOHandler::getDiagnosticLed() {
	return digitalRead(LED_PIN2);
}


bool IOHandler::handleMessage(String topic, String message) {
}

/**
 * act on rising edge after interrupt
 */
void IOHandler::updateUserButton() {
	userButton.update();
	if(userButton.risingEdge()){
		// toggle led
		Debug.println("updateUserButton detected");
		setDiagnosticLed(!getDiagnosticLed());
	}
}


void IOHandler::updateUpdateButton() {
	//userButton.update();

}
