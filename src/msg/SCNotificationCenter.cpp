//
//  SCNotificationCenter.cpp
//  SnowCat
//
//  Created by Moky on 13-9-16.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCArray.h"
#include "SCNotificationCenter.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

NotificationCenter::NotificationCenter(void)
: Object()
{
	m_pObserverLists = new Dictionary();
}

NotificationCenter::~NotificationCenter(void)
{
	if (m_pObserverLists)
	{
		delete m_pObserverLists;
		m_pObserverLists = NULL;
	}
}

static NotificationCenter * s_pDefaultNotificationCenter = new NotificationCenter();

NotificationCenter * NotificationCenter::defaultCenter(void)
{
	return s_pDefaultNotificationCenter;
}

void NotificationCenter::purgeCenter(void)
{
	BaseArray<std::string> emptyLists = BaseArray<std::string>();
	
	std::string key;
	IObject * obj;
	SC_DICTIONARY_FOREACH_REVERSE(m_pObserverLists, key, obj)
	{
		if (((Array *)obj)->empty())
		{
			emptyLists.add(key);
		}
	}
	
	if (!emptyLists.empty())
	{
		m_pObserverLists->removeObjectsForKeys(emptyLists);
	}
}

void NotificationCenter::postNotification(const Notification & notice) const
{
	IObject * obj;
	NotificationObserver * nob;
	Array * pList;
	
	// "name"
	pList = (Array *)m_pObserverLists->objectForKey(notice.getName());
	SC_ARRAY_FOREACH_REVERSE(pList, obj)
	{
		nob = (NotificationObserver *)obj;
		nob->perform(notice);
	}
	
	// "*"
	pList = (Array *)m_pObserverLists->objectForKey("*");
	SC_ARRAY_FOREACH_REVERSE(pList, obj)
	{
		nob = (NotificationObserver *)obj;
		nob->perform(notice);
	}
}

#pragma mark -

static inline bool isEqual(NotificationObserver * observer, IObject * obj)
{
	return *observer == *(NotificationObserver *)obj;
}

void NotificationCenter::addObserver(NotificationObserver * observer, const std::string & key)
{
	if (!observer || containsObserver(observer, key))
	{
		return;
	}
	
	Array * pList = (Array *)m_pObserverLists->objectForKey(key);
	if (!pList)
	{
		pList = new Array();
		m_pObserverLists->setObject(pList, key, false); // REF = 1
	}
	pList->addObject(observer);
}

void NotificationCenter::removeObserver(NotificationObserver * observer, const std::string & key)
{
	if (!observer)
	{
		return;
	}
	
	std::string k;
	IObject * obj;
	
	Array * list;
	
	SC_DICTIONARY_FOREACH_REVERSE(m_pObserverLists, k, obj)
	{
		if (key != "*" && key != k)
		{
			continue;
		}
		list = (Array *)obj;
		SC_ARRAY_FOREACH_REVERSE(list, obj)
		{
			if (isEqual(observer, obj))
			{
				list->removeObject(obj);
			}
		}
	}
}

bool NotificationCenter::containsObserver(NotificationObserver * observer, const std::string & key) const
{
	if (!observer)
	{
		return false;
	}
	
	std::string k;
	IObject * obj;
	
	Array * list;
	
	SC_DICTIONARY_FOREACH_REVERSE(m_pObserverLists, k, obj)
	{
		if (key != "*" && key != k)
		{
			continue;
		}
		list = (Array *)obj;
		SC_ARRAY_FOREACH_REVERSE(list, obj)
		{
			if (isEqual(observer, obj))
			{
				return true;
			}
		}
	}
	return false;
}

#pragma mark function handler

void NotificationCenter::addObserver(NotificationObserver::FunctionHandler function, const std::string & key)
{
	if (!function || containsObserver(function, key))
	{
		return;
	}
	NotificationObserver * observer = new NotificationFunctionObserver(function);
	addObserver(observer, key);
	observer->release();
}
void NotificationCenter::removeObserver(NotificationObserver::FunctionHandler function, const std::string & key)
{
	if (!function)
	{
		return;
	}
	NotificationObserver * observer = new NotificationFunctionObserver(function);
	removeObserver(observer, key);
	observer->release();
}
bool NotificationCenter::containsObserver(NotificationObserver::FunctionHandler function, const std::string & key) const
{
	NotificationObserver * observer = new NotificationFunctionObserver(function);
	bool has = containsObserver(observer, key);
	observer->release();
	return has;
}

#pragma mark object handler

void NotificationCenter::addObserver(IObject * target, NotificationObserver::ObjectHandler selector, const std::string & key)
{
	if (!target || containsObserver(target, selector, key))
	{
		return;
	}
	NotificationObserver * observer = new NotificationObjectObserver(target, selector);
	addObserver(observer, key);
	observer->release();
}
void NotificationCenter::removeObserver(IObject * target, NotificationObserver::ObjectHandler selector, const std::string & key)
{
	if (!target)
	{
		return;
	}
	NotificationObserver * observer = new NotificationObjectObserver(target, selector);
	removeObserver(observer, key);
	observer->release();
}
bool NotificationCenter::containsObserver(IObject * target, NotificationObserver::ObjectHandler selector, const std::string & key) const
{
	NotificationObserver * observer = new NotificationObjectObserver(target, selector);
	bool has = containsObserver(observer, key);
	observer->release();
	return has;
}

#pragma mark delegate handler

void NotificationCenter::addObserver(NotificationDelegate * delegate, const std::string & key)
{
	if (!delegate || containsObserver(delegate, key))
	{
		return;
	}
	NotificationObserver * observer = new NotificationDelegateObserver(delegate);
	addObserver(observer, key);
	observer->release();
}
void NotificationCenter::removeObserver(NotificationDelegate * delegate, const std::string & key)
{
	if (!delegate)
	{
		return;
	}
	NotificationObserver * observer = new NotificationDelegateObserver(delegate);
	removeObserver(observer, key);
	observer->release();
}
bool NotificationCenter::containsObserver(NotificationDelegate * delegate, const std::string & key) const
{
	NotificationObserver * observer = new NotificationDelegateObserver(delegate);
	bool has = containsObserver(observer, key);
	observer->release();
	return has;
}

NAMESPACE_END
