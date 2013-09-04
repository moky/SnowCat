//
//  SCClient.mm
//  SnowCat
//
//  Created by Moky on 13-8-13.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include <sys/sysctl.h>

#include "IPAddress.h"

#include "SCClient.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

bool Client::init(void)
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	// screen
	CGFloat scale = 1.0f;
	UIScreen * screen = [UIScreen mainScreen];
	if ([screen respondsToSelector:@selector(scale)])
	{
		scale = [screen scale];
	}
	CGRect frame = [screen bounds];
	m_uiScreenWidth = frame.size.width * scale;
	m_uiScreenHeight = frame.size.height * scale;
	
	// client info
	{
		size_t size;
		sysctlbyname("hw.machine", NULL, &size, NULL, 0);
		char * machine = (char *)malloc(size);
		memset(machine, 0, size);
		sysctlbyname("hw.machine", machine, &size, NULL, 0);
		m_sHardware = machine;
		free(machine);
	}
	{
		//NSString * string = [[UIDevice currentDevice] uniqueIdentifier];
		//m_sDeviceIdentifier = [string cStringUsingEncoding:NSUTF8StringEncoding];
		
		InitAddresses();
		GetIPAddresses();
		GetHWAddresses();
		
		for (int i = 0; i < MAXADDRS; ++i)
		{
			static unsigned long localHost = 0x7F000001;            // 127.0.0.1
			unsigned long theAddr;
			
			theAddr = ip_addrs[i];
			
			if (theAddr == 0) break;
			if (theAddr == localHost) continue;
			
			//Name: lo0 MAC: 2535A38:815B400:24D8921:BFFFAE98:8157000:20 IP: 127.0.0.1
			//Name: en0 MAC: 0E:815B400:24D8921:BFFFAE98:8157000:20 IP: 192.168.1.91
			//SCLog("Name: %s MAC: %s IP: %s\n", if_names[i], hw_addrs[i], ip_names[i]);
			m_sDeviceIdentifier = hw_addrs[i];
			
			//decided what adapter do you want
			if (strncmp(if_names[i], "en", 2) == 0)
			{
				break;
			}
		}
		
		std::size_t pos;
		while ((pos = m_sDeviceIdentifier.find(":")) != std::string::npos)
		{
			m_sDeviceIdentifier = m_sDeviceIdentifier.substr(0, pos) + "-" + m_sDeviceIdentifier.substr(pos + 1);
		}
		
		FreeAddresses();
	}
	{
		NSString * string = [[UIDevice currentDevice] model];
		m_sDeviceModel = [string cStringUsingEncoding:NSUTF8StringEncoding];
	}
	{
		NSString * string = [[UIDevice currentDevice] systemName];
		m_sSystemName = [string cStringUsingEncoding:NSUTF8StringEncoding];
	}
	{
		NSString * string = [[UIDevice currentDevice] systemVersion];
		m_sSystemVersion = [string cStringUsingEncoding:NSUTF8StringEncoding];
	}
	// bundle version
	{
		NSString * string = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
		m_sVersion = [string cStringUsingEncoding:NSUTF8StringEncoding];
	}
	
	updateClientParameters();
	
	[pool release];
	
	return true;
}

NAMESPACE_END
