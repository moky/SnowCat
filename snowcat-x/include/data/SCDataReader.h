//
//  SCDataReader.h
//  SnowCat
//
//  Created by Moky on 13-8-9.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCDataReader_h
#define SnowCat_SCDataReader_h

#include "SCData.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class Object;
class String;
class Array;
class Dictionary;

class DataReader : public Data
{
	typedef Data super;
	
public:
	DataReader(void);
	DataReader(const char * pszFilename);
	virtual ~DataReader(void);
	virtual bool init(void);
	
	Object * getRootObject(void);
	
protected:
	void reset(void);
	DataItem * read(void);
	
	String * getString(const unsigned int iLocation) const;
	
private:
	Object * parseRoot(void);
	
	Object * parseItem(const DataItem * pItem);
	
	Array * parseArrayItem(const DataItem * pItem);
	Dictionary * parseDictionaryItem(const DataItem * pItem);
	
private:
	Array * m_pStrings;   // strings array
	Object * m_pRootObject; // root object
	
	unsigned int m_iItemIndex;
};

NAMESPACE_END

#endif
