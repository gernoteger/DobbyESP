/**
 * @file AppController.h
 *
 *  Created on: 27.09.2015
 *      Author: gernot
 */

#ifndef APP_APPCONTROLLER_H_
#define APP_APPCONTROLLER_H_

#include "IOHandler.h"


/**
 * @brief Controls application  behaviour.
 * It sits between the @ref MessageHandler and the @ref IOHandler, seperating concerns.
 * It knows all high leve functions of both sides. For efficency, signals are imlemented as function calls.
 * All state information is kept here.
 * Specialized behaviors are subclassed; initialization is based on configuration!
 * TODO: define some factory!
 */

class AppController {
public:
	void statusQueryReceived();
	void userButtonPressed();
	void updateButtonPressed();
	void otaCommandReceived();

	/**
	 * just for tesing
	 */
	void diagnosticLedCommandReceived(bool state){
		IO.setDiagnosticLed(state);
	}


};

/**
 * global controller;
 */
extern AppController controller;

#endif /* APP_APPCONTROLLER_H_ */
