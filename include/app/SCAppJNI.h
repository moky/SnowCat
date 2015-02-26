//
//  SCAppJNI.h
//  SnowCat
//
//  Created by Moky on 13-9-29.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCAppJNI_h
#define SnowCat_SCAppJNI_h

#include "JniHelper.h"
#include "SCGeometry.h"

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(APP_NAMESPACE)

#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)

extern "C"
{
	jobject rootViewJNI(void);
	
	// create
	jobject createViewJNI(const char * className);
	
	// sub view
	void insertSubviewJNI(jobject parent, jobject child, int index);
	void removeSubviewJNI(jobject view, int index);
	
	// property
	void setViewFrameJNI(jobject view, const Size & size, const Point & center);
	void setViewImageJNI(jobject view, const char * image);
}

#endif // EOF '#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)'

NAMESPACE_END
NAMESPACE_END

#endif
