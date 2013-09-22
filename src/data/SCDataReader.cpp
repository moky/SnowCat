//
//  SCDataReader.cpp
//  SnowCat
//
//  Created by Moky on 13-8-9.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCAutoreleasePool.h"
#include "SCObject.h"
#include "SCString.h"
#include "SCArray.h"
#include "SCDictionary.h"

#include "SCDataReader.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

static Array * getStringArrayFromBuffer(const unsigned char * pBuffer, const unsigned long iBufferLength)
{
	if (!pBuffer || iBufferLength == 0)
	{
		SCError("could not happen");
		return NULL;
	}
	
	const char * p = (const char *)pBuffer;
	unsigned long * pCount = (unsigned long *)p; // 0 - 4,294,967,295 (4G)
	p += sizeof(unsigned long);
	
	Array * array = new Array();
	//array->initWithCapacity(*pCount);
	char * str = new char[65536];
	
	unsigned short * pLen;
	for (int i = 0; i < *pCount; i++)
	{
		pLen = (unsigned short *)p; // 0 - 65535
		p += sizeof(unsigned short);
		
		memset(str, 0, 65536);
		snprintf(str, *pLen + 1, "%s", p); // would append '\0' to the end
		p += *pLen;
		
		String * pString = new String(str);
		array->addObject(pString);
		pString->release();
	}
	delete[] str;
	
	array->autorelease();
	return array;
}

DataReader::DataReader(void)
: Data()
, m_pStrings(NULL)
, m_pRootObject(NULL)
, m_iItemIndex(0)
{
	
}

DataReader::DataReader(const char * pszFilename)
: Data(pszFilename)
, m_pStrings(NULL)
, m_pRootObject(NULL)
, m_iItemIndex(0)
{
	
}

DataReader::~DataReader(void)
{
	if (m_pStrings)
	{
		m_pStrings->release();
		m_pStrings = NULL;
	}
	if (m_pRootObject)
	{
		m_pRootObject->release();
		m_pRootObject = NULL;
	}
}

bool DataReader::init(void)
{
	if (!super::init())
	{
		return false;
	}
	if (m_pStrings)
	{
		m_pStrings->release();
		m_pStrings = NULL;
	}
	if (m_pRootObject)
	{
		m_pRootObject->release();
		m_pRootObject = NULL;
	}
	m_iItemIndex = 0;
	return true;
}

Object * DataReader::getRootObject(void)
{
	if (m_pRootObject)
	{
		return m_pRootObject; // already parsed
	}
	
	// init string array
	unsigned long iStringsBufferLength = 0;
	unsigned char * pStringsBuffer = getStringsBuffer(&iStringsBufferLength);
	if (!pStringsBuffer || iStringsBufferLength == 0)
	{
		SCError("failed to get frames buffer");
		return NULL;
	}
	
	PoolManager::sharedManager()->push();
	
	m_pStrings = getStringArrayFromBuffer(pStringsBuffer, iStringsBufferLength); // REF = 1, autoreleased
	
	if (!m_pStrings || m_pStrings->count() == 0)
	{
		SCError("string buffer empty");
		PoolManager::sharedManager()->pop();
		return NULL;
	}
	
	// parse from first item
	reset();
	DataItem * pItem = read();
	m_pRootObject = parseItem(pItem); // REF = 1, autoreleased
	SCAssert(pItem && m_pRootObject, "Invalid data");
	
	m_pRootObject->retain(); // REF = 2
	
	PoolManager::sharedManager()->pop();
	m_pStrings = NULL; // REF = 0
	
	return m_pRootObject;
}

#pragma mark -

void DataReader::reset(void)
{
	m_iItemIndex = 0;
}

DataItem * DataReader::read(void)
{
	DataItem * pItem = getItem(m_iItemIndex);
	m_iItemIndex++;
	return pItem;
}

String * DataReader::getString(const unsigned int iLocation) const
{
	if (!m_pStrings || m_pStrings->count() <= iLocation)
	{
		SCError("error, iLocation: %d", iLocation);
		return NULL;
	}
	return (String *)m_pStrings->objectAtIndex(iLocation);
}

#pragma mark -

Object * DataReader::parseItem(const DataItem * pItem)
{
	if (!pItem || pItem->reserved != '\0')
	{
		SCError("error");
		return NULL;
	}
	
	if (pItem->type >= DataItemTypeUnknown)
	{
		SCWarning("unknown type: %d", pItem->type);
		return NULL;
	}
	else if (pItem->type == DataItemTypeInteger)
	{
		String * pString = String::string();
		StringUtils::stringWithFormat(*pString, "%d", pItem->intValue);
		return pString;
	}
	else if (pItem->type == DataItemTypeFloat)
	{
		String * pString = String::string();
		StringUtils::stringWithFormat(*pString, "%f", pItem->floatValue);
		return pString;
	}
	else if (pItem->type == DataItemTypeString)
	{
		return getString(pItem->stringId);
	}
	else if (pItem->type == DataItemTypeArray)
	{
		return parseArrayItem(pItem);
	}
	else if (pItem->type == DataItemTypeDictionary)
	{
		return parseDictionaryItem(pItem);
	}
	else if (pItem->type == DataItemTypeBigInteger)
	{
		return getString(pItem->stringId); // load big integer as string currently
	}
	
	SCWarning("unrecognized type: %d", pItem->type);
	return NULL;
}

Array * DataReader::parseArrayItem(const DataItem * pItem)
{
	Array * pArray = Array::array();
	//pArray->initWithCapacity(pItem->count);
	
	for (int i = 0; i < pItem->count; i++)
	{
		DataItem * pChild = read();
		Object * pObj = parseItem(pChild);
		SCAssert(pObj, "Invalid item");
		pArray->addObject(pObj);
	}
	
	return pArray;
}

Dictionary * DataReader::parseDictionaryItem(const DataItem * pItem)
{
	Dictionary * pDict = new Dictionary();
	
	for (int i = 0; i < pItem->count; i++)
	{
		DataItem * pChild = read();
		String * pKey = pChild ? getString(pChild->key) : NULL;
		Object * pObj = parseItem(pChild);
		if (!pKey || pKey->length() == 0 || !pObj)
		{
			SCError("error, i = %d", i);
			continue;
		}
		pDict->setObject(pObj, pKey->c_str());
	}
	
	return pDict;
}

NAMESPACE_END
