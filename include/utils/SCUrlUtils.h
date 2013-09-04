//
//  SCUrlUtils.h
//  SnowCat
//
//  Created by Moky on 13-8-6.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCUrlUtils_h
#define SnowCat_SCUrlUtils_h

#include "SCStringUtils.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class UrlUtils : public StringUtils
{
public:
	static std::string & escape(std::string & string);
	static std::string & unescape(std::string & string);
	
	static std::string & addParameter(std::string & url, const std::string & key, const std::string & value);
	static std::string & addClientParameters(std::string & url);
	
	static inline bool isDynamic(const std::string & url) {
		return url.rfind("cgi-bin") != std::string::npos;
	}
	
#pragma mark Path
	
	static inline bool isAbsolutePath(const std::string & string) {
		return hasPrefix(string, "/") || hasPrefix(string, "file://");
	}
	
	/**
	 *  Cut down substring '../' and the prefix directory name at the same time.
	 */
	static std::string & trim(std::string & string);
	
	static inline std::string & scheme(std::string & string) {
		int pos = string.find("://");
		string = (pos == std::string::npos) ? "" : string.substr(0, pos);
		return string;
	}
	
	static inline std::string & fragment(std::string & string) {
		int pos = string.find("#");
		string = (pos == std::string::npos) ? "" : string.substr(pos + 1);
		return string;
	}
	
	static inline std::string & filename(std::string & string) {
		int pos = string.rfind('/');
		if (pos != std::string::npos) string = string.substr(pos + 1);
		return string;
	}
	
	static inline std::string & extension(std::string & string) {
		int pos = string.rfind('.');
		string = (pos == std::string::npos) ? "" : string.substr(pos + 1);
		return string;
	}
	
	static inline std::string & appendExtension(std::string & string, const std::string & ext) {
		int len1 = string.length(), len2 = ext.length();
		string += (len1 > 0 && string[len1 - 1] != '.' && len2 > 0 && ext[0] != '.') ? "." + ext : ext;
		return string;
	}
	
	static inline std::string & deleteExtension(std::string & string) {
		int pos = string.rfind('.');
		if (pos != std::string::npos) string = string.substr(0, pos);
		return string;
	}
	
	static inline std::string & appendPathComponent(std::string & string, const std::string & name) {
		int len1 = string.length(), len2 = name.length();
		string += (len1 > 0 && string[len1 - 1] != '/' && len2 > 0 && name[0] != '/') ? "/" + name : name;
		return string;
	}
	
	static inline std::string & deleteLastPathComponent(std::string & string) {
		int pos = string.rfind('/');
		string = (pos == std::string::npos) ? "" : string.substr(0, pos);
		return string;
	}
	
	static inline std::string & deleteFragment(std::string & string) {
		int pos = string.find("#");
		if (pos != std::string::npos) string = string.substr(0, pos);
		return string;
	}
};

NAMESPACE_END

#endif
