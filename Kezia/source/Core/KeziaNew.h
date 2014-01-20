#pragma once

#include "Core/Types.h"
#include "Core/PoolAllocator.h"

#include <new.h>

namespace
{
	Kezia::PoolAllocator g_Allocator;
}

void * operator new(const Kezia::U32 size)
{
	return g_Allocator.Allocate(size);
}

void * operator new(const Kezia::U32 size, const char * file, const Kezia::U32 line)
{
	return g_Allocator.Allocate(size, 0, file, line);
}

void * operator new[](const Kezia::U32 size)
{
	return g_Allocator.Allocate(size);
}

void * operator new[](const Kezia::U32 size, const char * file, const Kezia::U32 line)
{
	return g_Allocator.Allocate(size, 0, file, line);
}

void operator delete(void * pointer)
{
	g_Allocator.Deallocate((Kezia::Byte *)pointer);
}

void operator delete(void * pointer, const char * file, const Kezia::U32 line)
{
	g_Allocator.Deallocate((Kezia::Byte *)pointer);
}

void operator delete[](void * pointer)
{
	g_Allocator.Deallocate((Kezia::Byte *)pointer);
}

void operator delete[](void * pointer, const char * file, const Kezia::U32 line)
{
	g_Allocator.Deallocate((Kezia::Byte *)pointer);
}

void ReportMemoryLeaks()
{
	g_Allocator.ReportMemoryLeaks();
}

#define new new(__FILE__, __LINE__)