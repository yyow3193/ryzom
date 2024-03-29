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

namespace NLMISC {
	class CConfigFile;
}

#ifdef NL_OS_WINDOWS
#include <nel/misc/sstring.h>
typedef NLMISC::CSString TPathString;
#else
typedef std::string TPathString;
#endif

/// Asset database configuration
class CDatabaseConfig
{
public:
	~CDatabaseConfig();

	/// Searches for the configuration for the specified asset path by recursively going through all parent directories looking for 'database.cfg', initializes and applies the configuration.
	static bool init(const std::string &asset);
	static void release();

	static void initTextureSearchDirectories();

	static inline const TPathString &rootPath() { return s_RootPath; }
	static inline TPathString configPath() { return s_RootPath + "/database.cfg"; }

private:
	static void cleanup();
	static void searchDirectories(const char *var);

	static CDatabaseConfig s_Instance;
	static uint32 s_ConfigFileModification;

	static TPathString s_RootPath;
	static NLMISC::CConfigFile *s_ConfigFile;

};

/* end of file */
