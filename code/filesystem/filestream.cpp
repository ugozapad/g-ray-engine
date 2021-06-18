#include "stdafx.h"
#include "filestream.h"

FileStream::FileStream(const char* filename, const char* mode)
{
#ifdef _MSC_VER
	std::ios_base::openmode openmode = 0;
#else
	std::ios_base::openmode openmode = (std::ios_base::openmode)0;
#endif // _MSC_VER

    if (mode[0] == 'r')
        openmode |= std::ios_base::in;
    if (mode[0] == 'w')
        openmode |= std::ios_base::out;

    if (mode[1] == 'b')
        openmode |= std::ios_base::binary;

    m_FileHandle.open(filename, openmode);
    ASSERT(m_FileHandle.is_open());
}

FileStream::~FileStream()
{
    if (m_FileHandle.is_open())
        m_FileHandle.close();
}

void FileStream::Read(void* buffer, size_t count)
{
    m_FileHandle.read((char*)buffer, count);
}

void FileStream::Write(void* buffer, size_t count)
{
    m_FileHandle.write((char*)buffer, count);
}

void FileStream::Seek(SeekDir dir, long offset)
{
    std::ios_base::seekdir seekDir;

    if (dir == SeekDir::Begin)
        seekDir = std::ios_base::beg;
    else if (dir == SeekDir::Current)
        seekDir = std::ios_base::cur;
    else if (dir == SeekDir::End)
        seekDir = std::ios_base::end;

    m_FileHandle.seekg(offset, seekDir);
}

long FileStream::Tell()
{
    return m_FileHandle.tellg();
}

bool FileStream::Eof()
{
    return m_FileHandle.eof();
}

void FileStream::Flush()
{
    m_FileHandle.flush();
}
