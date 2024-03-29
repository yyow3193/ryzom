/**
 * \file tool_logger.h
 * \brief CToolLogger
 * \date 2012-02-19 10:33GMT
 * \author Jan Boon (Kaetemi)
 * Tool logger is fully implemented in header so small tools do not
 * need to link to this library unnecessarily.
 * NOTE: Needs to be changed not to use time_nl and string_common.
 */

/*
 * Copyright (C) 2012  by authors
 *
 * This file is part of RYZOM CORE PIPELINE.
 * RYZOM CORE PIPELINE is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of
 * the License, or (at your option) any later version.
 *
 * RYZOM CORE PIPELINE is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RYZOM CORE PIPELINE; see the file COPYING.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef NLMISC_TOOL_LOGGER_H
#define NLMISC_TOOL_LOGGER_H
#include <nel/misc/types_nl.h>

// STL includes
#include <string>
#include <stdio.h>
#include <stdlib.h>

// NeL includes
#include <nel/misc/time_nl.h>
#include <nel/misc/string_common.h>

// Project includes

#ifdef ERROR
#undef ERROR
#endif

#ifdef NL_DEBUG_H
#define tlerror(toolLogger, path, error, ...) nlwarning(error, ## __VA_ARGS__), toolLogger.writeError(NLMISC::ERROR, path, error, ## __VA_ARGS__)
#define tlwarning(toolLogger, path, error, ...) nlwarning(error, ## __VA_ARGS__), toolLogger.writeError(NLMISC::WARNING, path, error, ## __VA_ARGS__)
#define tlmessage(toolLogger, path, error, ...) nlinfo(error, ## __VA_ARGS__), toolLogger.writeError(NLMISC::MESSAGE, path, error, ## __VA_ARGS__)
#else
#define tlerror(toolLogger, path, error, ...) toolLogger.writeError(NLMISC::ERROR, path, error, ## __VA_ARGS__)
#define tlwarning(toolLogger, path, error, ...) toolLogger.writeError(NLMISC::WARNING, path, error, ## __VA_ARGS__)
#define tlmessage(toolLogger, path, error, ...) toolLogger.writeError(NLMISC::MESSAGE, path, error, ## __VA_ARGS__)
#endif

namespace NLMISC {

enum TError
{
	ERROR,
	WARNING,
	MESSAGE,
};

enum TDepend
{
	BUILD,
	DIRECTORY,
	RUNTIME,
};

const std::string s_ErrorHeader = "type\tpath\ttime\terror";
const std::string s_DependHeader = "type\toutput_file\tinput_file";

/**
 * \brief CToolLogger
 * \date 2012-02-19 10:33GMT
 * \author Jan Boon (Kaetemi)
 * CToolLogger
 */
class CToolLogger
{
private:
	FILE *m_ErrorLog;
	FILE *m_DependLog;

public:
	inline CToolLogger() : m_ErrorLog(NULL), m_DependLog(NULL)
	{

	}

	inline ~CToolLogger()
	{
		release();
	}

	inline void initError(const std::string &errorLog)
	{
		releaseError();

		m_ErrorLog = fopen(errorLog.c_str(), "wt");
		fwrite(s_ErrorHeader.c_str(), 1, s_ErrorHeader.length(), m_ErrorLog);
		fwrite("\n", 1, 1, m_ErrorLog);
		fflush(m_ErrorLog);

	}

	inline void initDepend(const std::string &dependLog)
	{
		releaseDepend();

		m_DependLog = fopen(dependLog.c_str(), "wt");
		fwrite(s_DependHeader.c_str(), 1, s_DependHeader.length(), m_DependLog);
		fwrite("\n", 1, 1, m_DependLog);
		// fflush(m_DependLog);
	}

	inline void writeError(TError type, const char *path, const char *error, ...)
	{
		if (m_ErrorLog)
		{
			switch (type)
			{
			case ERROR:
				fwrite("ERROR", 1, 5, m_ErrorLog);
				break;
			case WARNING:
				fwrite("WARNING", 1, 7, m_ErrorLog);
				break;
			case MESSAGE:
				fwrite("MESSAGE", 1, 7, m_ErrorLog);
				break;
			}
			fwrite("\t", 1, 1, m_ErrorLog);
			fprintf(m_ErrorLog, "%s", path);
			fwrite("\t", 1, 1, m_ErrorLog);
			std::string time = NLMISC::toString(NLMISC::CTime::getSecondsSince1970());
			fwrite(time.c_str(), 1, time.length(), m_ErrorLog);
			fwrite("\t", 1, 1, m_ErrorLog);
			va_list args;
			va_start(args, error);
			vfprintf(m_ErrorLog, error, args);
			va_end(args);
			fwrite("\n", 1, 1, m_ErrorLog);
			fflush(m_ErrorLog);
		}
	}

	/// inputFile can only be file. [? May be not-yet-existing file for expected input for future build runs. ?] Directories are handled on process level. [? You should call this before calling writeError on inputFile, so the error is also linked from the outputFile. ?]
	inline void writeDepend(TDepend type, const char *outputFile, const char *inputFile)
	{
		if (m_DependLog)
		{
			switch (type)
			{
			case BUILD:
				fwrite("BUILD", 1, 5, m_DependLog);
				break;
			case DIRECTORY:
				fwrite("DIRECTORY", 1, 9, m_DependLog);
				break;
			case RUNTIME:
				fwrite("RUNTIME", 1, 7, m_DependLog);
				break;
			}
			fwrite("\t", 1, 1, m_DependLog);
			fprintf(m_DependLog, "%s", outputFile);
			fwrite("\t", 1, 1, m_DependLog);
			fprintf(m_DependLog, "%s", inputFile);
			fwrite("\n", 1, 1, m_DependLog);
			// fflush(m_DependLog);
		}
	}

	inline void releaseError()
	{
		if (m_ErrorLog)
		{
			fflush(m_ErrorLog);
			fclose(m_ErrorLog);
			m_ErrorLog = NULL;
		}
	}

	inline void releaseDepend()
	{
		if (m_DependLog)
		{
			fflush(m_DependLog);
			fclose(m_DependLog);
			m_DependLog = NULL;
		}
	}

	inline void release()
	{
		releaseError();
		releaseDepend();
	}
}; /* class CToolLogger */

} /* namespace NLMISC */

#endif /* #ifndef NLMISC_TOOL_LOGGER_H */

/* end of file */
