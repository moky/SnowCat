//
//  SCAutoreleasePool.cpp
//  SnowCat
//
//  Created by Moky on 13-8-7.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include <pthread.h>

#include "SCLog.h"
#include "SCObject.h"
#include "SCString.h"
#include "SCDictionary.h"

#include "SCAutoreleasePool.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

AutoreleasePool::AutoreleasePool(void)
: Object()
, _pool(NULL)
{
	_pool = new Array();
	SCAssert(_pool, "Not enough memory");
}

AutoreleasePool::~AutoreleasePool(void)
{
	clear();
	if (_pool)
	{
		delete _pool;
	}
}

bool AutoreleasePool::init(void)
{
	clear();
	return true;
}

#pragma mark -

static PoolManager * s_pSharedPoolManager = NULL;
static ObjectDictionary<pthread_t> * s_pSharedPoolStacks = NULL;

static pthread_mutex_t s_async_mutex_autorelase_pool;

PoolManager * PoolManager::sharedManager(void)
{
	if (!s_pSharedPoolManager)
	{
		// create pool manager
		s_pSharedPoolManager = new PoolManager();
		SCAssert(s_pSharedPoolManager, "Not enough memory");
		
		// create pool stacks
		if (!s_pSharedPoolStacks)
		{
			s_pSharedPoolStacks = new ObjectDictionary<pthread_t>();
			SCAssert(s_pSharedPoolStacks, "Not enough memory");
		}
		
		pthread_mutex_init(&s_async_mutex_autorelase_pool, NULL);
	}
	return s_pSharedPoolManager;
}

Array * PoolManager::getCurrentStack(void)
{
	pthread_t tid = pthread_self();
	SCAssert(tid != 0, "Can not get thread id!");
	
	pthread_mutex_lock(&s_async_mutex_autorelase_pool);
	
	Array * stack = (Array *)s_pSharedPoolStacks->objectForKey(tid);
	if (!stack)
	{
		stack = new Array();
		SCAssert(stack, "Not enough memory");
		s_pSharedPoolStacks->setObject(stack, tid);
		SCLog("new autorelease pool for thread id: %lu, count: %u", (unsigned long)tid, s_pSharedPoolStacks->count());
	}
	
	pthread_mutex_unlock(&s_async_mutex_autorelase_pool);
	return stack;
}

void PoolManager::push(void)
{
	Array * stack = getCurrentStack();
	SCAssert(stack, "could not happen");
	if (stack)
	{
		AutoreleasePool * pool = new AutoreleasePool(); // ref = 1
		SCAssert(pool, "Not enough memory");
		if (pool)
		{
			pool->init();
			stack->addObject(pool, false); // ref = 1
		}
	}
}

void PoolManager::pop(void)
{
	Array * stack = getCurrentStack();
	SCAssert(stack, "could not happen");
	if (stack)
	{
		AutoreleasePool * pool = (AutoreleasePool *)stack->lastObject();
		SCAssert(pool, "Current autorelease pool should not be null");
		if (pool)
		{
			pool->clear();
			stack->removeObject(pool); // pool.ref = 0
		}
	}
}

NAMESPACE_END
