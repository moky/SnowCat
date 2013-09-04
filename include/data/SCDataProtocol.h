//
//  SCDataProtocol.h
//  SnowCat
//
//  Created by Moky on 13-8-27.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCDataProtocol_h
#define SnowCat_SCDataProtocol_h

#include "scMacros.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

typedef enum {
	DataItemTypeString           = 0,
	DataItemTypeArray            = 1,
	DataItemTypeDictionary       = 2,
	DataItemTypeInteger          = 3,
	DataItemTypeFloat            = 4,
	// 5 (reserved)
	DataItemTypeBigInteger       = 6,
	
	DataItemTypeUnknown          = 255,
} DataItemType;

typedef enum {
	DataFormatOK                 =  0,
	DataFormatErrorNotInit       = -1,
	DataFormatErrorSign          = -2,
	DataFormatErrorVersion       = -3,
	DataFormatErrorSize          = -4,
	DataFormatErrorItemsBuffer   = -5,
	DataFormatErrorStringsBuffer = -6,
} DataFormatError;

#pragma pack(1)

typedef struct {
	unsigned char type;         // 0 - 255
	unsigned char reserved;     // reserved & for bytes alignment
	unsigned short key;         // 0 - 65,535 (64K)
	union {
		// string
		unsigned long stringId; // 0 - 4,294,967,295 (4G)
		// dictionary/array
		unsigned short count;   // 0 - 65,535 (64K)
		// numeric
		unsigned long value;
		int intValue;
		unsigned int uintValue;
		float floatValue;
		bool boolValue;
	};
} DataItem;

typedef struct {
	struct {
		unsigned long  offset; // offset in memory buffer
		unsigned long  length; // length of the sub buffer
	} itemsBuffer;
	struct {
		unsigned long  offset; // offset in memory buffer
		unsigned long  length; // length of the sub buffer
	} stringsBuffer;
} DataBody;

typedef struct {
	// protocol
	unsigned char format[4]; // "PBF"
	unsigned char headVersion;
	unsigned char version;
	unsigned char reserved[10]; // reserved & for bytes alignment
	unsigned long fileLength;
	// descriptions
	unsigned char description[64];
	unsigned char copyright[32];
	unsigned char author[32];
	// body
	DataBody body;
} DataHead;

#pragma pack()

NAMESPACE_END

#endif
