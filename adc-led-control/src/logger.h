/*
 * logger.h
 *
 *  Created on: 06.07.2021
 *      Author: rb
 */

#ifndef _LOGGER_
#define _LOGGER_


#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

// log levels
typedef enum { trace = 0, debug, info, warn, error } loglevel_t;

extern loglevel_t log_level;
extern char log_buffer[255];
extern volatile unsigned int tick;

#define ENABLE_LOG
//#define LIGHT_LOG
//#define TICK_LOG

/*
 * print log details (level, file, line) to buffer
 */
int sprintLogDetails(char * buf, loglevel_t level, const char * file, uint32_t line);

/*
 * create a hex dump and send via can/uart
 */
void log_as_hex_dump(char * buffer, const char * data, int len);

/*
 * send buffer over CAN or/and UART
 */
void logLine(char * buf, uint8_t len, bool binary);

/*
 * print decimal value of a character as hex value
 */
int sprintChar2Hex(char * buf, const unsigned char ch);


#ifdef ENABLE_LOG

	void log(loglevel_t level, const char * file, uint32_t line, const char * data);
	void log_str(loglevel_t level, const char * file, uint32_t line, const char * data, const char * str);
	void log_dec(loglevel_t level, const char * file, uint32_t line, const char * data, const int i);
	void log_hex(loglevel_t level, const char * file, uint32_t line, const char * data, const int i);
	void log_binary(loglevel_t level, const char * file, uint32_t line, const char * data, int length);

	#define LOG_TRACE(data)				log(trace, __FILE__, __LINE__, data)
	#define LOG_TRACE_STR(data, str)	log_str(trace, __FILE__, __LINE__, data, str)
	#define LOG_TRACE_INT(data, i)		log_dec(trace, __FILE__, __LINE__, data, i)

	#define LOG_DEBUG(data)				log(debug, __FILE__, __LINE__, data)
	#define LOG_DEBUG_STR(data, str)	log_str(debug, __FILE__, __LINE__, data, str)
	#define LOG_DEBUG_INT(data, i)		log_dec(debug, __FILE__, __LINE__, data, i)

	#define LOG_INFO(data)				log(info, __FILE__, __LINE__, data)
	#define LOG_INFO_STR(data, str)		log_str(info, __FILE__, __LINE__, data, str)
	#define LOG_INFO_INT(data, i)		log_dec(info, __FILE__, __LINE__, data, i)

	#define LOG_WARN(data)				log(warn, __FILE__, __LINE__, data)
	#define LOG_WARN_STR(data, str)		log_str(warn, __FILE__, __LINE__, data, str)
	#define LOG_WARN_INT(data, i)		log_dec(warn, __FILE__, __LINE__, data, i)

	#define LOG_ERROR(data)				log(error, __FILE__, __LINE__, data)
	#define LOG_ERROR_STR(data, str)	log_str(error, __FILE__, __LINE__, data, str)
	#define LOG_ERROR_INT(data, i)		log_dec(error, __FILE__, __LINE__, data, i)

	#define LOG_AS_HEXDUMP(data, len)	log_binary(trace, __FILE__, __LINE__, data, len);

#else

	#define LOG_TRACE(...) 				{}
	#define LOG_TRACE_STR(data, str)	{}
	#define LOG_TRACE_INT(data, i)		{}

	#define LOG_DEBUG(...) 				{}
	#define LOG_DEBUG_STR(data, str)	{}
	#define LOG_DEBUG_INT(data, i)		{}

	#define LOG_INFO(...) 				{}
	#define LOG_INFO_STR(data, str)		{}
	#define LOG_INFO_INT(data, i)		{}

	#define LOG_WARN(...) 				{}
	#define LOG_WARN_STR(data, str)		{}
	#define LOG_WARN_INT(data, i)		{}

	#define LOG_ERROR(...) 				{}
	#define LOG_ERROR_STR(data, str)	{}
	#define LOG_ERROR_INT(data, i)		{}

	#define LOG_AS_HEXDUMP(data, len) 	{}

#endif


#endif /* _LOGGER_ */
