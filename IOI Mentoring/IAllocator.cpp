#include "pch.h"
#include "IAllocator.h"

class IAllocator
{
public:
	//another exercise, study std::span and replace this API to use spans instead of pointers+size
	virtual void Init(std::span<std::byte> memory_buffer) = 0;

	//should return nullptr if it can't/won't allocate
	virtual void* Allocate(int size) = 0;

	//should do nothing if passed nullptr, undefined behavior (e.g. crash) if passed a pointer that wasn't previously allocated by the same allocator
	virtual void Free(void* ptr) = 0;
};