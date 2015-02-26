//
//  SCObjectDictionary.h
//  SnowCat
//
//  Created by Moky on 13-8-12.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCObjectDictionary_h
#define SnowCat_SCObjectDictionary_h

#include "SCObject.h"
#include "SCArray.h"
#include "SCBaseDictionary.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

// foreach
#define SC_OBJECT_DICTIONARY_FOREACH(__dict__, __key__, __object__, __key_type__) \
		SC_BASE_DICTIONARY_FOREACH(__dict__, __key__, __object__, __key_type__, IObject *)

// foreach_reverse
#define SC_OBJECT_DICTIONARY_FOREACH_REVERSE(__dict__, __key__, __object__, __key_type__) \
		SC_BASE_DICTIONARY_FOREACH_REVERSE(__dict__, __key__, __object__, __key_type__, IObject *)

template <typename K>
class ObjectDictionary : public BaseDictionary<K, IObject *>
{
	typedef IObject * object_type;
	typedef BaseDictionary<K, object_type> super;
	
public:
	
#pragma mark Query a Dictionary
	
	inline unsigned int count(void) const {
		return this->size();
	}
	
	inline object_type objectForKey(const K & key) const {
		return this->value(key);
	}
	
#pragma mark Adding objects
	
	inline void setObject(object_type object, const K & key, const bool retain = true) {
		this->set(key, object, retain);
	}
	
#pragma mark Removing objects
	
	inline void removeObjectForKey(const K & key, const bool release = true) {
		this->remove(key, release);
	}
	
	inline void removeObjectsForKeys(const BaseArray<K> & keys, const bool release = true) {
		for (typename BaseArray<K>::const_iterator iter = keys.begin(); iter != keys.end(); ++iter)
			this->removeObjectForKey(*iter, release);
	}
	
	inline void removeAllObjects(const bool release = true) {
		this->removeAll(release);
	}
	
protected:
	inline void retainValue(object_type value) const {
		if (value) value->retain();
	}
	
	inline void releaseValue(object_type value) const {
		if (value) value->release();
	}
};

NAMESPACE_END

#endif
