/*
 * logging.h
 *
 *  Created on: 03.01.2016
 *      Author: gernot
 *
 *  logging for dobby, inspired by https://github.com/NextPlayLabs/nrf_utils
 *
 *  logging is controlled by macros, and a runtime variable
 *
 *  log levels:
 *
 * Macros:
 *
 * LOG_LEVEL:	defines the log level at compile time; runtime log level can be smaller than this
 *
 * LOG_FATAL(format, ...);
 * LOG_ERROR(format, ...);
 * LOG_WARN(format, ...);
 * LOG_INFO(format, ...);
 * LOG_DEBUG(format, ...);
 * LOG_TRACE(format, ...);
 *
 * if a large payload (i.e trace..) must be sent:
 * LOG(level,payload,message,...)
 *
 * set_log_level(level):	set runtime log level to this;
 *
 */

#ifndef INCLUDE_LOGGING_H_
#define INCLUDE_LOGGING_H_



/*
 	 syslog messages.
          0       Emergency
          1       Alert
          2       Critical
          3       Error
          4       Warning
          5       Notice
          6       Informational
          7       Debug
      additionally:
      	  8       Trace
 */


/**
 * possible log levels
 */
#define LOG_LEVEL_OFF      -1

#define LOG_LEVEL_FATAL     0
#define LOG_LEVEL_ERROR     3
#define LOG_LEVEL_WARN      4
#define LOG_LEVEL_INFO      6
#define LOG_LEVEL_DEBUG     7
#define LOG_LEVEL_TRACE     8


/**
 * default (compile time) log level is info
 */
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

#if LOG_LEVEL>LOG_LEVEL_OFF

typedef int8_t log_level;


extern log_level  currentLogLevel;
extern void log_messagef(log_level level,const char * file,uint line,const char* format,...);
extern void log_message(log_level level,const char * file,uint line,String message);


/**
 * struct for appenders. contains callback,
 * TODO: and specific level??
 */
typedef void (*cb_log_message_function)(log_level level,const char * file,uint32 line,const char* message,const char* message_full,void * userdata);

typedef  struct log_appender {
	cb_log_message_function cb_log_message;
	void * userdata;

	struct log_appender * next;
} log_appender ;

extern void add_appender(log_appender * new_appender);
extern void init_logging();





//C version
#define __LOG_DO_ACTUAL_LOGGINGF(level, format, ...)                                     \
do {      \
	log_messagef(level, __FILE__, __LINE__, format, ##__VA_ARGS__); \
} while (0)


//C++ version;
#define __LOG_DO_ACTUAL_LOGGING(level, message)                                     \
do {      \
	log_message(level, __FILE__, __LINE__, String("")+message); \
} while (0)


#endif /* INCLUDE_LOGGING_H_ */

#endif //LOG_LEVEL>LOG_LEVEL_OFF

// define those always!

#if LOG_LEVEL >= LOG_LEVEL_FATAL

#define LOG_FATALF(format, ...) __LOG_DO_ACTUAL_LOGGINGF(LOG_LEVEL_FATAL, format, ##__VA_ARGS__)
#define LOG_FATAL(message) __LOG_DO_ACTUAL_LOGGING(LOG_LEVEL_FATAL, message)

#else
#define LOG_FATALF(format, ...)
#define LOG_FATAL(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR

#define LOG_ERRORF(format, ...) __LOG_DO_ACTUAL_LOGGINGF(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)
#define LOG_ERROR(message) __LOG_DO_ACTUAL_LOGGING(LOG_LEVEL_ERROR, message)

#else
#define LOG_ERRORF(format, ...)
#define LOG_ERROR(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN

#define LOG_WARNF(format, ...) __LOG_DO_ACTUAL_LOGGINGF(LOG_LEVEL_WARN, format, ##__VA_ARGS__)
#define LOG_WARN(message) __LOG_DO_ACTUAL_LOGGING(LOG_LEVEL_WARN, message)

#else
#define LOG_WARNWARNINGF(format, ...)
#define LOG_WARN(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO

#define LOG_INFOF(format, ...) __LOG_DO_ACTUAL_LOGGINGF(LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define LOG_INFO(message) __LOG_DO_ACTUAL_LOGGING(LOG_LEVEL_INFO, message)

#else
#define LOG_INFOF(format, ...)
#define LOG_INFO(message)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG

#define LOG_DEBUGF(format, ...) __LOG_DO_ACTUAL_LOGGINGF(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define LOG_DEBUG(message) __LOG_DO_ACTUAL_LOGGING(LOG_LEVEL_DEBUG, message)

#else
#define LOG_DEBUGF(format, ...)
#define LOG_DEBUG(message)
#endif

