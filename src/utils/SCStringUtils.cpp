//
//  SCStringUtils.cpp
//  SnowCat
//
//  Created by Moky on 13-8-6.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCStringUtils.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

std::string & StringUtils::stringWithFormat(std::string &string, const std::string & format, ...)
{
	va_list ap;
    va_start(ap, format);
	
    char * pBuf = (char *)malloc(1024 * 64);
	SCAssert(pBuf, "Not enough memory");
    if (pBuf)
    {
		bzero(pBuf, 1024 * 64);
        vsnprintf(pBuf, 1024 * 64, format.c_str(), ap);
        string = pBuf;
        free(pBuf);
    }
	
    va_end(ap);
	return string;
}

bool StringUtils::isNumeric(const std::string & string)
{
	int len = string.length();
	int cnt = 0;
	for (int i = len - 1; i >= 0; --i)
	{
		if (string[i] == '-')
		{
			if (i > 0 || len == 1)
			{
				return false;
			}
		}
		else if (string[i] == '.')
		{
			if (++cnt > 1)
			{
				return false;
			}
		}
		else if ((string[i] < '0' || string[i] > '9'))
		{
			return false;
		}
	}
	return true;
}

bool StringUtils::isInteger(const std::string & string)
{
	int len = string.length();
	for (int i = len - 1; i >= 0; --i)
	{
		if (string[i] == '-')
		{
			if (i > 0 || len == 1)
			{
				return false;
			}
		}
		else if ((string[i] < '0' || string[i] > '9'))
		{
			return false;
		}
	}
	return true;
}

NAMESPACE_END
