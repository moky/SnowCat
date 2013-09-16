//
//  SCAutoreleasePool.h
//  SnowCat
//
//  Created by Moky on 13-8-7.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCAutoreleasePool_h
#define SnowCat_SCAutoreleasePool_h

#include "scMacros.h"
#include "SCObject.h"
#include "SCArray.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class AutoreleasePool : public Object
{
public:
	AutoreleasePool(void);
	virtual ~AutoreleasePool(void);
	virtual bool init(void);
	
	inline void addObject(Object * object) {
		_pool->addObject(object, false);
	}
	
	inline void removeObject(Object * object) {
		_pool->removeObject(object, false);
	}
	
	inline void clear(void) {
		_pool->removeAllObjects();
	}
	
private:
	Array * _pool;
};

class PoolManager
{
public:
	static PoolManager * sharedManager(void);
	
	void push(void);
	void pop(void);
	
	inline void addObject(Object * object) {
		getCurrentPool()->addObject(object);
	}
	
	inline void removeObject(Object * object) {
		getCurrentPool()->removeObject(object);
	}
	
private:
	Array * getCurrentStack(void);
	
	inline AutoreleasePool * getCurrentPool(void) {
		return (AutoreleasePool *)getCurrentStack()->lastObject();
	}
};

NAMESPACE_END

#endif
