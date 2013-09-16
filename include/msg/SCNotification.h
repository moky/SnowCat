//
//  SCNotification.h
//  SnowCat
//
//  Created by Moky on 13-9-16.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCNotification_h
#define SnowCat_SCNotification_h

#include <string>
#include "../types/SCObject.h"
#include "../types/SCDictionary.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class Notification : public Object
{
public:
	Notification(void) : Object(), m_sName(""), m_pSender(NULL), m_pUserInfo(NULL) {
	}
	Notification(const std::string & name, IObject * sender = NULL) : Object(), m_pUserInfo(NULL) {
		setName(name);
		setSender(sender);
	}
	virtual ~Notification(void) {
		setSender(NULL);
		setUserInfo(NULL);
	}
	
	// name
	inline void setName(const std::string & name) {
		m_sName = name;
	}
	inline const char * getName(void) const {
		return m_sName.c_str();
	}
	
	// sender
	inline void setSender(IObject * sender) {
		if (sender != m_pSender) {
			sender->retain();
			if (m_pSender) m_pSender->release();
			m_pSender = sender;
		}
	}
	inline IObject * getSender(void) const {
		return m_pSender;
	}
	
	// user info
	inline void setUserInfo(Dictionary * info) {
		if (info != m_pUserInfo) {
			info->retain();
			if (m_pUserInfo) m_pUserInfo->release();
			m_pUserInfo = info;
		}
	}
	inline Dictionary * getUserInfo(void) const {
		return m_pUserInfo;
	}
	inline void setUserInfo(const std::string & key, IObject * info) {
		if (!m_pUserInfo) m_pUserInfo = new Dictionary(); // REF = 1
		m_pUserInfo->setObject(info, key);
	}
	inline IObject * getUserInfo(const std::string & key) const {
		return m_pUserInfo ? m_pUserInfo->objectForKey(key) : NULL;
	}
	
private:
	std::string m_sName;
	IObject * m_pSender;
	Dictionary * m_pUserInfo;
};

NAMESPACE_END

#endif
