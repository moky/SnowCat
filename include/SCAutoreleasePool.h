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

NAMESPACE_BEGIN(SC_NAMESPACE)

class Array;

class AutoreleasePool : public Object
{
	Array * _pool;
	
public:
	AutoreleasePool(void);
	virtual ~AutoreleasePool(void);
	virtual bool init(void);
	
	void addObject(Object * object);
	void removeObject(Object * object);
	
	void clear(void);
};

class PoolManager
{
public:
	static PoolManager * sharedManager(void);
	
	void push(void);
	void pop(void);
	
	void addObject(Object * object);
	void removeObject(Object * object);
};

NAMESPACE_END

#endif
