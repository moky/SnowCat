//
//  SCAppImageView.cpp
//  SnowCat
//
//  Created by Moky on 13-9-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCAppImageView.h"

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(APP_NAMESPACE)

#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)

jobject ImageView::getAvatar(void)
{
	if (!m_avatar)
	{
		m_avatar = createViewJNI("android.widget.ImageView");
	}
	return m_avatar;
}

void ImageView::setImage(const URL & filename)
{
	setViewImageJNI(getAvatar(), filename.c_str());
}

#endif // EOF '#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)'

NAMESPACE_END
NAMESPACE_END
