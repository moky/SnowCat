//
//  SCPrismScrollView.m
//  SnowCat
//
//  Created by Moky on 14-4-17.
//  Copyright (c) 2014 Moky. All rights reserved.
//

#import "SCLog.h"
#import "SCNib.h"
#import "SCPageScrollViewDataSource.h"
#import "SCPrismScrollView.h"

@implementation UIPrismScrollView

// if new subview comes, make a wrapper for it, and perform prism effect
- (UIView *) showSubviewAtIndex:(NSUInteger)index
{
	UIView * view = [super showSubviewAtIndex:index];
	[self _wrapSubviewAtIndex:index];
	[self _performPrismEffectOnScrollView:self.scrollView];
	return view;
}

// make a wrapper for the subview
// to perform a prism effect on it
- (void) _wrapSubviewAtIndex:(NSUInteger)index
{
	UIScrollView * scrollView = self.scrollView;
	NSArray * subviews = scrollView.subviews;
	if (index >= [subviews count]) {
		SCLog(@"error index: %u, count: %u", (unsigned int)index, (unsigned int)[subviews count]);
		return;
	}
	UIView * subview = [subviews objectAtIndex:index];
	if (subview.tag == 9527) {
		// already altered
		return;
	}
	
	[subview retain];
	[subview removeFromSuperview];
	
	// build a wrapper
	UIView * view = [[UIView alloc] initWithFrame:subview.frame];
	view.tag = 9527;
	view.clipsToBounds = YES;
	view.autoresizingMask = subview.autoresizingMask;
	// set subview's frame to fill superview
	subview.frame = view.bounds;
	subview.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	[view addSubview:subview];
	
	// put back with a wrapper
	[scrollView insertSubview:view atIndex:index];
	
	// scale to fill
	[self _scaleSubview:subview withView:view toFillSize:scrollView.bounds.size];
	
	[view release];
	
	[subview release];
}

// scale subview to fill the prism window
// if you want an origin subview, override it and do nothing
- (void) _scaleSubview:(UIView *)subview withView:(UIView *)view toFillSize:(CGSize)winSize
{
	CGSize size = view.frame.size;
	if (size.width <= 0 || size.height <= 0) {
		SCLog(@"error size: %@", NSStringFromCGSize(size));
		return;
	}
	CGFloat dw = winSize.width / size.width;
	CGFloat dh = winSize.height / size.height;
	CGFloat delta = MAX(dw, dh);
	if (delta <= 1.0f) {
		// the subview is big enough
		return;
	}
	
	size.width *= delta;
	size.height *= delta;
	
	CGPoint origin = CGPointMake((size.width - winSize.width) * 0.5f, (size.height - winSize.height) * 0.5f);
	
	// resize the wrapper view to fit the window
	view.bounds = CGRectMake(origin.x, origin.y, winSize.width, winSize.height);
	
	// scale the subview to fill the wrapper
	subview.frame = CGRectMake(0, 0, size.width, size.height);
}

// perform prism effect
- (void) _performPrismEffectOnScrollView:(UIScrollView *)scrollView
{
	// calculating
	CGFloat CONTENT_WIDTH  = scrollView.contentSize.width;
	CGFloat CONTENT_HEIGHT = scrollView.contentSize.height;
	CGFloat OFFSET_X       = scrollView.contentOffset.x;
	CGFloat OFFSET_Y       = scrollView.contentOffset.y;
	CGFloat WIDTH          = scrollView.frame.size.width;
	CGFloat HEIGHT         = scrollView.frame.size.height;
	NSUInteger COUNT       = scrollView.subviews.count;
	
	if (OFFSET_X < 0 || OFFSET_X > CONTENT_WIDTH - WIDTH) {
		return;
	}
	if (OFFSET_Y < 0 || OFFSET_Y > CONTENT_HEIGHT - HEIGHT) {
		return;
	}
	
	CGFloat dx = OFFSET_X / WIDTH;
	CGFloat dy = OFFSET_Y / HEIGHT;
	
	CGFloat DELTA_X = 0;
	CGFloat DELTA_Y = 0;
	if (self.direction == UIPageScrollViewDirectionHorizontal) {
		DELTA_X = WIDTH * 0.25;
	} else {
		DELTA_Y = HEIGHT * 0.25;
	}
	
	NSArray * subviews = scrollView.subviews;
	UIView * view;
	for (int i = 0; i < COUNT; i++) {
		view = [[[subviews objectAtIndex:i] subviews] firstObject];
		view.center = CGPointMake(view.bounds.size.width * 0.5 + (dx - i) * DELTA_X,
								  view.bounds.size.height * 0.5 + (dy - i) * DELTA_Y);
	}
}

#pragma mark - UIScrollViewDelegate

// any offset changes, perform prism effect
- (void) scrollViewDidScroll:(UIScrollView *)scrollView
{
	[self _performPrismEffectOnScrollView:scrollView];
}

@end

#pragma mark -

@interface SCPrismScrollView ()

@property(nonatomic, retain) id<UIPageScrollViewDataSource> pageScrollViewDataSource;

@end

@implementation SCPrismScrollView

@synthesize scTag = _scTag;
@synthesize nodeFile = _nodeFile; // filename, use for awaked from nib

@synthesize pageScrollViewDataSource = _pageScrollViewDataSource;

- (void) dealloc
{
	[_pageScrollViewDataSource release];
	
	[_nodeFile release];
	[SCResponder onDestroy:self];
	[super dealloc];
}

- (instancetype) initWithCoder:(NSCoder *)aDecoder
{
	self = [super initWithCoder:aDecoder];
	if (self) {
		_scTag = 0;
		self.nodeFile = nil;
		
		self.pageScrollViewDataSource = nil;
	}
	return self;
}

- (instancetype) initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self) {
		_scTag = 0;
		self.nodeFile = nil;
		
		self.pageScrollViewDataSource = nil;
	}
	return self;
}

- (instancetype) initWithDictionary:(NSDictionary *)dict
{
	self = [self initWithFrame:CGRectZero];
	if (self) {
		[self buildHandlers:dict];
	}
	return self;
}

// buildHandlers:
- (void) buildHandlers:(NSDictionary *)dict
{
	[SCResponder onCreate:self withDictionary:dict];
	
	NSDictionary * dataSource = [dict objectForKey:@"dataSource"];
	if (dataSource) {
		self.pageScrollViewDataSource = [SCPageScrollViewDataSource create:dataSource];
		// self.dataSource only assign but won't retain the data delegate,
		// so don't release it now
		self.dataSource = _pageScrollViewDataSource;
	}
}

// awakeFromNib
SC_UIKIT_IMPLEMENT_AWAKE_FUNCTION()

// create:
SC_UIKIT_IMPLEMENT_CREATE_FUNCTIONS()

// setAttributes:
SC_UIKIT_IMPLEMENT_SET_ATTRIBUTES_FUNCTION()

+ (BOOL) setAttributes:(NSDictionary *)dict to:(UIPrismScrollView *)prismScrollView
{
	if (![SCPageScrollView setAttributes:dict to:prismScrollView]) {
		return NO;
	}
	
	return YES;
}

@end
