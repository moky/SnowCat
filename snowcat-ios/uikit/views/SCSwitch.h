//
//  SCSwitch.h
//  SnowCat
//
//  Created by Moky on 14-4-2.
//  Copyright (c) 2014 Moky. All rights reserved.
//

#import "SCControl.h"

@interface SCSwitch : UISwitch<SCUIKit>

+ (BOOL) setAttributes:(NSDictionary *)dict to:(UISwitch *)uiSwitch;

@end
