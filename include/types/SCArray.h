//
//  SCArray.h
//  SnowCat
//
//  Created by Moky on 13-8-7.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCArray_h
#define SnowCat_SCArray_h

#include "SCObjectArray.h"
#include "SCObject.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

// foreach
#define SC_ARRAY_FOREACH(__array__, __object__) \
		SC_OBJECT_ARRAY_FOREACH(__array__, __object__)

// foreach_reverse
#define SC_ARRAY_FOREACH_REVERSE(__array__, __object__) \
		SC_OBJECT_ARRAY_FOREACH_REVERSE(__array__, __object__)

class Array : public Object, public ObjectArray
{
public:
	virtual const char * description(void) const;
	
#pragma mark New array
	
	virtual Array * copy(void) const;
	
	static inline Array * array(void) {
		return (Array *)(new Array())->autorelease();
	}
	
	static Array * array(const Array & other) {
		return (Array *)(new Array(other))->autorelease();
	}
	
	static Array * array(IObject * obj1, ...);
	
#pragma mark Query Array
	
	Array * randomArray(unsigned int count = 0) const;
};

NAMESPACE_END

#endif
