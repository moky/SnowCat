//
//  SCLog.cpp
//  SnowCat
//
//  Created by Moky on 13-8-6.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <strings.h>

#include "SCUrl.h"

#include "SCLog.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

inline void _printf(const char *format, va_list args)
{
    char buf[1024 * 16];
	bzero(buf, 1024 * 16);
	
    vsprintf(buf, format, args);
	
#ifdef __android_log_print
    __android_log_print(ANDROID_LOG_DEBUG, "snowcat debug info",  buf);
#else
	printf("SnowCat: %s\n", buf);
#endif
}

inline void log(const char *format, va_list args)
{
	time_t t = time(NULL);
	tm * tm = gmtime(&t);
	
	char buf[16];
	bzero(buf, 16);
	snprintf(buf, 16, "[%02d:%02d:%02d] ", tm->tm_hour, tm->tm_min, tm->tm_sec);
	
	std::string string = std::string(buf).append(format);
	_printf(string.c_str(), args);
}

void log(const char *format, ...)
{
    va_list args;
    va_start(args, format);
	log(format, args);
    va_end(args);
}

void log(const char * file, const int line, const char * function, const char * format, ...)
{
	std::string string(file);
	string = UrlUtils::filename(string);
	
	char buf[256];
	bzero(buf, 256);
	snprintf(buf, 256, "%s [% 4d] %s", string.c_str(), line, function);
	
	string = std::string(buf).append(" > ").append(format);
	
    va_list args;
    va_start(args, format);
	log(string.c_str(), args);
    va_end(args);
}

NAMESPACE_END
