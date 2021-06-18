#ifndef LOG_H
#define LOG_H

void LogInit();
void LogClose();
void Msg(const char* msg, ...);

void Error(const char* msg, ...);

#include "filesystem/datastream.h"

class Log
{
public:
	static void Init();
	static void Close();

private:
	static std::unique_ptr<DataStream> ms_logFile;
};

#endif // !LOG_H
