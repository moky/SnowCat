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
	
	/**
	 *  remove empty list
	 */
	void purgeCenter(void);
	
	// post notification
	void postNotification(const Notification & notice) const;
	inline void postNotification(const std::string & name, IObject * sender = NULL) const {
		postNotification(Notification(name, sender));
	}
	
#pragma mark -
	
	void addObserver(NotificationObserver * observer, const std::string & key = "*");
	void removeObserver(NotificationObserver * observer, const std::string & key = "*");
	bool containsObserver(NotificationObserver * observer, const std::string & key = "*") const;
	
#pragma mark function handler
	void addObserver(NotificationObserver::FunctionHandler function, const std::string & key = "*");
	void removeObserver(NotificationObserver::FunctionHandler function, const std::string & key = "*");
	bool containsObserver(NotificationObserver::FunctionHandler function, const std::string & key = "*") const;
	
#pragma mark object handler
	void addObserver(IObject * target, NotificationObserver::ObjectHandler selector, const std::string & key = "*");
	void removeObserver(IObject * target, NotificationObserver::ObjectHandler selector = NULL, const std::string & key = "*");
	bool containsObserver(IObject * target, NotificationObserver::ObjectHandler selector = NULL, const std::string & key = "*") const;
	
#pragma mark delegate handler
	void addObserver(NotificationDelegate * delegate, const std::string & key = "*");
	void removeObserver(NotificationDelegate * delegate, const std::string & key = "*");
	bool containsObserver(NotificationDelegate * delegate, const std::string & key = "*") const;
	
private:
	Dictionary * m_pObserverLists;
};

NAMESPACE_END

#endif
