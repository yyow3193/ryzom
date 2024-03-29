// NeL - MMORPG Framework <http://dev.ryzom.com/projects/nel/>
// Copyright (C) 2015  Winch Gate Property Limited
// Author: Jan Boon <jan.boon@kaetemi.be>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <nel/misc/types_nl.h>
#include "database_config.h"

#include <nel/misc/debug.h>
#include <nel/misc/path.h>
#include <nel/misc/config_file.h>

using namespace std;
using namespace NLMISC;

TPathString CDatabaseConfig::s_RootPath;
NLMISC::CConfigFile *CDatabaseConfig::s_ConfigFile = NULL;
CDatabaseConfig CDatabaseConfig::s_Instance;
uint32 CDatabaseConfig::s_ConfigFileModification;

static std::set<TPathString> s_SearchPaths;

void CDatabaseConfig::cleanup()
{
	delete CDatabaseConfig::s_ConfigFile;
	CDatabaseConfig::s_ConfigFile = NULL;
}

CDatabaseConfig::~CDatabaseConfig()
{
	cleanup();
}

bool CDatabaseConfig::init(const std::string &asset)
{
	// release();

	TPathString rootPath = NLMISC::CPath::standardizePath(asset, false);
	TPathString configPath = rootPath + "/database.cfg";
	while (!CFile::fileExists(configPath))
	{
		int sep = CFile::getLastSeparator(rootPath);
		if (sep == string::npos)
			return false;

		rootPath = rootPath.substr(0, sep);
		if (rootPath.empty())
			return false;

		configPath = rootPath + "/database.cfg";
	}

	rootPath += "/";
	uint32 configFileModification = CFile::getFileModificationDate(configPath);
	if (rootPath == s_RootPath && s_ConfigFileModification == configFileModification)
		return true; // Do not reload

	nldebug("Initializing database config '%s'", configPath.c_str());
	release();

	s_RootPath = rootPath;
	s_ConfigFileModification = configFileModification;

	s_ConfigFile = new CConfigFile();
	s_ConfigFile->load(configPath);
	return true;
}

void CDatabaseConfig::initTextureSearchDirectories()
{
	searchDirectories("TextureSearchDirectories");
}

void CDatabaseConfig::searchDirectories(const char *var)
{
	CConfigFile::CVar &paths = s_ConfigFile->getVar(var);
	for (uint i = 0; i < paths.size(); i++)
	{
		TPathString path = paths.asString(i);
		if (s_SearchPaths.find(path) == s_SearchPaths.end())
		{
			CPath::addSearchPath(s_RootPath + path);
			s_SearchPaths.insert(path);
		}
	}
}

void CDatabaseConfig::release()
{
	s_SearchPaths.clear();
	CPath::clearMap();
	cleanup();
}

/* end of file */
