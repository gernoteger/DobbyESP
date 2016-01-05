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
#include <SmingCore/Network/UdpConnection.h>

#include "Node.h"

#include "logging.h"

#if LOG_LEVEL>LOG_LEVEL_OFF

#include "GELFAppender.h"

namespace dobby {


GELFAppender::GELFAppender(String _host,uint16 _port):port(_port) {

	appender.cb_log_message=GELFAppender::log_message_cb;
	appender.userdata=this;

	host=IPAddress(_host);
	//udp.connect(hostip,port);
	debugf("GELFAppender UDP: %s:%d",host.toString().c_str(),port);
	//LOG_INFO("GELFAppender UDP: "+host.toString()+":");

}

GELFAppender::~GELFAppender() {

}

void GELFAppender::add() {
	//append to loggin
	add_appender(&appender);
}


// static version to resolve userdata
void GELFAppender::log_message_cb(log_level level, const char* file,
		uint32 line, const char* message, const char* message_full,
		void* userdata) {
	GELFAppender * a=(GELFAppender*)userdata;
	a->log_message(level,file,line,message,message_full);
}

String toGELF(log_level level, const char* file,
		uint32 line, const char* message, const char* message_full){

	DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();

		//	{
		//	  "version": "1.1",
		//	  "host": "example.org",
		//	  "short_message": "A short message that helps you identify what is going on",
		//	  "full_message": "Backtrace here\n\nmore stuff",
		//	  "level": 1,
		//	  "_user_id": 9001,
		//	  "_some_info": "foo",
		//	  "_some_env_var": "bar"
		//	}

	String host=Node::node().id();
	//debugf("host=%s",host.c_str());

	root["version"]="1.1";
	root["host"]=host.c_str();
	root["short_message"]=message;
	if(message_full!=NULL){
		root["full_message"]=message;
	}
	//Timestamp: use rtc if valid, otherwise ignore!!
	//root["timestamp"]=123456
	root["level"]=level;
	root["_file"]=file;
	root["_line"]=line;

	return root.toJsonString();
}

void GELFAppender::log_message(log_level level, const char* file,
		uint32 line, const char* message, const char* message_full) {

	String gelf=toGELF(level,file,line,message,message_full);
	//m_printf(gelf.c_str());
	udp.sendStringTo(host,port,gelf);
}

/**
 * @brief: add a GELF appender for this host to logging
 */
void GELF_add_appender(String host,int port){
	GELFAppender * appender=new GELFAppender(host,port);
	appender->add();
}

} /* namespace dobby */

#endif
