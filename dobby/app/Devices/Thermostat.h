/*
 * TemperatureController.h
 *
 *  Created on: 30.09.2015
 *      Author: gernot
 */

#ifndef APP_THERMOSTAT_H_
#define APP_THERMOSTAT_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "Device.h"

namespace dobby {


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
class Thermostat: public Device {

	static const uint16 READING_MAX=1024;
	static const uint16 READING_MIN=0;

public:
	Thermostat(String id);
	virtual ~Thermostat();


	virtual String getTypeName() const{ return Thermostat::typeName(); }
	static String typeName() { return "thermostat"; }

	void setControlInterval(uint32 intervalMillis);

	virtual void load(JsonObject& object);
	void handleCommand(const String command,const String message);

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
protected:
	virtual String usage();
private:

	virtual void addCommandDescriptions(Vector<String>& commands);
	virtual void addSignalDescriptions(Vector<String>& signals);

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

	uint16 hysteresis=10;
	uint16 targetReading;
	//TODO: sensor type?? pos/neg


	uint8 heater_gpio;
	uint32 controlIntervalMillis;

	bool isHeating;
};

}

#endif /* APP_THERMOSTAT_H_ */
