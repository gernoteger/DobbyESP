/*
 * logging.c
 *
 * implementations for logging.
 *
 * stay in C, so we can use it in early phases!
 */



#include "user_config.h"

#include <stdio.h>
#include <string.h>

#include <SmingCore/SmingCore.h>
#include "Debug.h"
#include "logging.h"

#if LOG_LEVEL>LOG_LEVEL_OFF

log_level  currentLogLevel;

#define MAX_FORMAT_SIZE 40
#define MAX_MESSAGE_SIZE 256

void log_message_debugf(log_level level,const char * file,uint32 line,const char* message,const char* message_full,void * userdata);




log_appender uart_appender;
log_appender * appenders;



void init_logging(){
	appenders=NULL;
	uart_appender.cb_log_message=log_message_debugf;
	uart_appender.userdata=NULL;
	add_appender(&uart_appender);
}


/**
 * add appenders; there is no remove!
 * @param appender add this appender & set next to NULL;
 */
void add_appender(log_appender * new_appender){
	debugf("add_appender: %d",__LINE__);
	new_appender->next=NULL;
	if(appenders==NULL){
		appenders=new_appender;
		return;
	}

	debugf("add_appender: %d",__LINE__);

	for(log_appender * a=appenders;a!=NULL;appenders=appenders->next){
		if(appenders->next==NULL){
			appenders->next=new_appender;
			return;
		}
	}
}


//TODO: put somewhere else..
void m_snprintf(char *buf, size_t maxLen, const char *fmt, ...){
	va_list args;
	int n = 0;

	va_start(args, fmt);
	m_vsnprintf(buf, maxLen, fmt, args);
	va_end(args);
}

/**
 * my logger; will call log appenders as they are available
 * @param level
 * @param file
 * @param line
 * @param format
 */
void log_message(log_level level,const char * file,uint32 line,const char* format,...){
	//TODO: for now: just the serial...
	debugf("log_message START");

	debugf("log_message: %d", __LINE__);
	//create message once for all appenders...
	char msg[MAX_MESSAGE_SIZE];

	va_list args;
	va_start(args, format);
	m_snprintf(msg,sizeof(msg),format,args);

//	log_message_debugf(level,file,line,msg,NULL);
	debugf("log_message: %d", __LINE__);


	for(log_appender * a=appenders;a!=NULL;a=a->next){
		debugf("log_message: %d", __LINE__);

		a->cb_log_message(level,file,line,msg,NULL,a->userdata);
	}
	debugf("log_message: %d", __LINE__);

	// message is composed for all, hand it over
}

// prototype for appenders
void log_message_debugf(log_level level,const char * file,uint32 line,const char* message,const char* message_full,void * userdata){

	m_printf("%s:%d: %d: %s\r\n", file, line , level,message);

}

/**
 * testing: add a  simple uart appender
 */
void log_add_uart_appender(){
	uart_appender.cb_log_message=log_message_debugf;
	add_appender(&uart_appender);
}

#endif
