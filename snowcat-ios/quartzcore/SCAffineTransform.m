//
//  SCAffineTransform.m
//  SnowCat
//
//  Created by Moky on 14-7-9.
//  Copyright (c) 2014 Moky. All rights reserved.
//

#import "SCAffineTransform.h"

CATransform3D CATransform3DMakePerspective(CGPoint center, CGFloat disZ)
{
	CATransform3D transToCenter = CATransform3DMakeTranslation(-center.x, -center.y, 0.0f);
	CATransform3D transBack = CATransform3DMakeTranslation(center.x, center.y, 0.0f);
	CATransform3D scale = CATransform3DIdentity;
	scale.m34 = -1.0f / disZ;
	return CATransform3DConcat(CATransform3DConcat(transToCenter, scale), transBack);
}

CATransform3D CATransform3DPerspect(const CATransform3D t, CGPoint center, CGFloat disZ)
{
	return CATransform3DConcat(t, CATransform3DMakePerspective(center, disZ));
}
