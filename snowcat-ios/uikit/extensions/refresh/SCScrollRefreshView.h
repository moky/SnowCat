//
//  SCScrollRefreshView.h
//  SnowCat
//
//  Created by Moky on 15-1-11.
//  Copyright (c) 2015 Moky. All rights reserved.
//

#import "SCScrollRefreshControl.h"

@interface UIScrollRefreshView : UIScrollRefreshControl

@property(nonatomic, retain) NSString * visibleText;       // 'Pull to refresh'
@property(nonatomic, retain) NSString * willRefreshText;   // 'Release to refresh'
@property(nonatomic, retain) NSString * refreshingText;    // 'Refreshing...'
@property(nonatomic, retain) NSString * updatedText;       // 'Last updated'
@property(nonatomic, retain) NSString * updatedTimeFormat; // 'HH:mm:ss'

@property(nonatomic, retain) NSDate * updatedTime;              // last updated time
@property(nonatomic, readwrite, getter=isLoading) BOOL loading; // animating

@property(nonatomic, retain) UIView * trayView;
@property(nonatomic, retain) UIActivityIndicatorView * loadingIndicator;
@property(nonatomic, retain) UILabel * textLabel;
@property(nonatomic, retain) UILabel * timeLabel;

@end

@interface SCScrollRefreshView : UIScrollRefreshView<SCUIKit>

+ (BOOL) setAttributes:(NSDictionary *)dict to:(UIScrollRefreshView *)scrollRefreshView;

@end
