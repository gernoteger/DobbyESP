
#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <user_interface.h>

#include <Debug.h>

#include "IOHandler.h"
#include "MessageHandler.h"
#include "AppController.h"

//TODO: define via factory!!!
AppController controller;

/**
 * answer to status query
 */
void AppController::statusQueryReceived() {
	Debug.println("AppController::statusQueryReceived(: not yet implemented"); //TODO: implement
}

/**
 * user interface button pressed
 */

void AppController::userButtonPressed() {
	//just implemet a debugging response: toggle diagnostic led, and send som status message
	Debug.println("AppController::statusQueryReceived(: not yet implemented"); //TODO: implement

	// toggle Led
	IO.setDiagnosticLed(!IO.getDiagnosticLed());

	// send message; maybe add some info here...(status in JSon?)
	messageHandler.sendUserButtonMessage();

}


void AppController::updateButtonPressed() {
	Debug.println("AppController::updateButtonPressed(: not yet implemented"); //TODO: implement
}


void AppController::otaCommandReceived() {
	Debug.println("AppController::otaCommandReceived(: not yet implemented"); //TODO: implement
}
/*
 * AppController.cpp
 *
 *  Created on: 27.09.2015
 *      Author: gernot
 */





