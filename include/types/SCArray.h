//
//  SCArray.h
//  SnowCat
//
//  Created by Moky on 13-8-7.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCArray_h
#define SnowCat_SCArray_h

#include "SCBaseArray.h"
#include "SCObject.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

// foreach
#define SC_ARRAY_FOREACH(__array__, __object__) \
		SC_BASE_ARRAY_FOREACH(__array__, __object__, IObject *)

// foreach_reverse
#define SC_ARRAY_FOREACH_REVERSE(__array__, __object__) \
		SC_BASE_ARRAY_FOREACH_REVERSE(__array__, __object__, IObject *)

class Array : public Object, public BaseArray<IObject *>
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
	
	// return UINT_MAX if doesn't contain the object
	inline unsigned int indexOfObject(IObject * const object) const {
		return this->index(object);
	}
	
	inline IObject * objectAtIndex(const unsigned int index) const {
		return this->item(index);
	}
	
	inline IObject * lastObject(void) const {
		return this->lastItem();
	}
	
	inline bool containsObject(IObject * const object) const {
		return this->contains(object);
	}
	
	inline IObject * randomObject(void) const {
		return this->anyItem();
	}
	
	Array * randomArray(unsigned int count = 0) const;
	
#pragma mark Add Object
	
	inline void addObject(IObject * object, const bool retain = true) {
		this->add(object, retain);
	}
	
	inline void addObjectsFromArray(const Array & array, const bool retain = true) {
		this->add(array, retain);
	}
	
	inline void insertObject(IObject * object, unsigned int index, const bool retain = true) {
		this->insert(object, index, retain);
	}
	
	void replaceObjectAtIndex(const unsigned int index, IObject * object, bool release = true) {
		this->replace(object, index, release);
	}
	
#pragma mark Remove Object
	
	inline void removeLastObject(const bool release = true) {
		this->removeLast(release);
	}
	
	inline void removeObject(IObject * object, const bool release = true) {
		this->remove(object, release);
	}
	
	inline void removeObjectAtIndex(const unsigned int index, const bool release = true) {
		this->remove(index, release);
	}
	
	inline void removeObjectsInArray(const Array & array, const bool release = true) {
		this->remove(array, release);
	}
	
	inline void removeAllObjects(const bool release = true) {
		this->removeAll(release);
	}
	
protected:
	virtual void retainItem(IObject * item) const {
		if (item) item->retain();
	}
	
	virtual void releaseItem(IObject * item) const {
		if (item) item->release();
	}
};

NAMESPACE_END

#endif
