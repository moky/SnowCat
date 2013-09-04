//
//  SCData.cpp
//  SnowCat
//
//  Created by Moky on 13-8-8.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "SCLog.h"

#include "SCData.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

#define SC_DATA_HEAD_VERSION 0x01
#define SC_DATA_VERSION      0x01

Data::Data(void)
: File()
, m_bIsDataValid(false)
{
	
}

Data::Data(const char * pszFilename)
: File(pszFilename)
, m_bIsDataValid(false)
{
	m_bIsDataValid = (checkDataFormat() == DataFormatOK);
}

Data::~Data(void)
{
	
}

bool Data::init(void)
{
	File::init();
	m_bIsDataValid = false;
	return true;
}

bool Data::init(const unsigned long iBufferLength)
{
	if (!this->init())
	{
		return false;
	}
	
	if (iBufferLength <= sizeof(DataHead))
	{
		SCError("not enough buffer length: %ld", iBufferLength);
		return false;
	}
	
	m_iSize = iBufferLength;
	m_pBuffer = (unsigned char *)malloc(m_iSize);
	SCAssert(m_pBuffer, "Not enough memory");
	
	// init the buffer
	memset(m_pBuffer, 0, m_iSize);
	
	DataHead * pHead = (DataHead *)m_pBuffer;
	
	// protocol
	pHead->format[0] = 'P';
	pHead->format[1] = 'B';
	pHead->format[2] = 'F';
	pHead->format[3] = '\0';
	
	pHead->headVersion = SC_DATA_HEAD_VERSION;
	pHead->version     = SC_DATA_VERSION;
	
	pHead->fileLength = m_iSize;
	
	// descriptions
	time_t now = time(NULL);
	tm * tm = gmtime(&now);
	
	snprintf((char *)pHead->description, sizeof(pHead->description),
			 "Binary File for Plist. Generated at %d-%d-%d %d:%d:%d",
			 tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	
	snprintf((char *)pHead->copyright, sizeof(pHead->copyright),
			 "Copyright %d Slanissue Inc.", tm->tm_year + 1900);
	
	snprintf((char *)pHead->author, sizeof(pHead->author),
			 "Author: Moky@Beva.com, %d-%d-%d", 2013, 1, 4);
	
	return true;
}

int Data::checkDataFormat(void) const
{
	if (m_bIsDataValid)
	{
		// the data had been checked before
		// quick return here
		return DataFormatOK;
	}
	
	if (!m_pBuffer || m_iSize <= sizeof(DataHead))
	{
		SCError("data buffer not init");
		return DataFormatErrorNotInit;
	}
	
	// check head
	DataHead * pHead = (DataHead *)m_pBuffer;
	if (pHead->format[0] != 'P' || pHead->format[1] != 'B' || pHead->format[2] != 'F' || pHead->format[3] != '\0')
	{
		SCError("format sign error: [%c%c%c%c]", pHead->format[0], pHead->format[1], pHead->format[2], pHead->format[3]);
		return DataFormatErrorSign;
	}
	
	if (pHead->headVersion != SC_DATA_HEAD_VERSION || pHead->version != SC_DATA_VERSION)
	{
		SCError("versions error: %d.%d", pHead->headVersion, pHead->version);
		return DataFormatErrorVersion;
	}
	
	if (pHead->fileLength != m_iSize)
	{
		SCError("file length error: %ld != %lu", pHead->fileLength, m_iSize);
		return DataFormatErrorSize;
	}
	
	// check buffer info
	if (pHead->body.itemsBuffer.offset != sizeof(DataHead) ||
		pHead->body.itemsBuffer.length == 0)
	{
		SCError("frames buffer info error");
		return DataFormatErrorItemsBuffer;
	}
	
	if (pHead->body.stringsBuffer.offset != pHead->body.itemsBuffer.offset + pHead->body.itemsBuffer.length ||
		pHead->body.stringsBuffer.length == 0)
	{
		SCError("strings array buffer info error");
		return DataFormatErrorStringsBuffer;
	}
	
	unsigned long fileLength = sizeof(DataHead)
	+ pHead->body.itemsBuffer.length
	+ pHead->body.stringsBuffer.length;
	
	if (fileLength != m_iSize)
	{
		SCError("file length error");
		return DataFormatErrorSize;
	}
	
	return DataFormatOK;
}

unsigned char * Data::getItemsBuffer(unsigned long * pBufferLength) const
{
	if (checkDataFormat() != DataFormatOK)
	{
		return NULL;
	}
	DataHead * pHead = (DataHead *)m_pBuffer;
	*pBufferLength = pHead->body.itemsBuffer.length;
	return &m_pBuffer[pHead->body.itemsBuffer.offset];
}

unsigned char * Data::getStringsBuffer(unsigned long * pBufferLength) const
{
	if (checkDataFormat() != DataFormatOK)
	{
		return NULL;
	}
	DataHead * pHead = (DataHead *)m_pBuffer;
	*pBufferLength = pHead->body.stringsBuffer.length;
	return &m_pBuffer[pHead->body.stringsBuffer.offset];
}

DataItem * Data::getItem(const unsigned int iLocation) const
{
	unsigned long iBufferLength = 0;
	DataItem * pArray = (DataItem *)getItemsBuffer(&iBufferLength);
	if (!pArray || iBufferLength < sizeof(DataItem) * (iLocation + 1))
	{
		SCError("failed to get item, location: %d, element size: %ld, buffer length: %ld", iLocation, sizeof(DataItem), iBufferLength);
		return NULL;
	}
	return &pArray[iLocation];
}

NAMESPACE_END
