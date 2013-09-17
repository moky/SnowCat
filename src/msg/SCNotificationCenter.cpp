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

void NotificationCenter::addObserver(IObject * target, NotificationHandler selector, const std::string & name)
{
	SCAssert(target && selector, "parameters invalid");
	Array * pList = (Array *)m_pObserverLists->objectForKey(name);
	if (!pList)
	{
		pList = new Array();
		m_pObserverLists->setObject(pList, name, false); // REF = 1
	}
	
	NotificationObserver * pObserver = new NotificationObserver(target, selector);
	pList->addObject(pObserver, false); // REF = 1
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

NotificationObserver * NotificationCenter::locateObserver(const IObject * target, const std::string & name, const int method)
{
	std::string key;
	IObject * obj;
	Array * pList;
	NotificationObserver * nob;
	SC_DICTIONARY_FOREACH_REVERSE(m_pObserverLists, key, obj)
	{
		if (name != "*" && name != key)
		{
			continue;
		}
		pList = (Array *)obj;
		SC_ARRAY_FOREACH_REVERSE(pList, obj)
		{
			nob = (NotificationObserver *)obj;
			if (nob->getTarget() == target)
			{
				if (method == 0)
				{
					return nob;
				}
				else if (method == 1)
				{
					nob->retain();
					nob->autorelease();
					pList->removeObject(nob);
					return nob;
				}
				else if (method == -1)
				{
					pList->removeObject(nob);
				}
			}
		}
	}
	return NULL;
}

NAMESPACE_END
