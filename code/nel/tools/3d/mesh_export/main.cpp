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
#include "../mesh_utils/mesh_utils.h"

#include <nel/misc/cmd_args.h>
#include <nel/misc/path.h>

#include <nel/3d/register_3d.h>
#include <nel/3d/scene.h>

int printHelp(const NLMISC::CCmdArgs &args)
{
	printf("NeL Mesh Export\n");
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	NLMISC::CApplicationContext app;

	NLMISC::CCmdArgs args;
	args.setArgs(argc, (const char **)argv);

	if (args.getArgs().size() == 1
		|| args.haveArg('h')
		|| args.haveLongArg("help"))
		return printHelp(args);

	const NLMISC::CSString &filePath = args.getArgs().back();
	if (!NLMISC::CFile::fileExists(filePath))
	{
		printHelp(args);
		nlerror("File '%s' does not exist", filePath.c_str());
		return EXIT_FAILURE;
	}

	CMeshUtilsSettings settings;
	settings.SourceFilePath = filePath;
	
	if (args.haveArg('d'))
		settings.DestinationDirectoryPath = args.getArg('d');
	if (settings.DestinationDirectoryPath.empty())
		settings.DestinationDirectoryPath = args.getLongArg("dst");
	if (settings.DestinationDirectoryPath.empty())
		settings.DestinationDirectoryPath = filePath + "_export";
	settings.DestinationDirectoryPath += "/";

	settings.ToolDependLog = args.getLongArg("dependlog");
	if (settings.ToolDependLog.empty())
		settings.ToolDependLog = settings.DestinationDirectoryPath + "depend.log";
	settings.ToolErrorLog = args.getLongArg("errorlog");
	if (settings.ToolErrorLog.empty())
		settings.ToolErrorLog = settings.DestinationDirectoryPath + "error.log";

	NL3D::CScene::registerBasics();
	NL3D::registerSerial3d();

	return exportScene(settings);
}

/* end of file */
