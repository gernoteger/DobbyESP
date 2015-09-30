/*
 * ADC.cpp
 *
 *  Created on: 29.09.2015
 *      Author: gernot
 */

#include "ADC.h"
#include "../Wiring/WiringFrameworkIncludes.h"


ADC::ADC(ADC_SOURCE input) {
	// TODO Auto-generated constructor stub
	setInput(input);
}

ADC::~ADC() {
	// TODO Auto-generated destructor stub
}

void ADC::setInput(ADC_SOURCE input) {
	//check write data??
	//from espressif doc: The 107th byte in esp_init_data_default.bin(0〜～127byte) is named as
	//“vdd33_const”, and when wire TOUT pin to external circuitry, the
	//vdd33_const must be set as real power voltage of VDD3P3 pin 3 and 4.
	this->source=source;
	switch(input){
	case ADC_TOUT:
		//vdd33_const=1;
		break;
	case ADC_POWER:
		break;
	}
}

uint16 ADC::read() {
	return rawRead();
}





/* from ESPRESSIF documentation
 * system_adc_read is only available when wire TOUT pin to external
circuitry，Input Voltage Range restricted to 0 ~ 1.0V.
• The 107th byte in esp_init_data_default.bin(0〜～127byte) is named as
“vdd33_const”, and when wire TOUT pin to external circuitry, the
vdd33_const must be set as real power voltage of VDD3P3 pin 3 and 4.
• The range of operating voltage of ESP8266 is 1.8V~3.6V，the unit of
vdd33_const is 0.1V，so effective value range of vdd33_const is [18,
36].
Prototype:
uint16 system_adc_read(void)
Parameter:
none
Return:
input voltage of TOUT pin 6, unit：1/1024 V
 */

uint16 ADC::rawRead() {
	//uint16 system_adc_read(void);
	uint16 reading;

	switch(source){
	case ADC_TOUT:
		reading= system_adc_read();
		break;
	case ADC_POWER:
		//TODO: still don't know how to effectively set vdd33_const.
		reading= system_get_vdd33();
		break;
	}

	return reading;
}
