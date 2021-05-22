#pragma once
#include "pch.h"
#include "IAllocator.h"

class Allocator
{
public:
	virtual void Init(std::span<std::byte>* memory_buffer)
	{


	}

	//should return nullptr if it can't/won't allocate
	virtual void* Allocate(int size)
	{
		if (size == 0)
			return nullptr;


	}

	//should do nothing if passed nullptr, undefined behavior (e.g. crash) if passed a pointer that wasn't previously allocated by the same allocator
	virtual void Free(void* ptr)
	{
		if (ptr == nullptr)
			return;



	}



};
