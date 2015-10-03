/*
 * TemperatureController.h
 *
 *  Created on: 30.09.2015
 *      Author: gernot
 */

#ifndef APP_TEMPERATURECONTROLLER_H_
#define APP_TEMPERATURECONTROLLER_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

/**
 * @brief controls temperature
 *
 * Temperature is measured by adc-readings, and heats by switching a relay on.
 * It operates by using a time.
 * The assmption is, that the NTC is connected to the adc with a simple resistor devider:
 * VDD33<--[ R ]--- ADC --[NTC] -->GND
 *
 * therefore: higher readings -> lower temperature.
 * The Controller can be switched off temporarily
 */
class TemperatureController {

	static const uint16 READING_MAX=1024;
	static const uint16 READING_MIN=0;

public:
	TemperatureController(uint32 intervalMillis);
	virtual ~TemperatureController();

	void setControlInterval(uint32 intervalMillis);

	/**
	 * set target reading to
	 * @param target
	 */
	void setTargetReading(uint16 target,uint16 hysteresis=10);

	/**
	 * start the controller
	 */
	void start();

	/**
	 * stop the controller, and turn heating off
	 */
	void stop();

private:

	/**
	 * will run here
	 */
	void run();

	/**
	 * switch heating on/off
	 * @param on
	 */
	void setHeatingOn(bool on);

	Timer timer=Timer();

	// readings: we have 2 different readings to allow hysteresis
	uint16 readingOn=READING_MAX;
	uint16 readingOff=READING_MAX;

	bool isHeating;
};

#endif /* APP_TEMPERATURECONTROLLER_H_ */
