#ifndef INCLUDES_WINDOWS_H
#define INCLUDES_WINDOWS_H

#include <string>
#include <QDir>
#include <windows.h>

#include "vld.h"

#include "FileSystem.h"
#include "AppPath.h"
#include "Application.h"
#include "ResourcePaths.h"
#include "UserPaths.h"
#include "utility.h"

void setupPlatform(int argc, char *argv[])
{
}

void setupApp(int argc, char *argv[])
{
	{
		HMODULE hModule = GetModuleHandleW(NULL);
		WCHAR path[MAX_PATH];
		GetModuleFileNameW(hModule, path, MAX_PATH);

		std::wstring appPath(path);

		size_t pos = appPath.find_last_of(L"/");
		if (pos == std::wstring::npos)
		{
			pos = appPath.find_last_of(L"\\");
		}
		if (pos != std::wstring::npos)
		{
			appPath = appPath.substr(0, pos + 1);
		}
		AppPath::setAppPath(FilePath(appPath));
	}

	{
		FilePath userDataPath = AppPath::getAppPath().concatenate(L"user/");
		if (!userDataPath.exists())
		{
			FilePath userLocalPath = FilePath(std::string(std::getenv("LOCALAPPDATA")));
			if (!userLocalPath.exists())
			{
				userLocalPath = FilePath(std::string(std::getenv("APPDATA")) + "/../local");
			}

			if (userLocalPath.exists())
			{
				userDataPath = userLocalPath.getConcatenated(L"Coati Software/");
				if (utility::getApplicationArchitectureType() == APPLICATION_ARCHITECTURE_X86_64)
				{
					userDataPath.concatenate(L"Sourcetrail 64-bit/");
				}
				else
				{
					userDataPath.concatenate(L"Sourcetrail/");
				}
				userDataPath.makeCanonical();
				Application::EULA_ACCEPT_REQUIRED = false;

			}
			else
			{
				userDataPath = AppPath::getAppPath().concatenate(L"user_fallback/");
				LOG_ERROR(L"The \"%LOCALAPPDATA%\" path could not be found. Falling back to \"" + userDataPath.wstr() + L"\" to store settings data.");
				FileSystem::createDirectory(userDataPath);
			}
		}

		UserPaths::setUserDataPath(userDataPath);
	}

	// This "copyFile" method does nothing if the copy destination already exist
	FileSystem::copyFile(ResourcePaths::getFallbackPath().concatenate(L"ApplicationSettings.xml"), UserPaths::getAppSettingsPath());
	FileSystem::copyFile(ResourcePaths::getFallbackPath().concatenate(L"window_settings.ini"), UserPaths::getWindowSettingsPath());
}

#endif // INCLUDES_WINDOWS_H
