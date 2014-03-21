//
//  SCNotificationObserver.cpp
//  SnowCat
//
//  Created by Moky on 13-9-27.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCNotificationObserver.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

#pragma mark function observer

NotificationFunctionObserver::NotificationFunctionObserver(FunctionHandler handler)
: NotificationObserver()
, m_pfnFunction(handler)
{
	
}

NotificationFunctionObserver::~NotificationFunctionObserver(void)
{
	m_pfnFunction = NULL;
}

void NotificationFunctionObserver::perform(const Notification & notice) const
{
	if (m_pfnFunction)
	{
		m_pfnFunction(notice);
	}
}

bool NotificationFunctionObserver::operator == (const NotificationObserver & other) const
{
	if (!this->matchType(other))
	{
		return false;
	}
	NotificationFunctionObserver * observer = (NotificationFunctionObserver *)&other;
	if (m_pfnFunction && observer->m_pfnFunction)
	{
		return m_pfnFunction == observer->m_pfnFunction;
	}
	SCError("could not happen");
	return false;
}

#pragma mark object observer

NotificationObjectObserver::NotificationObjectObserver(IObject * target, ObjectHandler selector)
: NotificationObserver()
, m_pTarget(target)
, m_pfnSelector(selector)
{
	if (m_pTarget)
	{
		m_pTarget->retain();
	}
}

NotificationObjectObserver::~NotificationObjectObserver(void)
{
	if (m_pTarget)
	{
		m_pTarget->release();
		m_pTarget = NULL;
	}
	m_pfnSelector = NULL;
}

void NotificationObjectObserver::perform(const Notification & notice) const
{
	if (m_pTarget && m_pfnSelector)
	{
		(m_pTarget->*m_pfnSelector)(notice);
	}
}

bool NotificationObjectObserver::operator == (const NotificationObserver & other) const
{
	if (!this->matchType(other))
	{
		return false;
	}
	NotificationObjectObserver * observer = (NotificationObjectObserver *)&other;
	if (m_pTarget && observer->m_pTarget)
	{
		if (m_pfnSelector && observer->m_pfnSelector && m_pfnSelector != observer->m_pfnSelector)
		{
			return false;
		}
		return m_pTarget == observer->m_pTarget;
	}
	SCError("could not happen");
	return false;
}

#pragma mark delegate observer

NotificationDelegateObserver::NotificationDelegateObserver(NotificationDelegate * delegate)
: NotificationObserver()
, m_pDelegate(delegate)
{
	
}

NotificationDelegateObserver::~NotificationDelegateObserver(void)
{
	m_pDelegate = NULL;
}

void NotificationDelegateObserver::perform(const Notification & notice) const
{
	if (m_pDelegate)
	{
		m_pDelegate->onNotification(notice);
	}
}

bool NotificationDelegateObserver::operator == (const NotificationObserver & other) const
{
	if (!this->matchType(other))
	{
		return false;
	}
	NotificationDelegateObserver * observer = (NotificationDelegateObserver *)&other;
	if (m_pDelegate && observer->m_pDelegate)
	{
		return m_pDelegate == observer->m_pDelegate;
	}
	SCError("could not happen");
	return false;
}

NAMESPACE_END
