//
//  SCFileManager.mm
//  SnowCat
//
//  Created by Moky on 13-8-12.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCFileManager.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

/**
 *  directory type:
 *      0 - resources directory
 *      1 - document directory
 *      2 - caches directory
 *      4 - temporary directory
 */
bool SCFileManager_makeDirectory(const int type, std::string & dir)
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	NSString * string = @"";
	switch (type) {
		case 0:
			string = [[NSBundle mainBundle] resourcePath];
			break;
		case 1:
			string = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
			break;
		case 2:
			string = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0];
			break;
		case 4:
			string = NSTemporaryDirectory();
			break;
		default:
			break;
	}
	dir.clear();
	dir.assign([string cStringUsingEncoding:NSUTF8StringEncoding]);
	
	[pool release];
	return true;
}

NAMESPACE_END
