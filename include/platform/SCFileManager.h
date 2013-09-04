//
//  SCFileManager.h
//  SnowCat
//
//  Created by Moky on 13-8-12.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCFileManager_h
#define SnowCat_SCFileManager_h

#include "SCUrl.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class FileManager
{
	// main bundle
	URL m_sResourceDirectory;
	// directories in '/sdcard/'
	URL m_sDocumentDirectory;
	URL m_sCachesDirectory;
	URL m_sTemporaryDirectory;
	
public:
	FileManager(void);
	virtual ~FileManager(void);
	virtual bool init(void);
	
	static FileManager * sharedManager(void);
	
	URL & fullFilePath(URL & sResourceFilename) const;
	
	// DOS
	bool isExists(const URL & sFilePath);
	bool isWritable(const URL & sFilePath);
	
	bool copy(const URL & sSourcePath, const URL & sTargetPath);
	bool move(const URL & sSourcePath, const URL & sTargetPath);
	bool remove(const URL & sFilePath);
	bool mkdir(const URL & sDir);
	
	// directories
	const char * resourceDirectory(void) const { return m_sResourceDirectory.c_str(); }
	const char * documentDirectory(void) const { return m_sDocumentDirectory.c_str(); }
	const char * cachesDirectory(void) const { return m_sCachesDirectory.c_str(); }
	const char * temporaryDirectory(void) const { return m_sTemporaryDirectory.c_str(); }
};

NAMESPACE_END

#endif
