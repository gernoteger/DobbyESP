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

	static const uint16 TEMP_MIN=15.0f; // Minimum Temperature in Celsius
	static const uint16 TEMP_MAX=30.0f; // Minimum Temperature

	enum mode_t {OFF=0,ON=1,AUTO=2};


public:
	Thermostat(const String& id);
	Thermostat(const String& id,uint8 heater_gpio,float hysteresis ,uint32 controlIntervalMillis);

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
	void setTargetTemperature(float target,float hysteresis=1.0);

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

	// Reading/temp conversions

	/**
	 * convert a reading to a temperature
	 * @param reading
	 * @return
	 */
	float readingToCelsius(uint16 reading);

	/**
	 * switch heating on/off
	 * @param on
	 */
	void setHeatingOn(bool on,bool publish=false);

	/**
	 * set mode to this & set necessary actions
	 * @param mode
	 */
	void setMode(mode_t mode);

	Timer timer=Timer();

	// readings: we have 2 different readings to allow hysteresis
	float targetTemperature=TEMP_MIN;
	float hysteresis=1.0;
	//TODO: sensor type?? pos/neg

	mode_t mode=OFF;	// 0..off 1..on 2..auto

	uint8 heater_gpio;
	uint32 controlIntervalMillis;

	bool isHeating;
};

}

#endif /* APP_THERMOSTAT_H_ */
