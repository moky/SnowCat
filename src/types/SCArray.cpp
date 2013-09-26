//
//  SCArray.cpp
//  SnowCat
//
//  Created by Moky on 13-8-7.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include <stdlib.h>
#include <string.h>

#include "SCLog.h"
#include "SCString.h"

#include "SCArray.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

const char * Array::description(void) const
{
	String * string = new String();
	SCAssert(string, "Not enough memory");
	StringUtils::stringWithFormat(*string, "%s capacity: %u, count: %u [",
								  Object::description(),
								  this->capacity(), this->size());
	IObject * obj;
	SC_ARRAY_FOREACH(this, obj)
	{
		string->append("\n\t").append(obj->description());
	}
	string->append("\n]");
	
	string->autorelease();
	return string->c_str();
}

Array * Array::copy(void) const
{
	Array * array = new Array();
	SCAssert(array, "Not enough memory");
	IObject * obj;
	for (const_iterator iter = this->begin(); iter != this->end(); ++iter)
	{
		if (*iter == NULL)
		{
			SCWarning("empty value");
			continue;
		}
		obj = (*iter)->copy();
		SCAssert(obj, "Not enough memory");
		if (obj)
		{
			array->addObject(obj, false); // REF = 1
		}
	}
	return array;
}

Array * Array::array(IObject *obj1, ...)
{
	Array * array = new Array();
	SCAssert(array, "Not enough memory");
	
	va_list args;
	va_start(args, obj1);
	while (obj1)
	{
		array->addObject(obj1);
		obj1 = va_arg(args, IObject *);
	}
	va_end(args);
	
	array->autorelease();
	return array;
}

Array * Array::randomArray(unsigned int count) const
{
	if (count == 0 || count > this->size())
	{
		count = this->size();
	}
	IObject * obj;
	
	// temp array
	Array * tmp = new Array();
	SCAssert(tmp, "Not enough memory");
	SC_ARRAY_FOREACH(this, obj)
	{
		tmp->addObject(obj, false);
	}
	
	// new array
	Array * ret = new Array();
	SCAssert(ret, "Not enough memory");
	
	srand((unsigned int)time(NULL));
	while (tmp->count())
	{
		unsigned int index = (unsigned int)(rand() % tmp->count());
		SCLog("pick an object at %d / %d", index, tmp->count());
		ret->addObject(tmp->objectAtIndex(index));
		tmp->removeObjectAtIndex(index, false);
	}
	tmp->release();
	
	ret->autorelease();
	return ret;
}

NAMESPACE_END
