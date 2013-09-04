//
//  SCClient.h
//  SnowCat
//
//  Created by Moky on 13-8-12.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCClient_h
#define SnowCat_SCClient_h

#include <string>
#include "scMacros.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class Client
{
	// screen
	unsigned int m_uiScreenWidth;
	unsigned int m_uiScreenHeight;
	// client info
	std::string m_sHardware;         // "iPad2,1"
	std::string m_sDeviceIdentifier; // "C1898E26-2F6B-50CC-9221-9350CF40DFDE"
	std::string m_sDeviceModel;      // "iPad"
	std::string m_sSystemName;       // "iPhone_OS"
	std::string m_sSystemVersion;    // "5.1.1"
	// bundle version
	std::string m_sVersion;          // "1.0"
	
	std::string m_sClientParameters;
	void updateClientParameters(void);
	
#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)
	std::string m_sPackageName;
public:
	inline const char * getPackageName(void) const {
		return m_sPackageName.c_str();
	}
	void setInfo(const std::string & name, const std::string & value);
#endif
	
public:
	Client(void);
	virtual ~Client(void);
	virtual bool init(void);
	
	static Client * currentClient(void);
	
	inline unsigned int getScreenWidth(void) const {
		return m_uiScreenWidth;
	}
	inline unsigned int getScreenHeight(void) const {
		return m_uiScreenHeight;
	}
	
	inline const char * getHardware(void) const {
		return m_sHardware.c_str();
	}
	inline const char * getDeviceIdentifier(void) const {
		return m_sDeviceIdentifier.c_str();
	}
	inline const char * getDeviceModel(void) const {
		return m_sDeviceModel.c_str();
	}
	inline const char * getSystemName(void) const {
		return m_sSystemName.c_str();
	}
	inline const char * getSystemVersion(void) const {
		return m_sSystemVersion.c_str();
	}
	
	// client version from bundle
	inline const char * getVersion(void) const {
		return m_sVersion.c_str();
	}
	
	// for HTTP Request
	inline const char * getClientParameters(void) const {
		return m_sClientParameters.c_str();
	}
	
#pragma mark Network
	
	bool isConnected(void) const;
	bool isWiFiActived(void) const;
	
	bool openURL(const char * pszURL) const;
};

NAMESPACE_END

#endif
