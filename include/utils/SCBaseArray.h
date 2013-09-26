//
//  SCBaseArray.h
//  SnowCat
//
//  Created by Moky on 13-8-12.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCBaseArray_h
#define SnowCat_SCBaseArray_h

#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <vector>

#include "scMacros.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

// foreach
#define SC_BASE_ARRAY_FOREACH(__array__, __item__, __item_type__) \
	if ((__array__) && (__array__)->size() > 0) \
		for (typename BaseArray<__item_type__>::const_iterator _iter = (__array__)->begin(); \
			_iter != (__array__)->end() && ((__item__) = *_iter, true); ++_iter)

// foreach_reverse
#define SC_BASE_ARRAY_FOREACH_REVERSE(__array__, __item__, __item_type__) \
	if ((__array__) && (__array__)->size() > 0) \
		for (typename BaseArray<__item_type__>::const_iterator _iter = (__array__)->end(); \
			_iter != (__array__)->begin() && ((__item__) = *(--_iter), true); )

template <typename E>
class BaseArray : public std::vector<E>
{
	typedef std::vector<E> super;
	
public:
	typedef typename super::const_iterator const_iterator;
	typedef typename super::value_type     value_type;
	
	BaseArray(void) : super() {
	}
	
	BaseArray(const BaseArray & other) : super() {
		this->add(other);
	}
	
	BaseArray(const unsigned int count, E item1, ...) : BaseArray() {
		va_list args;
		va_start(args, item1);
		for (unsigned int i = 0; i < count; ++i) {
			this->addObject(item1);
			item1 = va_arg(args, E);
		}
		va_end(args);
	}
	
	virtual ~BaseArray(void) {
		this->removeAll();
	}
	
#pragma mark Query array
	
	inline unsigned int count(void) const {
		return this->size();
	}
	
	// return UINT_MAX if doesn't contain the object
	inline unsigned int index(const E item) const {
		for (const_iterator iter = this->begin(); iter != this->end(); ++iter)
			if (item == *iter) return iter - this->begin();
		return UINT_MAX;
	}
	
	inline E item(const unsigned int index) const {
		return index < this->size() ? (*this)[index] : NULL;
	}
	
	inline E lastItem(void) const {
		return this->size() > 0 ? (*this)[this->size() - 1] : NULL;
	}
	
	inline bool contains(const E item) const {
		return this->index(item) != UINT_MAX;
	}
	
	// random
	inline E anyItem(void) const {
		return this->size() > 0 ? this->item((unsigned int)(rand() % this->size())) : NULL;
	}
	
#pragma mark Add item
	
	inline void add(E item, const bool retain = true) {
		if (retain) this->retainItem(item);
		this->push_back(item);
	}
	
	inline void add(const BaseArray & array, const bool retain = true) {
		for (const_iterator iter = array.begin(); iter != array.end(); ++iter)
			this->add(*iter, retain);
	}
	
	inline void insert(E item, unsigned int index, const bool retain = true) {
		if (index > this->size()) index = this->size();
		if (retain) this->retainItem(item);
		super::insert(this->begin() + index, item);
	}
	
	inline void replace(E item, const unsigned int index, const bool release = true) {
		if (index >= this->size()) return;
		this->retainItem(item); // this operation always retain the new item
		if (release) this->releaseItem((*this)[index]);
		(*this)[index] = item;
	}
	
#pragma mark Remove item
	
	inline void remove(const unsigned int index, const bool release = true) {
		if (index < this->size()) {
			if (release) this->releaseItem((*this)[index]);
			this->erase(this->begin() + index);
		}
	}
	
	inline void remove(E item, const bool release = true) {
		this->remove(this->index(item), release);
	}
	
	inline void removeLast(const bool release = true) {
		if (this->size() > 0) {
			if (release) this->releaseItem((*this)[this->size() - 1]);
			this->pop_back();
		}
	}
	
	inline void remove(const BaseArray & array, const bool release = true) {
		for (const_iterator iter = array.begin(); iter != array.end(); ++iter)
			this->remove(*iter, release);
	}
	
	inline void removeAll(const bool release = true) {
		if (release)
			for (const_iterator iter = this->begin(); iter != this->end(); ++iter)
				this->releaseItem(*iter);
		this->clear();
	}
	
protected:
	virtual void retainItem(E item) const {};
	virtual void releaseItem(E item) const {};
};

NAMESPACE_END

#endif
