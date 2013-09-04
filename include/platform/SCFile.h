//
//  SCFile.h
//  SnowCat
//
//  Created by Moky on 13-8-9.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCFile_h
#define SnowCat_SCFile_h

#include "scMacros.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class File
{
protected:
	unsigned char * m_pBuffer;
	unsigned long m_iSize;
	
public:
	File(void);
	File(const char * pszFilename);
	virtual ~File(void);
	virtual bool init(void);
	virtual bool init(unsigned char * pBuffer, const unsigned long iBufferLen);
	
	virtual bool read(const char * pszFilename);
	virtual bool write(const char * pszFilename);
};

NAMESPACE_END

#endif
