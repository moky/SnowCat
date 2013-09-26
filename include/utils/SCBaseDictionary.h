//
//  SCBaseDictionary.h
//  SnowCat
//
//  Created by Moky on 13-8-12.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCBaseDictionary_h
#define SnowCat_SCBaseDictionary_h

#include <map>

#include "scMacros.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

// foreach
#define SC_BASE_DICTIONARY_FOREACH(__dict__, __key__, __object__, __key_type__, __object_class__) \
	if ((__dict__) && (__dict__)->size() > 0) \
		for (typename BaseDictionary<__key_type__, __object_class__>::const_iterator _iter = (__dict__)->begin(); \
			_iter != (__dict__)->end() && ((__key__) = _iter->first, (__object__) = _iter->second, true); ++_iter)

// foreach_reverse
#define SC_BASE_DICTIONARY_FOREACH_REVERSE(__dict__, __key__, __object__, __key_type__, __object_class__) \
	if ((__dict__) && (__dict__)->size() > 0) \
		for (typename BaseDictionary<__key_type__, __object_class__>::const_iterator _iter = (__dict__)->end(); \
			_iter != (__dict__)->begin() && ((__key__) = (--_iter)->first, (__object__) = _iter->second, true); )

template <typename K, class V>
class BaseDictionary : public std::map<K, V>
{
	typedef std::map<K, V> super;
	
public:
	typedef typename super::const_iterator const_iterator;
	typedef typename super::value_type     value_type;
	
	BaseDictionary(void) : super() {
	}
	
	BaseDictionary(const BaseDictionary & other) : super() {
		this->set(other);
	}
	
	BaseDictionary(const unsigned int count, const K & key1, V value1, ...) : BaseDictionary() {
		va_list args;
		va_start(args, value1);
		for (unsigned int i = 0; i < count; ++i) {
			this->setObject(value1, key1);
			key1 = va_arg(args, K);
			value1 = va_arg(args, V);
		}
		va_end(args);
	}
	
	virtual ~BaseDictionary(void) {
		this->removeAll();
	}
	
#pragma mark Query dictionary
	
	inline unsigned int count(void) const {
		return this->size();
	}
	
	inline V value(const K & key) const {
		const_iterator iter = this->find(key);
		return (iter == this->end()) ? NULL : iter->second;
	}
	
#pragma mark Add key-value
	
	inline void set(const K & key, V value, const bool retain = true) {
		if (retain) this->retainValue(value);
		this->remove(key, retain); // NOTICE: BE CAREFUL!!
		this->insert(value_type(key, value));
	}
	
	inline void set(const BaseDictionary & other, const bool retain = true) {
		for (const_iterator iter = other.begin(); iter != other.end(); ++iter)
			this->set(iter->first, iter->second, retain);
	}
	
#pragma mark Remove key-value
	
	inline void remove(const K & key, const bool release = true) {
		V val = this->value(key);
		if (val) {
			if (release) this->releaseValue(val);
			this->erase(key);
		}
	}
	
	inline void removeAll(const bool release = true) {
		if (release && this->size() > 0)
			for (const_iterator iter = this->begin(); iter != this->end(); ++iter)
				this->releaseValue(iter->second);
		this->clear();
	}
	
protected:
	virtual void retainValue(V value) const {};
	virtual void releaseValue(V value) const {};
};

NAMESPACE_END

#endif
