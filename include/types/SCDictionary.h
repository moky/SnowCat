//
//  SCDictionary.h
//  SnowCat
//
//  Created by Moky on 13-8-7.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCDictionary_h
#define SnowCat_SCDictionary_h

#include <string>
#include "SCObjectDictionary.h"
#include "SCObject.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

// foreach
#define SC_DICTIONARY_FOREACH(__dict__, __key__, __object__) \
		SC_OBJECT_DICTIONARY_FOREACH(__dict__, __key__, __object__, std::string)

// foreach_reverse
#define SC_DICTIONARY_FOREACH_REVERSE(__dict__, __key__, __object__) \
		SC_OBJECT_DICTIONARY_FOREACH_REVERSE(__dict__, __key__, __object__, std::string)

class Array;

class Dictionary : public Object, public ObjectDictionary<std::string>
{
public:
	virtual const char * description(void) const;
	
#pragma mark New dictionary
	
	virtual Dictionary * copy(void) const;
	
	static Dictionary * dictionary(void) {
		return (Dictionary *)(new Dictionary())->autorelease();
	}
	
	static Dictionary * dictionary(const Dictionary * other) {
		return (Dictionary *)(new Dictionary(*other))->autorelease();
	}
	
	static Dictionary * dictionary(const char * key1, IObject * value1, ...);
	
#pragma mark Query dictionary
	
	Array * allKeys(void) const;
};

NAMESPACE_END

#endif
