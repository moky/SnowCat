//
//  scMacros.h
//  SnowCat
//
//  Created by Moky on 13-7-22.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_scMacros_h
#define SnowCat_scMacros_h

#if !defined (ANDROID) && defined (__android_log_print)
	#define ANDROID
#endif

// platforms
#define SC_PLATFORM_IPHONE_3          0x0001
#define SC_PLATFORM_IPHONE_4          0x0002
#define SC_PLATFORM_IPHONE_5          0x0004
#define SC_PLATFORM_IPHONE            (SC_PLATFORM_IPHONE_3 | SC_PLATFORM_IPHONE_4 | SC_PLATFORM_IPHONE_5)

#define SC_PLATFORM_IPAD_1            0x0101
#define SC_PLATFORM_IPAD_2            0x0102
#define SC_PLATFORM_IPAD_3            0x0104
#define SC_PLATFORM_IPAD_MINI         0x0108
#define SC_PLATFORM_IPAD              (SC_PLATFORM_IPAD_1 | SC_PLATFORM_IPAD_2 | SC_PLATFORM_IPAD_3 | SC_PLATFORM_IPAD_MINI)

#define SC_PLATFORM_IPOD_5            0x0201
#define SC_PLATFORM_IPOD_NANO         0x0202
#define SC_PLATFORM_IPOD              (SC_PLATFORM_IPOD_5 | SC_PLATFORM_IPOD_NANO)

#define SC_PLATFORM_IOS               (SC_PLATFORM_IPHONE | SC_PLATFORM_IPAD | SC_PLATFORM_IPOD)

#define SC_PLATFORM_ANDROID           0x1000

// detect...
#ifdef ANDROID
	#define SC_PLATFORM               SC_PLATFORM_ANDROID
#else
	#define SC_PLATFORM               SC_PLATFORM_IOS
#endif

#define SC_PLATFORM_IS(_X_)           (SC_PLATFORM & (_X_))






#define SC_LOG_ERROR                  1
#define SC_LOG_INFO                   2
#define SC_LOG_WARNING                4
/**
 *  Log level
 *      0: nothing
 *      1: error
 *      3: error, info
 *      7: error, info, warning
 */
#define SC_LOG                        (SC_LOG_ERROR | SC_LOG_INFO | SC_LOG_WARNING)


#ifdef assert
#define SCAssert(cond, msg)           assert(cond)
#else
//#define SCAssert(cond, msg)           if (!(cond)) { SCError("%s", msg); }
#define SCAssert(cond, msg)           do {} while(0)
#endif



// namespace
#ifdef __cplusplus
#	define NAMESPACE_BEGIN(__NS__)    namespace __NS__ {
#	define NAMESPACE_END              }
#	define USING_NAMESPACE(__NS__)    using namespace __NS__
#else
#	define NAMESPACE_BEGIN(__NS__)
#	define NAMESPACE_END
#	define USING_NAMESPACE(__NS__)
#endif
// namespace snowcat {}
#define SC_NAMESPACE                  snowcat
#define USING_NS_SC                   USING_NAMESPACE(SC_NAMESPACE)

#endif
