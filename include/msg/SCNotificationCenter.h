//
//  SCNotificationCenter.h
//  SnowCat
//
//  Created by Moky on 13-9-16.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCNotificationCenter_h
#define SnowCat_SCNotificationCenter_h

#include "SCNotification.h"
#include "SCNotificationObserver.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class NotificationCenter: public Object
{
public:
	NotificationCenter(void);
	virtual ~NotificationCenter(void);
	
	static NotificationCenter * defaultCenter(void);
	
	void addObserver(IObject * target, NotificationHandler selector, const std::string & name = "*");
	void removeObserver(IObject * target, const std::string & name = "*");
	
	void postNotification(const Notification & notice) const;
	inline void postNotification(const std::string & name, IObject * sender = NULL) const {
		postNotification(Notification(name, sender));
	}
	
private:
	Dictionary * m_pObserverLists;
};

NAMESPACE_END

#endif
