//
//  SCClient.cpp
//  SnowCat
//
//  Created by Moky on 13-8-12.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "SCLog.h"
#include "SCUrl.h"
#include "SCClient.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

Client::Client(void)
{
	init();
}

Client::~Client(void)
{
	
}

#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)

bool Client::init(void)
{
	// screen
	m_uiScreenWidth      = 1024;
	m_uiScreenHeight     = 768;
	// client info
	m_sHardware         = ""; // iPad2,1
	m_sDeviceIdentifier = ""; // C1898E26-2F6B-50CC-9221-9350CF40DFDE
	m_sDeviceModel      = ""; // iPad
	m_sSystemName       = ""; // iPhone_OS
	m_sSystemVersion    = ""; // 5.1.1
	// bundle version
	m_sVersion          = "1.0";
	
	#ifdef getPackageNameJNI
	m_sPackageName = getPackageNameJNI();
	#else
	m_sPackageName = "com.beva.client";
	#endif
	return true;
}

void Client::setInfo(const std::string & name, const std::string & value)
{
	if (name.empty())
	{
		SCError("key is empty");
	}
	// screen
	else if (strcasecmp(name.c_str(), "ScreenWidth") == 0)
	{
		m_uiScreenWidth = atoi(value.c_str());
	}
	else if (strcasecmp(name.c_str(), "ScreenHeight") == 0)
	{
		m_uiScreenHeight = atoi(value.c_str());
	}
	// client info
	else if (strcasecmp(name.c_str(), "Hardware") == 0)
	{
		m_sHardware = value;
	}
	else if (strcasecmp(name.c_str(), "IMEI") == 0)
	{
		m_sDeviceIdentifier = value;
	}
	else if (strcasecmp(name.c_str(), "Model") == 0)
	{
		m_sDeviceModel = value;
	}
	else if (strcasecmp(name.c_str(), "SystemName") == 0)
	{
		m_sSystemName = value;
	}
	else if (strcasecmp(name.c_str(), "SystemVersion") == 0)
	{
		m_sSystemVersion = value;
	}
	// bundle version
	else if (strcasecmp(name.c_str(), "Version") == 0)
	{
		m_sVersion = value;
	}
	updateClientParameters();
}

#include <jni.h>
//#include "android/jni/JniHelper.h"

extern "C"
{
	void Java_com_slanissue_bevatingx_bevatingx_nativeSetClientInfo(JNIEnv*  env, jobject thiz, jstring clientInfo)
	{
		const char * str = env->GetStringUTFChars(clientInfo, NULL);
		std::string string = str ? str : "";
		env->ReleaseStringUTFChars(clientInfo, str);
		
		SCLog("clientInfo: %s", string.c_str());
		
		std::size_t pos = string.find("=");
		if (pos == std::string::npos)
		{
			SCError("error");
			return;
		}
		std::string key = string.substr(0, pos);
		std::string value = string.substr(pos + 1);
		StringUtils::trim(key);
		StringUtils::trim(value);
		
		SCLog("key: %s, value: %s", key.c_str(), value.c_str());
		Client::currentClient()->setInfo(key, value);
	}
}

bool Client_openURL(const std::string & url)
{
	// TODO: implement for Android
	SCWarning("not implement");
	return true;
}

#else

extern bool Client_openURL(const std::string & url);

#endif // #if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)


static Client * s_pCurrentClient = new Client();

Client * Client::currentClient(void)
{
	return s_pCurrentClient;
}

void Client::updateClientParameters(void)
{
	std::string string = "";
	// client-version
	string.append("client-version=").append(getVersion());
	
	// screen
	char screen[32];
	memset(screen, 0, 32);
	snprintf(screen, 32, "%dx%d", getScreenWidth(), getScreenHeight());
	string.append("&screen=").append(screen);
	
	// os
	std::string os = std::string(getSystemName()).append("/").append(getSystemVersion());
	UrlUtils::escape(os);
	string.append("&os=").append(os);
	
	// device
	std::string device = std::string(getDeviceModel()).append("/").append(getHardware());
	UrlUtils::escape(device);
	string.append("&device=").append(device);
	
	// imei
	string.append("&imei=").append(getDeviceIdentifier());
	
	m_sClientParameters = string;
}

#pragma mark Network

static int checkSocket(int socket, const char * name)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifreq));
	strcpy(ifr.ifr_name, name);
	return ioctl(socket, SIOCGIFADDR, &ifr);
}

bool Client::isConnected(void) const
{
	SCLog("checking network...");
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		SCError("not connect");
		return false;
	}
	return false;
}

bool Client::isWiFiActived(void) const
{
	SCLog("checking wifi...");
	bool flag = false;
	
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		SCLog("not connect");
		return false;
	}
	
	std::string ss[] = {"en0", "en1", "wlan0", "wlan1", "eth0", "eth1"};
	int res;
	for (int i = 0; i < 6; ++i)
	{
		res = checkSocket(fd, ss[i].c_str());
		if (res != -1)
		{
			SCLog("found at %s", ss[i].c_str());
			flag = true;
			break;
		}
	}
	
	close(fd);
	return flag;
}

bool Client::openURL(const std::string & url) const
{
	if (url.empty())
	{
		SCError("error");
		return false;
	}
	
	URL tmp(url);
	if (tmp.isDynamic())
	{
		tmp.addClientParameters();
	}
	
	// trying
	if (Client_openURL(tmp))
	{
		return true;
	}
	
	// cannot open, try the next fragment
	std::size_t pos = url.find("#");
	if (pos == std::string::npos)
	{
		SCLog("cannot open url: %s", url.c_str());
		return false;
	}
	
	return openURL(url.substr(pos + 1));
}

bool Client::openURL(const std::string & url, Rect & frame) const
{
	std::size_t pos = url.find(":");
	if (pos == std::string::npos)
	{
		SCError("invalid scheme: %s", url.c_str());
		return false;
	}
	
	URL tmp(url);
	if (tmp.isDynamic())
	{
		tmp.addClientParameters();
	}
	
	if (frame.size.width <= 0)
	{
		frame.size.width = getScreenWidth();
	}
	if (frame.size.height <= 0)
	{
		frame.size.width = getScreenHeight();
	}
	
	// TODO: implement here for both iOS and Android
	SCWarning("not implement");
	return true;
}

NAMESPACE_END
