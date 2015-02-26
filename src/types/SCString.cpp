//
//  SCString.cpp
//  SnowCat
//
//  Created by Moky on 13-8-7.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCString.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

String::String(void)
: Object()
, std::string("")
{
	
}

String::String(const char * psz)
: Object()
, std::string(psz)
{
	
}

String::String(const std::string & string)
: Object()
, std::string(string)
{
	
}

String::~String(void)
{
	
}

const char * String::description(void) const
{
	String * string = new String();
	SCAssert(string, "Not enough memory");
	StringUtils::stringWithFormat(*string, "%s len: %d, value: \"%s\"",
								  Object::description(),
								  this->length(), this->c_str());
	string->autorelease();
	return string->c_str();
}

NAMESPACE_END
