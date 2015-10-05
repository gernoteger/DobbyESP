

/**
 * @brief handles all hardware io.
 * Reads Hardware io and passes it to mqtt respectively the console
 */

#ifndef APP_IOHANDLER_H_
#define APP_IOHANDLER_H_


#include "Libraries/Bounce/Bounce.h"


#define LED_PIN1 4 // GPIO4
#define LED_PIN2 5 // GPIO5

#define USER_BUTTON_PIN 13 // GPIO13
#define UPDATE_BUTTON_PIN 0 // GPIO0


namespace dobby {

class IOHandler {
public:
	IOHandler();

	/**
	 * initialize IO configuration
	 */
	void init();
	void setDiagnosticLed(bool on);
	bool getDiagnosticLed();

	void setHeater(bool on);

	/**
	 * handle message on hardware
	 * @param topic
	 * @param message
	 * @return true if handled
	 */
	bool handleMessage(String topic,String message);
private:
	void updateUserButton();
	void updateUpdateButton();

	// setup buttons here
	Bounce userButton=Bounce(USER_BUTTON_PIN,5);
};

extern IOHandler IO;


}  // namespace dobby

#endif /* APP_IOHANDLER_H_ */

