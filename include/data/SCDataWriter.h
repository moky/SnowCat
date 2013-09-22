//
//  SCDataWriter.h
//  SnowCat
//
//  Created by Moky on 13-8-9.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCDataWriter_h
#define SnowCat_SCDataWriter_h

#include "SCData.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class Object;

class DataWriter : public Data
{
	typedef Data super;
	
public:
	DataWriter(void);
	DataWriter(Object * data);
	
	virtual bool init(Object * data);
};

NAMESPACE_END

#endif
