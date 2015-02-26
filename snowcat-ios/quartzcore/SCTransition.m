//
//  SCTransition.m
//  SnowCat
//
//  Created by Moky on 14-3-27.
//  Copyright (c) 2014 Moky. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "scMacros.h"
#import "SCTransition.h"

//CA_EXTERN NSString * const kCAMediaTimingFunctionLinear __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCAMediaTimingFunctionEaseIn __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCAMediaTimingFunctionEaseOut __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCAMediaTimingFunctionEaseInEaseOut __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCAMediaTimingFunctionDefault __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_3_0);
CAMediaTimingFunction * CAMediaTimingFunctionFromString(NSString * string)
{
	SC_SWITCH_BEGIN(string)
		SC_SWITCH_CASE(string, @"EaseInOut")
			return [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
		SC_SWITCH_CASE(string, @"EaseInEaseOut")
			return [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
		SC_SWITCH_CASE(string, @"EaseIn")
			return [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseIn];
		SC_SWITCH_CASE(string, @"EaseOut")
			return [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseOut];
		SC_SWITCH_CASE(string, @"Linear")
			return [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
		SC_SWITCH_DEFAULT
	SC_SWITCH_END
	
	return [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionDefault];
}

//CA_EXTERN NSString * const kCAFillModeForwards __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCAFillModeBackwards __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCAFillModeBoth __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCAFillModeRemoved __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCAFillModeFrozen CA_DEPRECATED;
NSString * const CAFillModeFromString(NSString * string)
{
	SC_SWITCH_BEGIN(string)
		SC_SWITCH_CASE(string, @"Forward")
			return kCAFillModeForwards;
		SC_SWITCH_CASE(string, @"Backward")
			return kCAFillModeBackwards;
		SC_SWITCH_CASE(string, @"Both")
			return kCAFillModeBoth;
		SC_SWITCH_CASE(string, @"Remove")
			return kCAFillModeRemoved;
//		SC_SWITCH_CASE(string, @"Froze")
//			return kCAFillModeFrozen;
		SC_SWITCH_DEFAULT
	SC_SWITCH_END
	
	return string;
}

/* Common transition types. */
//CA_EXTERN NSString * const kCATransitionFade
//__OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCATransitionMoveIn
//__OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCATransitionPush
//__OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCATransitionReveal
//__OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
NSString * const CATransitionTypeFromString(NSString * string)
{
	SC_SWITCH_BEGIN(string)
		SC_SWITCH_CASE(string, @"Fade")
			return kCATransitionFade;
		SC_SWITCH_CASE(string, @"Move")
			return kCATransitionMoveIn;
		SC_SWITCH_CASE(string, @"Push")
			return kCATransitionPush;
		SC_SWITCH_CASE(string, @"Reveal")
			return kCATransitionReveal;
		SC_SWITCH_DEFAULT
	SC_SWITCH_END
	
	/**
	 *  pageCurl     - 向上翻一页
	 *  pageUnCurl   - 向下翻一页
	 *  rippleEffect - 滴水效果
	 *  suckEffect   - 收缩效果，如一块布被抽走
	 *  cube         - 立方体效果
	 *  oglFlip      - 上下翻转效果
	 */
	return string;
}

/* Common transition subtypes. */
//CA_EXTERN NSString * const kCATransitionFromRight __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCATransitionFromLeft __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCATransitionFromTop __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
//CA_EXTERN NSString * const kCATransitionFromBottom __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
NSString * const CATransitionSubtypeFromString(NSString * string)
{
	SC_SWITCH_BEGIN(string)
		SC_SWITCH_CASE(string, @"Right")
			return kCATransitionFromRight;
		SC_SWITCH_CASE(string, @"Left")
			return kCATransitionFromLeft;
		SC_SWITCH_CASE(string, @"Top")
			return kCATransitionFromTop;
		SC_SWITCH_CASE(string, @"Bottom")
			return kCATransitionFromBottom;
		SC_SWITCH_DEFAULT
	SC_SWITCH_END
	
	return string;
}

@interface SCTransition ()

@property(nonatomic, retain) CATransition * animation;

@end

@implementation SCTransition

@synthesize animation = _animation;

- (void) dealloc
{
	[_animation release];
	[super dealloc];
}

- (instancetype) init
{
	self = [super init];
	if (self) {
		self.animation = nil;
	}
	return self;
}

- (instancetype) initWithDictionary:(NSDictionary *)dict
{
	self = [self init];
	if (self) {
		CATransition * trans = [[CATransition alloc] init];
		trans.delegate = self;
		trans.removedOnCompletion = YES;
		self.animation = trans;
		[trans release];
	}
	return self;
}

// create:
SC_IMPLEMENT_CREATE_FUNCTIONS()

- (BOOL) setAttributes:(NSDictionary *)dict
{
	// duration
	id duration = [dict objectForKey:@"duration"];
	if (duration) {
		_animation.duration = [duration doubleValue];
	}
	
	// type
	NSString * type = [dict objectForKey:@"type"];
	if (type) {
		_animation.type = CATransitionTypeFromString(type);
	}
	
	// subtype
	NSString * subtype = [dict objectForKey:@"subtype"];
	if (subtype) {
		_animation.subtype = CATransitionSubtypeFromString(subtype);
	}
	
	// startProgress
	id startProgress = [dict objectForKey:@"startProgress"];
	if (startProgress) {
		_animation.startProgress = [startProgress floatValue];
	}
	
	// endProgress
	id endProgress = [dict objectForKey:@"endProgress"];
	if (endProgress) {
		_animation.endProgress = [endProgress floatValue];
	}
	
	// removedOnCompletion
	id removedOnCompletion = [dict objectForKey:@"removedOnCompletion"];
	if (removedOnCompletion) {
		_animation.removedOnCompletion = [removedOnCompletion boolValue];
	}
	
	// timingFunction
	NSString * timingFunction = [dict objectForKey:@"timingFunction"];
	if (timingFunction) {
		_animation.timingFunction = CAMediaTimingFunctionFromString(timingFunction);
	}
	
	// fillMode
	NSString * fillMode = [dict objectForKey:@"fillMode"];
	if (fillMode) {
		_animation.fillMode = CAFillModeFromString(fillMode);
	}
	
	return YES;
}

- (void) runWithView:(UIView *)view
{
	[view.layer addAnimation:_animation forKey:nil];
}

@end
