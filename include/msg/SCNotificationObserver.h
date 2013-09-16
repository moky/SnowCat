//
//  SCNotificationObserver.h
//  SnowCat
//
//  Created by Moky on 13-9-16.
//  Copyright (c) 2013年 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCNotificationObserver_h
#define SnowCat_SCNotificationObserver_h

#include "SCNotification.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

typedef void (IObject::*NotificationHandler)(const Notification &);

class NotificationObserver : public Object
{
public:
	NotificationObserver(IObject * target, NotificationHandler handler) : Object() {
		m_pTarget = target;
		m_pfnSelector = handler;
	}
	virtual ~NotificationObserver(void) {
		m_pTarget = NULL;
		m_pfnSelector = NULL;
	}
	
	inline void perform(const Notification & notice) const {
		if (m_pTarget && m_pfnSelector) (m_pTarget->*m_pfnSelector)(notice);
	}
	
	// target
	inline IObject * getTarget(void) const {
		return m_pTarget;
	}
	
private:
	IObject * m_pTarget;
	NotificationHandler m_pfnSelector;
};

NAMESPACE_END

#endif
