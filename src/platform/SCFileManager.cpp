//
//  SCFileManager.cpp
//  SnowCat
//
//  Created by Moky on 13-8-12.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include <sys/stat.h>

#include "SCLog.h"
#include "SCClient.h"
#include "SCFile.h"
#include "SCFileManager.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

static bool mkdir_(const std::string & sPath)
{
	return mkdir(sPath.c_str(), 0755) == 0;
}

/**
 *  directory type:
 *      0 - resources directory
 *      1 - document directory
 *      2 - caches directory
 *      4 - temporary directory
 */
#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)
// Android
bool SCFileManager_makeDirectory(const int type, std::string & dir)
{
	dir.clear();
	if (type == 0)
	{
		// the resources directory is "" in Android
		return true;
	}
	
	std::string sPackageName = Client::currentClient()->getPackageName();
	std::string path = "/sdcard/.beva/" + sPackageName;
	if (!mkdir_(path))
	{
		path = "/data/data/" + sPackageName;
	}
	
	switch (type) {
		case 1:
			path.append("/docs");
			break;
		case 2:
			path.append("/caches");
			break;
		case 4:
			path.append("/tmp");
			break;
		default:
			break;
	}
	
	dir.assign(path);
	return mkdir_(dir);
}
#else
// iOS
extern bool SCFileManager_makeDirectory(const int type, std::string & dir);
#endif

FileManager::FileManager(void)
: m_sResourceDirectory("")
, m_sDocumentDirectory("")
, m_sCachesDirectory("")
, m_sTemporaryDirectory("")
{
	bool ok0 = SCFileManager_makeDirectory(0, m_sResourceDirectory);
	bool ok1 = SCFileManager_makeDirectory(1, m_sDocumentDirectory);
	bool ok2 = SCFileManager_makeDirectory(2, m_sCachesDirectory);
	bool ok4 = SCFileManager_makeDirectory(4, m_sTemporaryDirectory);
	
	if (!ok0 || !ok1 || !ok2 || !ok4)
	{
		SCError("error");
		SCAssert(false, "error");
	}
}

FileManager::~FileManager(void)
{
	
}

static FileManager * s_pSharedFileManager = new FileManager();

FileManager * FileManager::sharedManager(void)
{
	return s_pSharedFileManager;
}

URL & FileManager::fullFilePath(URL &sResourceFilename) const
{
#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)
	// Android app contains the resource directory in the .apk package
	// with the 'assets/' prefix.
	// no need to add anything to the resource path
	return sResourceFilename;
#endif
	URL dir(m_sResourceDirectory);
	sResourceFilename = dir.appendPathComponent(sResourceFilename);
	return sResourceFilename;
}

#pragma mark DOS

bool FileManager::isExists(const URL &sFilePath)
{
	if (sFilePath.empty())
	{
		return false;
	}
	else if (sFilePath.isAbsolutePath())
	{
		return access(sFilePath.c_str(), 0) == 0;
	}
	
#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)
	// assets file in apk?
	//...
#endif
	return false;
}

bool FileManager::isWritable(const URL &sFilePath)
{
	if (sFilePath.empty())
	{
		SCError("%s", sFilePath.c_str());
		return false;
	}
	if (!m_sDocumentDirectory.empty() && sFilePath.hasPrefix(m_sDocumentDirectory))
	{
		return true;
	}
	if (!m_sCachesDirectory.empty() && sFilePath.hasPrefix(m_sCachesDirectory))
	{
		return true;
	}
	if (!m_sTemporaryDirectory.empty() && sFilePath.hasPrefix(m_sTemporaryDirectory))
	{
		return true;
	}
	return false;
}

bool FileManager::copy(const URL &sSourcePath, const URL &sTargetPath)
{
	if (!isExists(sSourcePath) || !isWritable(URL(sTargetPath).deleteLastPathComponent()))
	{
		SCError("failed to copy file %s to %s", sSourcePath.c_str(), sTargetPath.c_str());
		return false;
	}
	
	File file;
	if (!file.read(sSourcePath.c_str()))
	{
		SCError("failed to read file: %s", sSourcePath.c_str());
		return false;
	}
	if (!file.write(sTargetPath.c_str()))
	{
		SCError("failed to write file: %s", sTargetPath.c_str());
		return false;
	}
	
	return true;
}

bool FileManager::move(const URL &sSourcePath, const URL &sTargetPath)
{
	if (!isExists(sSourcePath) || !isWritable(URL(sTargetPath).deleteLastPathComponent()))
	{
		SCError("cannot move file %s to %s", sSourcePath.c_str(), sTargetPath.c_str());
		return false;
	}
	
	if (rename(sSourcePath.c_str(), sTargetPath.c_str()) == 0)
	{
		return true;
	}
	SCLog("cannot rename %s to %s, try copying...", sSourcePath.c_str(), sTargetPath.c_str());
	
	// cannot rename, try copy
	if (!copy(sSourcePath, sTargetPath))
	{
		SCError("failed to copy file %s to %s", sSourcePath.c_str(), sTargetPath.c_str());
		return false;
	}
	
	if (!remove(sSourcePath))
	{
		SCWarning("cannot remove old file: %s", sSourcePath.c_str());
		return false;
	}
	
	return true;
}

bool FileManager::remove(const URL &sFilePath)
{
	if (!isExists(sFilePath) || !isWritable(URL(sFilePath).deleteLastPathComponent()))
	{
		SCError("cannot remove file: %s", sFilePath.c_str());
		return false;
	}
	if (unlink(sFilePath.c_str()) == 0)
	{
		return true;
	}
	else
	{
		SCError("failed to remove file: %s", sFilePath.c_str());
		return false;
	}
}

bool FileManager::mkdir(const URL &sDir)
{
	if (!sDir.isAbsolutePath())
	{
		SCError("cannot mkdir: %s", sDir.c_str());
		return false;
	}
	URL dir(sDir);
	dir.trim();
	if (dir.empty() || isExists(dir))
	{
		return true;
	}
	
	// create upper directory
	if (!mkdir(URL(dir).deleteLastPathComponent()))
	{
		SCError("failed to create upper dir: %s", sDir.c_str());
		return false;
	}
	
	if (mkdir_(dir))
	{
		return true;
	}
	else
	{
		SCError("failed to mkdir: %s", dir.c_str());
		return false;
	}
}

NAMESPACE_END
