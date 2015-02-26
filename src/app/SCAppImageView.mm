//
//  SCAppImageView.mm
//  SnowCat
//
//  Created by Moky on 13-9-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCAppImageView.h"

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(APP_NAMESPACE)

void * ImageView::getAvatar(void)
{
	if (!m_pAvatar)
	{
		m_pAvatar = [[UIImageView alloc] init];
	}
	return m_pAvatar;
}

void ImageView::setImage(const URL & filename)
{
	NSString * string = [[NSString alloc] initWithCString:filename.c_str() encoding:NSUTF8StringEncoding];
	UIImage * image = [[UIImage alloc] initWithContentsOfFile:string];
	[string release];
	
	if (!image)
	{
		SCError("no such image: %s", filename.c_str());
		return;
	}
	Size size(image.size.width, image.size.height);
	
	[(UIImageView *)getAvatar() initWithImage:image];
	[image release];
	
	setSize(size);
}

NAMESPACE_END
NAMESPACE_END
