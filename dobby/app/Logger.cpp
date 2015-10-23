/*
 * Logger.cpp
 *
 *  Created on: 19.10.2015
 *      Author: gernot
 */

#include "user_config.h"
#include <SmingCore/SmingCore.h>
#include "Debug.h"
#include "Logger.h"

namespace dobby {

Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}


void Logger::logheap(const char* msg){
	int currentHeap = system_get_free_heap_size();
	Debug.printf("free heap at %s = %d\r\n", msg,currentHeap);
}

} /* namespace dobby */
