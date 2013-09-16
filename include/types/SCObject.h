//
//  SCObject.h
//  SnowCat
//
//  Created by Moky on 13-8-7.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCObject_h
#define SnowCat_SCObject_h

#include <typeinfo>
#include "scMacros.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class IObject
{
public:
	virtual const char * className(void) const = 0;
	virtual const char * description(void) const = 0;
	
	// deep copy!!
	virtual IObject * copy(void) const = 0;
	
	virtual void retain(void) = 0;
	virtual void release(void) = 0;
	virtual IObject * autorelease(void) = 0;
};

class Object : public IObject
{
public:
	// when the object is created, the refrence count of it is 1
	Object(void) : m_uiRefrence(1) {}
	virtual ~Object(void) {}
	virtual const char * className(void) const { return typeid(*this).name(); }
	
	virtual const char * description(void) const;
	
	virtual Object * copy(void) const;
	
	inline void retain(void) {
		SCAssert(m_uiRefrence > 0, "reference count should greater than 0");
		++m_uiRefrence;
	}
	
	inline void release(void) {
		SCAssert(m_uiRefrence > 0, "reference count should greater than 0");
		--m_uiRefrence;
		if (m_uiRefrence == 0) delete this;
	}
	
	Object * autorelease(void);
	
	inline unsigned int retainCount(void) const { return m_uiRefrence; }
	
private:
	unsigned int m_uiRefrence; // count of refrence
	
	friend class AutoreleasePool;
};

template <class T>
class ObjectContainer : public Object
{
public:
	ObjectContainer(T * obj) : object(obj) {}
	virtual ~ObjectContainer(void) {
		if (object) delete object;
		object = 0;
	}
	
	static inline ObjectContainer * container(T * obj) {
		ObjectContainer * oc = new ObjectContainer(obj);
		SCAssert(oc, "Not enough memory");
		if (oc) oc->autorelease();
		return oc;
	}
	
	T * object;
};

NAMESPACE_END

#endif
