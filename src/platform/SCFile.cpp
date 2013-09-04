//
//  SCFile.cpp
//  SnowCat
//
//  Created by Moky on 13-8-9.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SCLog.h"
#include "SCFile.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

File::File(void)
: m_pBuffer(0)
, m_iSize(0)
{
	
}

File::File(const char * pszFilename)
: m_pBuffer(0)
, m_iSize(0)
{
	bool ok = this->read(pszFilename);
	SCAssert(ok, "init file error");
	if (ok)
	{
		SCLog("success to init file: %s, content length: %lu", pszFilename, m_iSize);
	}
	else
	{
		SCLog("failed to init file: %s", pszFilename);
	}
}

File::~File(void)
{
	if (m_pBuffer)
	{
		free(m_pBuffer);
	}
}

bool File::init(void)
{
	if (m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = NULL;
	}
	m_iSize = 0;
	return true;
}

bool File::init(unsigned char *pBuffer, const unsigned long iBufferLen)
{
	SCAssert(pBuffer && iBufferLen > 0, "Invalid buffer");
	this->init();
	
	m_pBuffer = (unsigned char *)malloc(iBufferLen);
	SCAssert(m_pBuffer, "Not enough memory");
	memset(m_pBuffer, 0, iBufferLen);
	
	memcpy(m_pBuffer, pBuffer, iBufferLen);
	m_iSize = iBufferLen;
	return true;
}

bool File::read(const char *pszFilename)
{
	SCAssert(pszFilename, "Invalid filename");
	this->init();
	
	FILE * fp = fopen(pszFilename, "rb");
	if (!fp)
	{
		SCError("failed to open file: %s", pszFilename);
		return false;
	}
	
	fseek(fp, 0, SEEK_END);
	m_iSize = ftell(fp);
	SCAssert(m_iSize > 0, "Empty file");
	fseek(fp, 0, SEEK_SET);
	
	m_pBuffer = (unsigned char *)malloc(m_iSize);
	SCAssert(m_pBuffer, "Not enough memory");
	memset(m_pBuffer, 0, m_iSize);
	
	unsigned long len = fread(m_pBuffer, sizeof(unsigned char), m_iSize, fp);
	fclose(fp);
	
	SCAssert(len == m_iSize, "I/O error");
	return len == m_iSize;
}

bool File::write(const char * pszFilename)
{
	SCAssert(pszFilename, "Invalid filename");
	SCAssert(m_pBuffer && m_iSize > 0, "Invalid buffer");
	
	FILE * fp = fopen(pszFilename, "wb");
	if (!fp)
	{
		SCError("Permission denied: %s", pszFilename);
		return false;
	}
	
	unsigned long len = fwrite(m_pBuffer, sizeof(unsigned char), m_iSize, fp);
	fclose(fp);
	
	SCAssert(len == m_iSize, "I/O error");
	return len == m_iSize;
}

NAMESPACE_END
