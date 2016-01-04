/*
 * GraylogAppender.cpp
 *
 *  Created on: 03.01.2016
 *      Author: gernot
 */

#include "user_config.h"



#include <stdio.h>
#include <string.h>

#include <SmingCore/SmingCore.h>
#include "logging.h"

#if LOG_LEVEL>LOG_LEVEL_OFF

#include "GraylogAppender.h"

namespace dobby {


GraylogAppender::GraylogAppender(String host,uint16 port):host(host),port(port) {

	appender.cb_log_message=GraylogAppender::log_message;
	appender.userdata=this;
}

GraylogAppender::~GraylogAppender() {
	// TODO Auto-generated destructor stub
}

void GraylogAppender::add() {
	//append to loggin
	add_appender(&appender);
}


// static version to resolve userdata
void GraylogAppender::log_message(log_level level, const char* file,
		uint32 line, const char* message, const char* message_full,
		void* userdata) {
	GraylogAppender * a=(GraylogAppender*)userdata;
	a->log_message(level,file,line,message,message_full);
}

void GraylogAppender::log_message(log_level level, const char* file,
		uint32 line, const char* message, const char* message_full) {

	//TODO: graylog
	m_printf("file=%s line=%d level=%d message=%s\r\n", file, line , level,message);
}

} /* namespace dobby */

#endif
