//
//  SCString.h
//  SnowCat
//
//  Created by Moky on 13-8-6.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCString_h
#define SnowCat_SCString_h

#include "SCStringUtils.h"
#include "SCObject.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class String : public Object, public std::string
{
public:
	String(void);
	String(const char * psz);
	String(const std::string & string);
	virtual ~String(void);
	
	inline String & operator = (const char * psz) {
		this->assign(psz);
		return *this;
	}
	inline String & operator = (const std::string & string) {
		this->assign(string);
		return *this;
	}
	
	virtual const char * description(void) const;
	
	virtual String * copy(void) const {
		return new String(*this);
	}
	
	static inline String * string(void) {
		return (String *)(new String())->autorelease();
	}
	static inline String * string(const std::string & other) {
		return (String *)(new String(other))->autorelease();
	}
	
#pragma mark Numeric
	inline bool isNumeric(void) const {
		return StringUtils::isNumeric(*this);
	}
	inline bool isInteger(void) const {
		return StringUtils::isInteger(*this);
	}
	inline bool isFloat(void) const {
		return StringUtils::isFloat(*this);
	}
	
	inline int intValue(void) const {
		return StringUtils::intValue(*this);
	}
	inline double floatValue(void) const {
		return StringUtils::floatValue(*this);
	}
	
#pragma mark Trim
	inline String & ltrim(const std::string & pattern) {
		return (*this = StringUtils::ltrim(*this, pattern));
	}
	inline String & rtrim(const std::string & pattern) {
		return (*this = StringUtils::rtrim(*this, pattern));
	}
	inline String & trim(const std::string & pattern) {
		return (*this = StringUtils::trim(*this, pattern));
	}
	
	inline String & ltrim(void) {
		return (*this = StringUtils::ltrim(*this));
	}
	inline String & rtrim(void) {
		return (*this = StringUtils::rtrim(*this));
	}
	inline String & trim(void) {
		return (*this = StringUtils::trim(*this));
	}
	
#pragma mark Case
	inline String & lowercaseString(void) {
		return (*this = StringUtils::lowercaseString(*this));
	}
	inline String & uppercaseString(void) {
		return (*this = StringUtils::uppercaseString(*this));
	}
	
#pragma mark Prefix & Suffix
	inline bool hasPrefix(const std::string & prefix) const {
		return StringUtils::hasPrefix(*this, prefix);
	}
	inline bool hasSuffix(const std::string & prefix) const {
		return StringUtils::hasSuffix(*this, prefix);
	}
};

NAMESPACE_END

#endif
