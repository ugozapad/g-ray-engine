#include "stdafx.h"
#include "filesystem.h"
#include "filestream.h"
#include <memory>

class FileSystem : public IFileSystem
{
public:
	virtual void Initialize();

	virtual void AddPath(const std::string& name, const std::string& path);

	virtual bool Exist(const char* filename);
	virtual bool Exist(const char* pathname, const char* filename);

	virtual std::string GetDefaultPath();
	virtual std::string GetFileName(const std::string& filename);
	virtual std::string GetFilePath(const std::string& path, const std::string& filename);
	virtual std::string GetPath(const std::string& path);


	virtual DataStreamPtr OpenReadStream(const std::string& filename);
	virtual DataStreamPtr OpenWriteStream(const std::string& filename);

	virtual DataStreamPtr OpenReadStream(const std::string& path, const std::string& filename);
	virtual DataStreamPtr OpenWriteStream(const std::string& path, const std::string& filename);

private:
	typedef std::unordered_map<std::string, std::string>::iterator PathsIterator;

	std::unordered_map<std::string, std::string> ms_paths;
	std::vector<std::string> ms_files;

};

IFileSystem* IFileSystem::Instance()
{
	static FileSystem s_fileSystem;
	return (IFileSystem*)&s_fileSystem;
}

#ifdef _WIN32
#include <io.h> 
#define access    _access_s
#else
#include <unistd.h>
#endif

bool DirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

bool operator==(const std::pair<std::string, std::string>& lhs, const std::pair<std::string, std::string>& rhs) {
	return lhs.first == rhs.first;
}

bool FileSystem::Exist(const char* filename)
{
	return access(filename, 0) == 0;
}

bool FileSystem::Exist(const char* pathname, const char* filename)
{
	std::string finfilename;
	finfilename += GetPath(pathname);
	finfilename += "/";
	finfilename += filename;

	return Exist(finfilename.c_str());
}

std::string FileSystem::GetDefaultPath()
{
	return "data";
}

std::string FileSystem::GetFileName(const std::string& filename)
{
	std::string new_filename = filename.substr(filename.find_last_of('/') + 1);
	return new_filename.substr(0, new_filename.find('.'));
}

std::string FileSystem::GetFilePath(const std::string& path, const std::string& filename)
{
	PathsIterator fullpath;
	for (fullpath = ms_paths.begin(); fullpath != ms_paths.end(); ++fullpath)
		if (fullpath->first == path)
			break;

	if (fullpath == ms_paths.end())
		Error("FileSystem::GetFilePath: Failed to find path '%s', because this path is not exist.", path.c_str());

	std::string buf;
	buf += fullpath->second;
	buf += "/";
	buf += filename;

	return buf;
}

std::string FileSystem::GetPath(const std::string& path)
{
	for (PathsIterator it = ms_paths.begin(); it != ms_paths.end(); ++it)
		if (it->first == path)
			return it->second;
}

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>

void RecursiveSearch(const std::string& path, std::vector<std::string>& files)
{
	struct _finddata_t c_file;
	intptr_t hFile;
	
	std::string bufferPath;
	bufferPath += path;
	bufferPath += "/";
	bufferPath += "*.*";

	if ((hFile = _findfirst(bufferPath.c_str(), &c_file)) != -1L)
	{
		do {
			if (strcmp(c_file.name, ".") == 0 ||
				strcmp(c_file.name, "..") == 0 ||
				strcmp(c_file.name, ".git") == 0 ||
				strcmp(c_file.name, ".gitignore") == 0 ||
				strcmp(c_file.name, ".gitignore") == 0 ||
				strcmp(c_file.name, ".vs") == 0)
				continue;

			if (c_file.attrib & _A_SUBDIR)
			{
				std::string nextPath;
				nextPath += path;
				nextPath += "/";
				nextPath += c_file.name;

				RecursiveSearch(nextPath, files);
			}
			else
			{
				std::string filepath;
				filepath += path;
				filepath += "/";
				filepath += c_file.name;
				files.push_back(filepath);
			}

		} while (_findnext(hFile, &c_file) == 0);
		_findclose(hFile);
	}
}

void FileSystem::Initialize()
{
	// Register base paths.
	AddPath("app_data", "app_data");
	AddPath("logs", "app_data/logs");
	AddPath("screenshots", "app_data/screenshots");

	// register game data.
	AddPath("textures", "data/textures");
	AddPath("models", "data/models");
	AddPath("levels", "data/levels");
	AddPath("maps", "data/maps");
	AddPath("materials", "data/materials");
	AddPath("scripts", "data/scripts");
	AddPath("shaders", "data/shaders");

	RecursiveSearch(GetDefaultPath(), ms_files);
	Msg("total %i files", ms_files.size());
}

void FileSystem::AddPath(const std::string& name, const std::string& path)
{
	if (!Exist(path.c_str()))
		CreateDirectoryA(path.c_str(), NULL);

	ms_paths[name] = path;
}

DataStreamPtr FileSystem::OpenReadStream(const std::string& filename)
{
	if (!FileSystem::Exist(filename.c_str()))
		Error("FileSystem::OpenReadStream: File '%s' is not exist.", filename.c_str());

	return std::make_unique<FileStream>(filename.c_str(), "rb");
}

DataStreamPtr FileSystem::OpenWriteStream(const std::string& filename)
{
	return std::make_unique<FileStream>(filename.c_str(), "wb");
}

DataStreamPtr FileSystem::OpenReadStream(const std::string& path, const std::string& filename)
{
	std::string name;
	name += GetPath(path);
	name += "/";
	name += filename;

	if (!FileSystem::Exist(name.c_str()))
		Error("FileSystem::OpenReadStream: File '%s' is not exist.", filename.c_str());

	return OpenReadStream(name);
}

DataStreamPtr FileSystem::OpenWriteStream(const std::string& path, const std::string& filename)
{
	std::string name;
	name += GetPath(path);
	name += "/";
	name += filename;

	return OpenWriteStream(name);
}

std::string ReadStreamToString(std::unique_ptr<DataStream>& stream)
{
	stream->Seek(SeekDir::End, 0);
	size_t length = stream->Tell();
	stream->Seek(SeekDir::Begin, 0);

	char* buffer = (char*)malloc(length + 1);
	stream->Read(buffer, length);
	buffer[length] = '\0';

	std::string returnBuffer;
	returnBuffer += buffer;

	free(buffer);

	return returnBuffer;
}
