//
//  SCNotificationObserver.h
//  SnowCat
//
//  Created by Moky on 13-9-16.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCNotificationObserver_h
#define SnowCat_SCNotificationObserver_h

#include "SCNotification.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class NotificationObserver : public Object {
public:
	typedef void (*FunctionHandler)(const Notification &);
	typedef void (IObject::*ObjectHandler)(const Notification &);
	
	virtual void perform(const Notification & notice) const = 0;
	
	virtual bool operator == (const NotificationObserver & other) const = 0;
	inline bool operator != (const NotificationObserver & other) const {
		return !(*this == other);
	}
	
protected:
	typedef enum {
		FUNCTION = 1,
		OBJECT   = 2,
		DELEGATE = 3,
	} type;
	
	virtual type getType(void) const = 0;
	inline bool matchType(const NotificationObserver & other) const {
		return other.getType() == this->getType();
	}
};

#pragma mark function observer

class NotificationFunctionObserver : public NotificationObserver {
public:
	NotificationFunctionObserver(FunctionHandler handler);
	virtual ~NotificationFunctionObserver(void);
	
	virtual void perform(const Notification & notice) const;
	
	virtual bool operator == (const NotificationObserver & other) const;
	
protected:
	virtual type getType(void) const { return FUNCTION; }
	
private:
	FunctionHandler m_pfnFunction;
};

#pragma mark object observer

class NotificationObjectObserver : public NotificationObserver {
public:
	NotificationObjectObserver(IObject * target, ObjectHandler selector = NULL);
	virtual ~NotificationObjectObserver(void);
	
	virtual void perform(const Notification & notice) const;
	
	virtual bool operator == (const NotificationObserver & other) const;
	
protected:
	virtual type getType(void) const { return OBJECT; }
	
private:
	IObject * m_pTarget;
	ObjectHandler m_pfnSelector;
};

#pragma mark delegate observer

class NotificationDelegateObserver : public NotificationObserver
{
public:
	NotificationDelegateObserver(NotificationDelegate * delegate);
	virtual ~NotificationDelegateObserver(void);
	
	virtual void perform(const Notification & notice) const;
	
	virtual bool operator == (const NotificationObserver & other) const;
	
protected:
	virtual type getType(void) const { return DELEGATE; }
	
private:
	NotificationDelegate * m_pDelegate;
};

NAMESPACE_END

#endif
