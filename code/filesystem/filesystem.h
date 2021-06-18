#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "filesystem/datastream.h"

class IFileSystem
{
public:
	static IFileSystem* GetFileSystem();

public:
	virtual void Initialize() = 0;

	virtual void AddPath(const std::string& name, const std::string& path) = 0;

	virtual bool Exist(const char* filename) = 0;
	virtual bool Exist(const char* pathname, const char* filename) = 0;

	virtual std::string GetDefaultPath() = 0;
	virtual std::string GetFileName(const std::string& filename) = 0;
	virtual std::string GetFilePath(const std::string& path, const std::string& filename) = 0;
	virtual std::string GetPath(const std::string& path) = 0;


	virtual DataStreamPtr OpenReadStream(const std::string& filename) = 0;
	virtual DataStreamPtr OpenWriteStream(const std::string& filename) = 0;

	virtual DataStreamPtr OpenReadStream(const std::string& path, const std::string& filename) = 0;
	virtual DataStreamPtr OpenWriteStream(const std::string& path, const std::string& filename) = 0;
};

std::string ReadStreamToString(std::unique_ptr<DataStream>& stream);

#endif // !FILESYSTEM_H
