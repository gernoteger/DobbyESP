/*
 * Logger.h
 *
 *  Created on: 19.10.2015
 *      Author: gernot
 */

#ifndef APP_LOGGER_H_
#define APP_LOGGER_H_



namespace dobby {

class Logger {
public:
	Logger();
	~Logger();



	static void logheap(const char* msg);
	static void logheap(const char* file,uint16 line);

};

} /* namespace dobby */

#define LOGHEAP() Logger::logheap(__FILE__,__LINE__)

#endif /* APP_LOGGER_H_ */
