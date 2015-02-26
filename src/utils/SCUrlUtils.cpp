//
//  SCUrlUtils.cpp
//  SnowCat
//
//  Created by Moky on 13-8-6.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"

#include "SCUrlUtils.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

std::string & UrlUtils::escape(std::string &string)
{
	SCWarning("not implement");
	return string;
}

std::string & UrlUtils::unescape(std::string &string)
{
	SCWarning("not implement");
	return string;
}


std::string & UrlUtils::addParameter(std::string &url, const std::string &key, const std::string &value)
{
	std::string fragment("");
	std::size_t pos = url.find("#");
	if (pos != std::string::npos)
	{
		fragment = url.substr(pos);
		url = url.substr(0, pos);
	}
	
	// TODO: check whether the key exists...
	
	// TODO: escape value...
	
	pos = url.find("?");
	if (pos != std::string::npos)
	{
		url.append("&").append(key).append("=").append(value);
	}
	else
	{
		url.append("?").append(key).append("=").append(value);
	}
	
	url.append(fragment);
	return url;
}

std::string & UrlUtils::addClientParameters(std::string &url)
{
	if (url.rfind("client-version=") != std::string::npos)
	{
		SCWarning("client parameters has already added: %s", url.c_str());
		return url;
	}
	
	std::string fragment("");
	std::size_t pos = url.find("#");
	if (pos != std::string::npos)
	{
		fragment = url.substr(pos);
		url = url.substr(0, pos);
	}
	// append client stat parameters
	const char * pszClient = "client-version=";//CNClient::getClientParameters();
	pos = url.find("?");
	if (pos != std::string::npos)
	{
		url.append("&").append(pszClient);
	}
	else
	{
		url.append("?").append(pszClient);
	}
	
	if (!fragment.empty())
	{
		url.append(fragment);
	}
	
	return url;
}

std::string & UrlUtils::trim(std::string &string)
{
	StringUtils::trim(string);
	
	std::size_t pos1, pos2;
	while ((pos1 = string.find("/../")) != std::string::npos)
	{
		if (pos1 < 1 || string[pos1 - 1] == '/')
		{
			SCError("error: %s", string.c_str());
			break;
		}
		pos2 = string.rfind("/", pos1 - 1);
		if (pos2 == std::string::npos)
		{
			pos2 = - 1;
		}
		string = string.substr(0, pos2 + 1) + string.substr(pos1 + 4);
	}
	
	if (string.length() > 0 && string[string.length() - 1] == '/')
	{
		string = string.substr(0, string.length() - 1);
	}
	
	return string;
}

NAMESPACE_END
