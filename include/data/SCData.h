//
//  SCData.h
//  SnowCat
//
//  Created by Moky on 13-8-8.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCData_h
#define SnowCat_SCData_h

#include "SCDataProtocol.h"
#include "SCFile.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class Data : public File
{
protected:
	bool m_bIsDataValid;
	
public:
	Data(void);
	Data(const char * pszFilename);
	virtual ~Data(void);
	virtual bool init(void);
	
	virtual bool init(const unsigned long iBufferLength); // create an initialized buffer
	
	virtual int checkDataFormat(void) const;
	
#pragma mark Data Interfaces
	
protected:
	DataItem * getItem(const unsigned int iLocation) const;
	unsigned char * getItemsBuffer(unsigned long * pBufferLength) const;
	unsigned char * getStringsBuffer(unsigned long * pBufferLength) const;
};

NAMESPACE_END

#endif
