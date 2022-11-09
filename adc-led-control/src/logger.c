/*
 * logger.c
 *
 *  Created on: 06.07.2021
 *      Author: rb
 */


#include "logger.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "ctype.h"
#include "uartHandler.h"


char level_str[5][8] = {"[TRACE]", "[DEBUG]", "[INFO ]", "[WARN ]", "[ERROR]"};

loglevel_t log_level = trace;

char log_buffer[255];


#ifdef ENABLE_LOG

bool is_print(char c) {
	return (c >= 0x20 && c <= 0x7E);
}

/**
  return from int (hex) a char (hex) code (one character)
  example: 'R' -> 2, '\n' -> 10
*/
char codeNibble(unsigned char c)
{
	c &= 0x0F;
	if (c <= 9)
		c += 0x30;
	else
		c += 0x37;
	return c;
}

int sprint(char * buf, const char * data)
{
	strcpy(buf, data);
	return strlen(buf);
}

int sprintChar2Hex(char * buf, const unsigned char ch)
{
	uint8_t index  = 0;
	buf[index++] = codeNibble(ch >> 4);
	buf[index++] = codeNibble(ch);
	buf[index] = '\0';
	return index;
}

int sprintTick(char * buf)
{
	uint8_t len = 0;
	len = sprint(buf, " (");

	itoa(tick, buf + len, 10);
	len = strlen(buf);

	len += sprint(buf + len, "ms)");
	return len;
}

int sprintLogDetails(char * buf, loglevel_t level, const char * file, uint32_t line)
{
	itoa(level, buf, 10);
	uint8_t len = strlen(buf);

#ifndef LIGHT_LOG
	buf[len++] = ';';

	strcpy(buf + len, file);
	len += strlen(file);

	buf[len++] = ';';

	itoa(line, buf + len, 10);
	len = strlen(buf);
#endif

	buf[len++] = ':';
	buf[len] = '\0';

	return len;
}


void logLine(char * buf, uint8_t len, bool binary)
{
#ifdef TICK_LOG
	if (!binary) {
		// add tick (ms since start)
		len += sprintTick(buf + len);
	}
#endif

	// overwrite '\0' with '\n' character
	buf[len++] = '\n';

	sendUART(buf, len);


//	// split the whole buffer and send 8 bytes parts via can
//	uint8_t index = 0, part = 0;
//	while(index < len)
//	{
//		uint8_t dlc = 0;
//		while(index < len && dlc < 8) {
//			index++; dlc++;
//		}
//		send_PDO_log(buf + part, dlc);
//		part += dlc;
//	}
}


void log(loglevel_t level, const char * file, uint32_t line, const char * data)
{
	if (level >= log_level)
	{
		uint8_t index = sprintLogDetails(log_buffer, level, file, line);
		index += sprint(log_buffer + index, data);
		logLine(log_buffer, index, false);
	}
}

void log_str(loglevel_t level, const char * file, uint32_t line, const char * data, const char * str)
{
	if (level >= log_level)
	{
		uint8_t index = sprintLogDetails(log_buffer, level, file, line);
		index += sprint(log_buffer + index, data);
		log_buffer[index++] = ' ';
		index += sprint(log_buffer + index, str);
		logLine(log_buffer, index, false);
	}
}

void log_dec(loglevel_t level, const char * file, uint32_t line, const char * data, const int i)
{
	if (level >= log_level)
	{
		uint8_t index = sprintLogDetails(log_buffer, level, file, line);
		index += sprint(log_buffer + index, data);
		log_buffer[index++] = ' ';
		itoa(i, log_buffer + index, 10);
		logLine(log_buffer, strlen(log_buffer), false);
	}
}

void log_hex(loglevel_t level, const char * file, uint32_t line, const char * data, const int i)
{
	if (level >= log_level)
	{
		uint8_t index = sprintLogDetails(log_buffer, level, file, line);
		index += sprint(log_buffer + index, data);
		log_buffer[index++] = ' ';
		log_buffer[index++] = '0';
		log_buffer[index++] = 'x';
		itoa(i, log_buffer + index, 16);
		logLine(log_buffer, strlen(log_buffer), false);
	}
}

void log_binary(loglevel_t level, const char * file, uint32_t line, const char * data, int length)
{
	if (level >= log_level)
	{
		uint8_t index = sprintLogDetails(log_buffer, level, file, line);
		log_buffer[index++] = '\0';
		log_buffer[index++] = length;
		memcpy(log_buffer + index, data, length);
		index += length;
		logLine(log_buffer, index, true);
	}
}

#endif


//void TEST_LOG(loglevel_t level, const char * file, uint32_t line, char * data, ...)
//{
//	if (trace >= log_level)
//	{
//		uint8_t index = sprintLogDetails(log_buffer, level, file, line);
//
//		 /* Declare a va_list type variable */
//		va_list myargs;
//		va_start(myargs, data);
//
//		index += vsprintf(log_buffer + index, data, myargs);
//
//	    /* Clean up the va_list */
//	    va_end(myargs);
//
//		logLine(log_buffer, index);
//	}
//}




