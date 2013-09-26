//
//  SCStringUtils.h
//  SnowCat
//
//  Created by Moky on 13-8-6.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCStringUtils_h
#define SnowCat_SCStringUtils_h

#include <stdlib.h>
#include <string>

#include "MD5.h"

#include "scMacros.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class StringUtils
{
public:
	static std::string & stringWithFormat(std::string & string, const std::string & format, ...); 
	
#pragma mark Numeric
	
	/**
	 *    0123456789 || -0123456789 || .0123456789 || -.0123456789
	 */
	static bool isNumeric(const std::string & string);
	
	/**
	 *    0123456789 || -0123456789
	 */
	static bool isInteger(const std::string & string);
	
	/**
	 *    .0123456789 || -.0123456789
	 */
	static inline bool isFloat(const std::string & string) {
		return isNumeric(string) && (!isInteger(string));
	}
	
	static inline int intValue(const std::string & string) {
		return atoi(string.c_str());
	}
	
	static inline double floatValue(const std::string & string) {
		return atof(string.c_str());
	}
	
#pragma mark Trim
	
	static inline std::string & ltrim(std::string & string, const std::string & pattern) {
		std::size_t pos = string.find_first_not_of(pattern);
		string = (pos == std::string::npos) ? "" : string.substr(pos);
		return string;
	}
	
	static inline std::string & rtrim(std::string & string, const std::string & pattern) {
		std::size_t pos = string.find_last_not_of(pattern);
		string = (pos == std::string::npos) ? "" : string.substr(0, pos + 1);
		return string;
	}
	
	static inline std::string & trim(std::string & string, const std::string & pattern) {
		return rtrim(ltrim(string, pattern), pattern);
	}
	
	static inline std::string & ltrim(std::string & string) {
		return ltrim(string, " \n\r\t");
	}
	
	static inline std::string & rtrim(std::string & string) {
		return rtrim(string, " \n\r\t");
	}
	
	static inline std::string & trim(std::string & string) {
		return trim(string, " \n\r\t");
	}
	
#pragma mark Case
	
	static inline std::string & lowercaseString(std::string & string) {
		for (int i = string.length() - 1; i >= 0; --i)
			string[i] = tolower(string[i]);
		return string;
	}
	
	static inline std::string & uppercaseString(std::string & string) {
		for (int i = string.length() - 1; i >= 0; --i)
			string[i] = toupper(string[i]);
		return string;
	}
	
#pragma mark Prefix & Suffix
	
	static inline bool hasPrefix(const std::string & string, const std::string & prefix) {
		int len1 = string.length(), len2 = prefix.length();
		return len1 < len2 ? false : strcmp(string.substr(0, len2).c_str(), prefix.c_str()) == 0;
	}
	
	static inline bool hasSuffix(const std::string & string, const std::string & suffix) {
		int len1 = string.length(), len2 = suffix.length();
		return len1 < len2 ? false : strcmp(string.substr(len1 - len2).c_str(), suffix.c_str()) == 0;
	}
	
#pragma mark MD5
	
	static inline std::string & md5(std::string & string) {
		string = MD5(string).toString();
		return string;
	}
};

NAMESPACE_END

#endif
