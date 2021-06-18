#include "stdafx.h"
#include "memorymanager.h"

FILE* gMemFile = NULL;
std::vector<uintptr_t> gUnfreedMem;

MemoryManager& MemoryManager::Get()
{
	static MemoryManager s_memoryManager;
	return s_memoryManager;
}

MemoryManager::MemoryManager()
{
	gMemFile = fopen("MEM.TXT", "w");
}

MemoryManager::~MemoryManager()
{
	if (gUnfreedMem.empty())
		fprintf(gMemFile, "all blocks is freed\n");

	for (auto it : gUnfreedMem)
	{
		auto& tag = MemoryTagManager::instance()->getTag(it);
		fprintf(gMemFile, "!!! UNFREED BLOCK %p \n", (void*)it);
		fprintf(gMemFile, "%s:%i\n", tag.filename, tag.line);
	}

	fclose(gMemFile);
}

void* MemoryManager::Allocate(size_t size)
{
	void* memblock = (void*)malloc(size);
	if (!memblock)
		Error("MemoryManager::Allocate: Failed to allocate %i bytes.", size);

	fprintf(gMemFile, "allocate %p with size %i\n", memblock, size);
	fflush(gMemFile);

	gUnfreedMem.push_back((uintptr_t)memblock);
	return memblock;
}

void MemoryManager::Free(void* ptr)
{
	ASSERT(ptr);
	free(ptr);

	auto unfreedptr = std::find(gUnfreedMem.begin(), gUnfreedMem.end(), (uintptr_t)ptr);
	if (unfreedptr != gUnfreedMem.end())
		gUnfreedMem.erase(unfreedptr);

	fprintf(gMemFile, "free %p\n", ptr);
	fflush(gMemFile);
}

static MemoryTagManager* s_memoryTagManager;

MemoryTagManager* MemoryTagManager::instance()
{
	if (!s_memoryTagManager)
		s_memoryTagManager = new MemoryTagManager;

	return s_memoryTagManager;
}

void MemoryTagManager::addTag(uintptr_t pointerAddress, const char* filename, int line)
{
	m_tags.push_back(std::make_pair(pointerAddress, MemoryTag{ filename, line }));
}

MemoryTag& MemoryTagManager::getTag(uintptr_t ptr)
{
	for (int i = 0; i < m_tags.size(); i++)
		if (m_tags[i].first == ptr)
			return m_tags[i].second;
}
