//
//  SCLog.h
//  SnowCat
//
//  Created by Moky on 13-8-6.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCLog_h
#define SnowCat_SCLog_h

#include "scMacros.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

/**
 *  Print log info
 *
 *  Usage:
 *      SCLog(format, ...)
 *  Output:
 *      "[hh:mm:ss]+[Filename]+[LineNumber]+[Class Function] > Info"
 */
#ifdef SC_LOG
	#define SCLog(format, ...) snowcat::log(__FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#else
	#define SCLog(...)         do {} while(0)
#endif

#if SC_LOG & SC_LOG_ERROR
	#define SCError(...)       SCLog(__VA_ARGS__)
#else
	#define SCError(...)       do {} while(0)
#endif

#if SC_LOG & SC_LOG_INFO
	#define SCError(...)       SCLog(__VA_ARGS__)
#else
	#define SCInfo(...)        do {} while(0)
#endif

#if SC_LOG & SC_LOG_WARNING
	#define SCWarning(...)     SCLog(__VA_ARGS__)
#else
	#define SCWarning(...)     do {} while(0)
#endif

void log(const char * format, ...);
void log(const char * file, const int line, const char * function, const char * format, ...);

NAMESPACE_END

#endif
