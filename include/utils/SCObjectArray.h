//
//  SCObjectArray.h
//  SnowCat
//
//  Created by Moky on 13-9-22.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCObjectArray_h
#define SnowCat_SCObjectArray_h

#include "SCObject.h"
#include "SCBaseArray.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

// foreach
#define SC_OBJECT_ARRAY_FOREACH(__array__, __object__) \
	SC_BASE_ARRAY_FOREACH(__array__, __object__, IObject *)

// foreach_reverse
#define SC_OBJECT_ARRAY_FOREACH_REVERSE(__array__, __object__) \
	SC_BASE_ARRAY_FOREACH_REVERSE(__array__, __object__, IObject *)

class ObjectArray : public BaseArray<IObject *>
{
	typedef IObject * object_type;
	typedef BaseArray<object_type> super;
	
public:
	
#pragma mark Query Array
	
	// return UINT_MAX if doesn't contain the object
	inline unsigned int indexOfObject(object_type const object) const {
		return this->index(object);
	}
	
	inline object_type objectAtIndex(const unsigned int index) const {
		return this->item(index);
	}
	
	inline object_type lastObject(void) const {
		return this->lastItem();
	}
	
	inline bool containsObject(const object_type object) const {
		return this->contains(object);
	}
	
	inline object_type randomObject(void) const {
		return this->anyItem();
	}
	
#pragma mark Add Object
	
	inline void addObject(object_type object, const bool retain = true) {
		this->add(object, retain);
	}
	
	inline void addObjectsFromArray(const ObjectArray & array, const bool retain = true) {
		this->add(array, retain);
	}
	
	inline void insertObject(object_type object, unsigned int index, const bool retain = true) {
		this->insert(object, index, retain);
	}
	
	inline void replaceObjectAtIndex(const unsigned int index, object_type object, bool release = true) {
		this->replace(object, index, release);
	}
	
#pragma mark Remove Object
	
	inline void removeLastObject(const bool release = true) {
		this->removeLast(release);
	}
	
	inline void removeObject(object_type object, const bool release = true) {
		this->remove(object, release);
	}
	
	inline void removeObjectAtIndex(const unsigned int index, const bool release = true) {
		this->remove(index, release);
	}
	
	inline void removeObjectsInArray(const ObjectArray & array, const bool release = true) {
		this->remove(array, release);
	}
	
	inline void removeAllObjects(const bool release = true) {
		this->removeAll(release);
	}
	
protected:
	inline void retainItem(object_type item) const {
		if (item) item->retain();
	}
	
	inline void releaseItem(object_type item) const {
		if (item) item->release();
	}
};

NAMESPACE_END

#endif
