#include "stdafx.h"
#include "datastream.h"

DataStream::DataStream()
{
	m_data = 0;
	m_size = 0;
	m_position = 0;
	m_readOnly = 0;
}

DataStream::DataStream(void* data, size_t size, bool readonly)
{
	m_data = (uint8_t*)data;
	m_size = size;
	m_position = 0;
	m_readOnly = readonly;
}

DataStream::~DataStream()
{
	if (m_data)
		free(m_data);

	m_data = 0;
	m_size = 0;
	m_position = 0;
	m_readOnly = 0;
}

void DataStream::Read(void* buffer, size_t count)
{
	if (m_position >= Tell())
		return;
	if (m_position + count > Tell())
		return;

	memcpy(buffer, m_data + m_position, count);
	m_position += count;
}

void DataStream::Write(void* buffer, size_t count)
{
	if (m_readOnly)
		return;
	if (m_position >= Tell())
		return;
	if (m_position + count > Tell())
		return;

	memcpy(m_data + m_position, buffer, count);
	m_position += count;
}

void DataStream::Seek(SeekDir dir, long offset)
{
	switch (dir)
	{
	case SeekDir::Begin:
		m_position = 0;
		m_position += offset;
		break;
	case SeekDir::Current:
		m_position += offset;
		break;
	case SeekDir::End:
		m_position = m_size;
		m_position += offset;
		break;
	}
}

long DataStream::Tell()
{
    return m_position;
}

bool DataStream::Eof()
{
	return m_position >= m_size;
}

void DataStream::Flush()
{

}
