#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "datastream.h"

class FileStream : public DataStream
{
public:
	FileStream(const char* filename, const char* mode);
	~FileStream();

    void Read(void* buffer, size_t count);
    void Write(void* buffer, size_t count);

    void Seek(SeekDir dir, long offset);
    long Tell();

    bool Eof();

    void Flush();

private:
    std::fstream m_FileHandle;
};

#endif // FILESTREAM_H
