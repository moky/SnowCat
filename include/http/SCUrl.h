//
//  SCUrl.h
//  SnowCat
//
//  Created by Moky on 13-8-6.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCUrl_h
#define SnowCat_SCUrl_h

#include "SCString.h"
#include "SCUrlUtils.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class URL : public String
{
public:
	URL(const std::string &string) : String(string) {}
	virtual ~URL(void) {}
	
	inline URL & operator = (const char * psz) {
		this->assign(psz);
		return *this;
	}
	inline URL & operator = (const std::string & string) {
		this->assign(string);
		return *this;
	}
	
#pragma mark Parameters
	
	inline URL & addParameter(const std::string & key, const std::string & value) {
		return (*this = UrlUtils::addParameter(*this, key, value));
	}
	inline URL & addClientParameters(void) {
		return (*this = UrlUtils::addClientParameters(*this));
	}
	
	inline bool isDynamic(void) const {
		return UrlUtils::isDynamic(*this);
	}
	
#pragma mark Path
	
	inline bool isAbsolutePath(void) const {
		return UrlUtils::isAbsolutePath(*this);
	}
	inline URL & trim(void) {
		return (*this = UrlUtils::trim(*this));
	}
	
	inline URL & appendExtension(const std::string & ext) {
		return (*this = UrlUtils::appendExtension(*this, ext));
	}
	inline URL & deleteExtension(void) {
		return (*this = UrlUtils::deleteExtension(*this));
	}
	
	inline URL & appendPathComponent(const std::string & name) {
		return (*this = UrlUtils::appendPathComponent(*this, name));
	}
	inline URL & deleteLastPathComponent(void) {
		return (*this = UrlUtils::deleteLastPathComponent(*this));
	}
	
	inline URL & deleteFragment(void) {
		return (*this = UrlUtils::deleteFragment(*this));
	}
};

NAMESPACE_END

#endif
