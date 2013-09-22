//
//  SCDictionary.cpp
//  SnowCat
//
//  Created by Moky on 13-8-7.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCString.h"
#include "SCArray.h"

#include "SCDictionary.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

const char * Dictionary::description(void) const
{
	String * string = new String();
	SCAssert(string, "Not enough memory");
	StringUtils::stringWithFormat(*string, "%s capacity: %u, count: %u {",
								  Object::description(),
								  this->max_size(), this->size());
	std::string key;
	IObject * obj;
	SC_DICTIONARY_FOREACH(this, key, obj)
	{
		string->append("\n\t").append(key).append(": ").append(obj->description());
	}
	string->append("\n}");
	
	string->autorelease();
	return string->c_str();
}

Dictionary * Dictionary::copy(void) const
{
	Dictionary * dict = new Dictionary();
	SCAssert(dict, "Not enough memory");
	IObject * obj;
	for (const_iterator iter = this->begin(); iter != this->end(); iter++)
	{
		if (iter->second == NULL)
		{
			SCWarning("empty value, key: %s", iter->first.c_str());
			continue;
		}
		obj = iter->second->copy();
		SCAssert(obj, "Not enough memory");
		if (obj)
		{
			dict->setObject(obj, iter->first, false); // REF = 1
		}
	}
	return dict;
}

Dictionary * Dictionary::dictionary(const char * key1, IObject *value1, ...)
{
	Dictionary * dict = new Dictionary();
	SCAssert(dict, "Not enough memory");
	
	va_list args;
	va_start(args, value1);
	while (key1 && strlen(key1) > 0 && value1)
	{
		dict->setObject(value1, key1);
		key1 = va_arg(args, const char *);
		value1 = va_arg(args, IObject *);
	}
	va_end(args);
	
	dict->autorelease();
	return dict;
}

#pragma mark Query a Dictionary

Array * Dictionary::allKeys(void) const
{
	Array * array = new Array();
	SCAssert(array, "Not enough memory");
	
	std::string key;
	IObject * obj;
	SC_DICTIONARY_FOREACH(this, key, obj)
	{
		if (!key.empty())
		{
			String * string = new String(key);
			SCAssert(string, "Not enough memory");
			if (string)
			{
				array->addObject(string);
				string->release();
			}
		}
	}
	
	return array;
}

NAMESPACE_END
