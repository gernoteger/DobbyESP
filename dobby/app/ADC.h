/*
 * ADC.h
 *
 *  Created on: 29.09.2015
 *      Author: gernot
 */

#ifndef APP_ADC_H_
#define APP_ADC_H_

#include <Delegate.h>


typedef enum {
	ADC_VDD33,
	ADC_TOUT
}ADC_SOURCE;

typedef Delegate<void(int reading)> ADCDelegate;

/**
 * @brief handles adc
 */
class ADC {
public:
	/**
	 * create ADC end set to desired input
	 * @param
	 */
	ADC(ADC_SOURCE input);
	~ADC();

	/**
	 * set source for readings
	 * @param input
	 */
	void setInput(ADC_SOURCE source);

	/**
	 * read once
	 * @return 0..1024 for 0V..1V on input
	 */
	uint16 read();

	/**
	 *
	 * @param interval_millis
	 * @param callback
	 * @return
	 */
	int startReading(int interval_millis,ADCDelegate callback);
	int stopReading();

	/**
	 * @name advanced maintainance functions; probably needed for Radio decoupling
	 */
	///@{

	/**
	 * floating average over N samples
	 * @param N
	 */
	void setAveraging(int N);

	/**
	 * measure N times as often as the callback function is called. Automatically sets Oversampling to N;
	 * Only makes sense for automatic updates
	 * @param N
	 */
	void setOversampling(int N);

	/**
	 * pause; remembers settings
	 * @return
	 */
	void pauseReading();

	/**
	 * resume reading wth old settings
	 * @return
	 */
	void resumeReading();

	///@}
private:

	ADC_SOURCE source;

	/**
	 * read once w/o any averaging etc
	 * @return
	 */
	uint16 rawRead();

};

extern ADC adc;

#endif /* APP_ADC_H_ */
