/*
 * GraylogAppender.h
 *
 *  Created on: 03.01.2016
 *      Author: gernot
 */



#ifndef APP_DEVICES_GRAYLOGAPPENDER_H_
#define APP_DEVICES_GRAYLOGAPPENDER_H_

#include "logging.h"
#if LOG_LEVEL>LOG_LEVEL_OFF

namespace dobby {

class GELFAppender {
public:
	GELFAppender(String host,uint16 port);
	~GELFAppender();

	/**
	 * tell logger we are ready
	 */
	void add();


private:
	IPAddress host;
	uint16 port;

	UdpConnection udp=UdpConnection();

	log_appender appender;
	static void log_message_cb(log_level level,const char * file,uint32 line,const char* message,const char* message_full,void * userdata);

	void log_message(log_level level,const char * file,uint32 line,const char* message,const char* message_full);
};

void GELF_add_appender(String host,int port);

} /* namespace dobby */


#endif

#endif /* APP_DEVICES_GRAYLOGAPPENDER_H_ */


