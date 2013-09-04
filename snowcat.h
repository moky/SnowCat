//
//  snowcat.h
//  SnowCat
//
//  Created by Moky on 13-8-6.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_snowcat_h
#define SnowCat_snowcat_h

// base
#include "scMacros.h"
#include "SCLog.h"
#include "SCAutoreleasePool.h"
// data
#include "SCDataProtocol.h"
#include "SCData.h"
#include "SCDataReader.h"
#include "SCDataWriter.h"
// types
#include "SCObject.h"
#include "SCString.h"
#include "SCArray.h"
#include "SCDictionary.h"
#include "SCInteger.h"
// utils
#include "SCStringUtils.h"
#include "SCUrlUtils.h"
#include "SCBaseArray.h"
#include "SCBaseDictionary.h"
#include "SCObjectDictionary.h"
// http
#include "SCUrl.h"
// data
// platform
#include "SCFile.h"
#include "SCFileManager.h"
#include "SCClient.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

const char * version(void);

NAMESPACE_END

#endif
