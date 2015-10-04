/*
 * TemperatureSensor.h
 *
 *  Created on: 30.09.2015
 *      Author: gernot
 */

#ifndef APP_NTCSENSOR_H_
#define APP_NTCSENSOR_H_

#include "ADC.h"

namespace dobby {


/**
 * @brief use an NTC to measure temperature via adc
 */
class NTCSensor: public ADC {
public:
	NTCSensor();
	virtual ~NTCSensor();

	/**
	 * read temperatur in celsius
	 */
	uint16 readCelsius();

	/**
	 * convert reading to celsius
	 */
	float reading2celsius(uint16 reading);
	uint16 celsius2reading(float celsius);

};

}

#endif /* APP_NTCSENSOR_H_ */
