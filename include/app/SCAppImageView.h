//
//  SCAppImageView.h
//  SnowCat
//
//  Created by Moky on 13-9-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCAppImageView_h
#define SnowCat_SCAppImageView_h

#include "SCUrl.h"
#include "SCAppView.h"

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(APP_NAMESPACE)

class ImageView : public View {
	typedef View super;
	
public:
	void setImage(const URL & filename);
	
#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)
	virtual jobject getAvatar(void);
#else
	virtual void * getAvatar(void);
#endif
};

NAMESPACE_END
NAMESPACE_END

#endif
