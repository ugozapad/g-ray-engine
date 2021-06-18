#include "stdafx.h"
#include "log.h"
#include "filesystem/filesystem.h"

std::unique_ptr<DataStream> Log::ms_logFile;
std::unique_ptr<DataStream> g_logFile;
std::ofstream g_LogFile;

void Log::Init()
{

}

void Log::Close()
{

}

static char* g_month[12] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
static int g_day_in_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int build_id;

void CalculateBuildNumber()
{
	static int start_day = 9;
	static int start_month = 3;
	static int start_year = 2021;

	// Calculating build
	int build = 0, mnum = 0, dnum, ynum, mcnt;
	char mon[4];
	char buf[128];
	strcpy(buf, __DATE__);
	sscanf(buf, "%s %d %d", mon, &dnum, &ynum);
	for (int i = 0; i < 12; i++) {
		if (stricmp(g_month[i], mon) == 0) {
			mnum = i;
			break;
		}
	}

	build_id = (ynum - start_year) * 365 + dnum - start_day;

	for (int i = 0; i < mnum; ++i)
		build_id += g_day_in_month[i];

	for (int i = 0; i < start_month - 1; ++i)
		build_id -= g_day_in_month[i];
}

void LogInit()
{
#ifdef WIN32
	char UserName[256];
	DWORD dwUserName = 256;

	if (!GetUserNameA(UserName, &dwUserName))
		strcpy(UserName, "unnamed");

	char logfilename[256];
	sprintf(logfilename, "engine_%s.log", UserName);
#else
	char logfilename[256];
	sprintf(logfilename, "engine.log");
#endif
	g_logFile = IFileSystem::GetFileSystem()->OpenWriteStream("logs", logfilename);

	Msg("'%s' build %d, %s", "engine", build_id, __DATE__);
}

void LogClose()
{
	//if (g_LogFile.is_open())
	//	g_LogFile.close();
	g_logFile.release();
}

#include <time.h>

void Msg(const char* msg, ...)
{
	char buffer[1024];
	char buffer2[1024];
	va_list args;

	va_start(args, msg);
	vsnprintf(buffer, sizeof(buffer), msg, args);
	va_end(args);

	strcat(buffer, "\n");

	time_t t = time(0);
	struct tm* ti = localtime(&t);

	char timestr[256];
	sprintf(timestr, "%s", asctime(ti));
	timestr[strlen(timestr) - 1] = '\0';

	sprintf(buffer2, "[%s] %s", timestr, buffer);

	if (g_logFile)
	{
		g_logFile->Write(buffer2, strlen(buffer2));
		g_logFile->Flush();
	}

#ifdef WIN32
	OutputDebugStringA(buffer2);
#else
	fwrite(buffer2, strlen(buffer2), sizeof(char), stdout);
#endif
}

void Error(const char* msg, ...)
{
	char buffer[1024];
	va_list args;

	va_start(args, msg);
	vsnprintf(buffer, sizeof(buffer), msg, args);
	va_end(args);

	Msg("%s", buffer);
	
#ifdef WIN32
	MessageBoxA(0, buffer, "Error", MB_OK | MB_ICONERROR);
#else
	fwrite(buffer, strlen(buffer), sizeof(char), stderr);
#endif

	exit(1);
}
