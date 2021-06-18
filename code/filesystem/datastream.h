#ifndef DATASTREAM_H
#define DATASTREAM_H

enum class SeekDir
{
	Begin,
	Current,
	End
};

class DataStream
{
public:
	DataStream();
	DataStream(void* data, size_t size, bool readonly);
	virtual ~DataStream();

	virtual void Read(void* buffer, size_t count);
	virtual void Write(void* buffer, size_t count);

	virtual void Seek(SeekDir dir, long offset);
	virtual long Tell();

	virtual bool Eof();

	virtual void Flush();

    template <typename T>
	void Read(T* buffer) { return Read((void*)buffer, sizeof(T)); }

	template <typename T>
	void Write(T* buffer) { return Write((void*)buffer, sizeof(T)); }

private:
	uint8_t* m_data;
	size_t m_size, m_position;

	bool m_readOnly;
};

typedef std::unique_ptr<DataStream> DataStreamPtr;

#endif // DATASTREAM_H
