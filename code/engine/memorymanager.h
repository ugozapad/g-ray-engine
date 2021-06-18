#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

class MemoryManager
{
public:
	static MemoryManager& Get();
public:
	MemoryManager();
	~MemoryManager();

	void* Allocate(size_t size);
	void Free(void* ptr);
};

struct MemoryTag
{
	const char* filename;
	int line;
};

class MemoryTagManager
{
public:
	static MemoryTagManager* instance();

public:
	/// Tag is associated with pointer address.
	void addTag(uintptr_t pointerAddress, const char* filename, int line);

	/// Get tag by address.
	MemoryTag& getTag(uintptr_t ptr);

private:
	std::vector<std::pair<uintptr_t, MemoryTag>> m_tags;
};

//template <typename T, typename... Args>
//inline T* mem_new(Args... args)
//{
//	return new(MemoryManager::Get().Allocate(sizeof(T))) T(args...);
//}

template <typename T, typename... Args>
inline T* mem_new(const char* file, int line, Args... args)
{
	// Allocate
	T* block = new(MemoryManager::Get().Allocate(sizeof(T))) T(args...);

	// add memory tag
	MemoryTagManager::instance()->addTag((uintptr_t)block, file, line);
	return block;
}

template <typename T>
inline void mem_delete(T*& mem)
{
	MemoryManager::Get().Free(mem);
	mem = NULL;
}

#define New(CLASS, ...) \
	mem_new<CLASS>(__FILE__, __LINE__, __VA_ARGS__)

template <typename T>
struct StlDeleter
{
public:
	void operator()(T* ptr) const
	{
		mem_delete(ptr);
	}
};

#endif // !MEMORYMANAGER_H
