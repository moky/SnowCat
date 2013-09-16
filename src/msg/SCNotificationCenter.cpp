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

static NotificationCenter * s_pDefaultNotificationCenter = NULL;

NotificationCenter * NotificationCenter::defaultCenter(void)
{
	if (!s_pDefaultNotificationCenter)
	{
		s_pDefaultNotificationCenter = new NotificationCenter();
	}
	return s_pDefaultNotificationCenter;
}

void NotificationCenter::addObserver(IObject * target, NotificationHandler selector, const std::string & name)
{
	SCAssert(target && selector, "parameters invalid");
	Array * pList = (Array *)m_pObserverLists->objectForKey(name);
	if (!pList)
	{
		pList = new Array();
		m_pObserverLists->setObject(pList, name, false); // REF = 1
	}
	else
	{
		IObject * obj;
		NotificationObserver * nob;
		SC_ARRAY_FOREACH_REVERSE(pList, obj)
		{
			nob = (NotificationObserver *)obj;
			if (target == nob->getTarget())
			{
				SCWarning("observer is exists for name: %s", name.c_str());
				return;
			}
		}
	}
	
	NotificationObserver * pObserver = new NotificationObserver(target, selector);
	pList->addObject(pObserver, false); // REF = 1
}

void NotificationCenter::removeObserver(IObject * target, const std::string & name)
{
	BaseArray<std::string> emptyLists = BaseArray<std::string>();
	
	std::string key;
	IObject * obj;
	NotificationObserver * nob;
	SC_DICTIONARY_FOREACH_REVERSE(m_pObserverLists, key, obj)
	{
		if (name != "*" && name != key)
		{
			continue;
		}
		Array * pList = (Array *)obj;
		
		SC_ARRAY_FOREACH_REVERSE(pList, obj)
		{
			nob = (NotificationObserver *)obj;
			if (target == nob->getTarget())
			{
				pList->removeObject(obj);
			}
		}
		
		if (pList->empty())
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
	std::string name = notice.getName();
	
	if (name == "*")
	{
		std::string key;
		IObject * obj;
		NotificationObserver * nob;
		SC_DICTIONARY_FOREACH_REVERSE(m_pObserverLists, key, obj)
		{
			Array * pList = (Array *)obj;
			SC_ARRAY_FOREACH_REVERSE(pList, obj)
			{
				nob = (NotificationObserver *)obj;
				nob->perform(notice);
			}
		}
	}
	else if (Array * pList = (Array *)m_pObserverLists->objectForKey(name))
	{
		IObject * obj;
		NotificationObserver * nob;
		SC_ARRAY_FOREACH_REVERSE(pList, obj)
		{
			nob = (NotificationObserver *)obj;
			nob->perform(notice);
		}
	}
}

NAMESPACE_END
