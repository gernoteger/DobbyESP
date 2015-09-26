/*
 * IOHandler.cpp
 *
 *  Created on: 26.09.2015
 *      Author: gernot
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <user_interface.h>


#include "IOHandler.h"

#define LED_PIN1 4 // GPIO4
#define LED_PIN2 5 // GPIO5

IOHandler IO;

IOHandler::IOHandler() {
}

void IOHandler::init() {
	// configure leds
	pinMode(LED_PIN1, OUTPUT);
	pinMode(LED_PIN2, OUTPUT);

	//configure switches

	//configura ADC
}

/**
 * switch the diagnostic led...
 * @param on
 */
void IOHandler::setDiagnosticLed(bool on) {
	digitalWrite(LED_PIN2, on);
}
