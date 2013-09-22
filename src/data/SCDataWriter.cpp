//
//  SCDataWriter.cpp
//  SnowCat
//
//  Created by Moky on 13-8-9.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCObject.h"
#include "SCString.h"
#include "SCArray.h"
#include "SCDictionary.h"
#include "SCInteger.h"

#include "SCDataWriter.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

static unsigned char * createBufferWithStringArray(Array * pArray, unsigned long * pBufferLength)
{
	if (!pArray || pArray->count() == 0 || !pBufferLength)
	{
		return NULL;
	}
	SCAssert(pArray->count() < 65536 * 256, "too big array");
	
	unsigned int iMaxLength = 65536 * pArray->count();
	
	unsigned char * pBuffer = (unsigned char *)malloc(iMaxLength);
	memset(pBuffer, 0, iMaxLength);
	char * p = (char *)pBuffer;
	
	// save 'count' first
	unsigned long * pCount = (unsigned long *)p; // 0 - 4,294,967,295 (4G)
	p += sizeof(unsigned long);
	*pCount = pArray->count();
	
	unsigned short * pLen;
	for (int i = 0; i < *pCount; i++)
	{
		String * pString = (String *)pArray->objectAtIndex(i);
		SCAssert(pString->length() < 65536, "too long string");
		
		pLen = (unsigned short *)p; // 0 - 65535
		p += sizeof(unsigned short);
		*pLen = pString->length();
		
		sprintf(p, "%s", pString->c_str());
		p += *pLen;
	}
	
	*pBufferLength = (unsigned char *)p - pBuffer;
	return pBuffer;
}

static int indexForString(String * pString, Array * pArray)
{
	if (!pString)
	{
		return -1;
	}
	unsigned int iCount = pArray->count();
	unsigned int index = 0;
	for (; index < iCount; index++)
	{
		String * pValue = (String *)pArray->objectAtIndex(index);
		if (strcmp(pString->c_str(), pValue->c_str()) == 0)
		{
			break;
		}
	}
	if (index >= iCount)
	{
		//SCLog("adding new string %s to array, index: %d", pString->c_str(), pArray->count());
		pArray->addObject(pString);
	}
	
	return index;
}

static int processData(DataItem * pItemBuffer, unsigned int iPlaceLeft, IObject * pObject, Array * pStringsArray); // pre-defined

static int processString(DataItem * pItemBuffer, unsigned int iPlaceLeft, String * pString, Array * pStringsArray)
{
	if (pString->isNumeric())
	{
		if (pString->isInteger())
		{
			pItemBuffer->type = DataItemTypeInteger;
			pItemBuffer->intValue = pString->intValue();
		}
		else
		{
			pItemBuffer->type = DataItemTypeFloat;
			pItemBuffer->floatValue = pString->floatValue();
		}
		return 1;
	}
	
	int index = indexForString(pString, pStringsArray);
	SCAssert(index >= 0, "");
	if (index < 0)
	{
		SCError("could not happen");
		return 0;
	}
	pItemBuffer->type = DataItemTypeString;
	pItemBuffer->stringId = index;
	return 1;
}

static int processArray(DataItem * pItemBuffer, unsigned int iPlaceLeft, const Array * pArray, Array * pStringsArray)
{
	int iCount = 0;
	pItemBuffer->type = DataItemTypeArray;
	pItemBuffer->count = pArray->count();
	iCount++;
	
	DataItem * pChild = pItemBuffer + 1; // first child, starts from next space
	IObject * pObject = NULL;
	SC_ARRAY_FOREACH(pArray, pObject)
	{
		int cnt = processData(pChild, iPlaceLeft - iCount, pObject, pStringsArray);
		if (cnt <= 0)
		{
			SCError("error");
			continue;
		}
		pChild += cnt;
		iCount += cnt;
	}
	return iCount;
}

static int processDictionary(DataItem * pItemBuffer, unsigned int iPlaceLeft, const Dictionary * pDict, Array * pStringsArray)
{
	int iCount = 0;
	pItemBuffer->type = DataItemTypeDictionary;
	pItemBuffer->count = pDict->count();
	iCount++;
	
	DataItem * pChild = pItemBuffer + 1; // first child, starts from next space
	std::string key;
	IObject * obj;
	SC_DICTIONARY_FOREACH(pDict, key, obj)
	{
		String * pKey = new String(key);
		int index = indexForString(pKey, pStringsArray);
		pKey->release();
		
		SCAssert(index >= 0, "");
		if (index < 0)
		{
			SCError("could not happen");
			continue;
		}
		pChild->key = index;
		
		int cnt = processData(pChild, iPlaceLeft - iCount, obj, pStringsArray);
		if (cnt <= 0)
		{
			SCError("error");
			continue;
		}
		pChild += cnt;
		iCount += cnt;
	}
	return iCount;
}

static int processData(DataItem * pItemBuffer, unsigned int iPlaceLeft, IObject * pObject, Array * pStringsArray)
{
	SCAssert(iPlaceLeft > 0, "");
	if (iPlaceLeft == 0)
	{
		SCError("not enough space");
		return -1;
	}
	int iCount = 0;
	if (!pItemBuffer || !pObject || !pStringsArray)
	{
		SCError("error");
	}
	else if (Dictionary * pDict = dynamic_cast<Dictionary *>(pObject))
	{
		iCount += processDictionary(pItemBuffer, iPlaceLeft, pDict, pStringsArray);
	}
	else if (Array * pArray = dynamic_cast<Array *>(pObject))
	{
		iCount += processArray(pItemBuffer, iPlaceLeft, pArray, pStringsArray);
	}
	else if (String * pString = dynamic_cast<String *>(pObject))
	{
		iCount += processString(pItemBuffer, iPlaceLeft, pString, pStringsArray);
	}
	else if (Integer * pInteger = dynamic_cast<Integer *>(pObject))
	{
		pItemBuffer->type = DataItemTypeBigInteger;
		String * pString = pInteger->stringValue(NULL); // save big integer as string
		iCount += processString(pItemBuffer, iPlaceLeft, pString, pStringsArray);
	}
	else
	{
		SCError("Warning: Unknown Object.");
		pItemBuffer->type = DataItemTypeUnknown;
		iCount++;
	}
	return iCount;
}

static unsigned char * createItemsBuffer(Object * pData, unsigned long * pBufferLength, Array * pStringsArray)
{
	if (!pData || !pBufferLength || !pStringsArray)
	{
		SCError("error");
		return NULL;
	}
	
	unsigned int iMaxItems = 65536;
	unsigned long iBufferLength = sizeof(DataItem) * iMaxItems;
	
	DataItem * pItemsBuffer = (DataItem *)malloc(iBufferLength);
	SCAssert(pItemsBuffer, "Not enough memory");
	memset(pItemsBuffer, 0, iBufferLength);
	
	int iCount = processData(pItemsBuffer, iMaxItems, pData, pStringsArray);
	if (iCount <= 0)
	{
		SCError("error");
		free(pItemsBuffer);
		pItemsBuffer = 0;
	}
	*pBufferLength = sizeof(DataItem) * iCount;
	return (unsigned char *)pItemsBuffer;
}

DataWriter::DataWriter(void)
: Data()
{
	
}

DataWriter::DataWriter(Object * data)
: Data()
{
	bool ok = init(data);
	SCAssert(ok, "init data error");
	(void)ok;
}

bool DataWriter::init(Object * data)
{
	if (!data)
	{
		return false;
	}
	
	Array * pStringsArray = new Array();
	//pStringsArray->initWithCapacity(65536);
	
	// items buffer
	unsigned long iItemsBufferLength = 0;
	unsigned char * pItemsBuffer = createItemsBuffer(data, &iItemsBufferLength, pStringsArray);
	if (!pItemsBuffer || iItemsBufferLength == 0)
	{
		SCError("error");
		if (pItemsBuffer) free(pItemsBuffer);
		pStringsArray->release();
		return false;
	}
	
	// string array buffer
	unsigned long iStringsBufferLength = 0;
	unsigned char * pStringsBuffer = createBufferWithStringArray(pStringsArray, &iStringsBufferLength);
	if (!pStringsBuffer || iStringsBufferLength == 0)
	{
		SCError("error");
		if (pStringsBuffer) free(pStringsBuffer);
		if (pItemsBuffer) free(pItemsBuffer);
		pStringsArray->release();
		return false;
	}
	
	//---- OK!
	SCLog("iItemsBufferLength = %ld, iStringsBufferLength = %ld", iItemsBufferLength, iStringsBufferLength);
	
	unsigned long iBufferLength = sizeof(DataHead) + iItemsBufferLength + iStringsBufferLength;
	SCLog("iBufferLength = %ld", iBufferLength);
	
	if (!super::init(iBufferLength))
	{
		if (pStringsBuffer) free(pStringsBuffer);
		if (pItemsBuffer) free(pItemsBuffer);
		pStringsArray->release();
		return false;
	}
	
	// init head
	unsigned long iHeadOffset = 0;
	unsigned long iHeadLength = sizeof(DataHead);
	DataHead * pHead = (DataHead *)&m_pBuffer[iHeadOffset];
	
	// copy items buffer
	unsigned long iItemsBufferOffset = iHeadOffset + iHeadLength;
	memcpy(&m_pBuffer[iItemsBufferOffset], pItemsBuffer, iItemsBufferLength);
	pHead->body.itemsBuffer.offset = iItemsBufferOffset;
	pHead->body.itemsBuffer.length = iItemsBufferLength;
	
	// copy strings array buffer
	unsigned long iStringsBufferOffset = iItemsBufferOffset + iItemsBufferLength;
	memcpy(&m_pBuffer[iStringsBufferOffset], pStringsBuffer, iStringsBufferLength);
	pHead->body.stringsBuffer.offset = iStringsBufferOffset;
	pHead->body.stringsBuffer.length = iStringsBufferLength;
	
	if (pStringsBuffer) free(pStringsBuffer);
	if (pItemsBuffer) free(pItemsBuffer);
	pStringsArray->release();
	
	m_bIsDataValid = (checkDataFormat() == DataFormatOK);
	return m_bIsDataValid;
}

NAMESPACE_END
