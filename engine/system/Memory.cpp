/******************************************************************************
 *
 * Oak game engine
 * Copyright (c) 2013 Remi Papillie
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 *****************************************************************************/

#include <engine/system/Memory.hpp>
#include <engine/system/StackWalker.hpp>

#include <engine/system/Log.hpp>

#include <cstring>
#include <cstdio>

// global override for new and delete operators

void *operator new(size_t size)
{
	return oak::Memory::allocate(size);
}

void operator delete(void *ptr)
{
	oak::Memory::free(ptr);
}

void operator delete[](void *ptr)
{
	oak::Memory::free(ptr);
}

namespace oak {

#ifdef OAK_DEBUG
	Memory::StackAllocationStats Memory::allocationStats[MAX_UNIQUE_CALL_STACKS];
#endif

void *Memory::allocate(size_t size)
{
	#ifdef OAK_DEBUG
		// save some space for debug infos
		size += sizeof(PointerHeader);
	#endif
	
	// actual allocation
	void *pointer = ::malloc(size);
	
	#ifdef OAK_DEBUG
		// sample the call stack, and save the hash with the pointer
		PointerHeader *header = (PointerHeader *)pointer;
		header->size = size;
		header->stackHash = Memory::trackAllocation(size);
		
		pointer = (void *)((char *)pointer + sizeof(PointerHeader));
	#endif
	
	return pointer;
}

void Memory::free(void *ptr)
{
	#ifdef OAK_DEBUG
		// find the actual pointer allocated previously
		ptr = (void *)((char *)ptr - sizeof(PointerHeader));
		
		// track the released memory
		PointerHeader *header = (PointerHeader *)ptr;
		Memory::trackFree(header->size, header->stackHash);
	#endif
	
	::free(ptr);
}

void Memory::dumpUsedMemory()
{
	#ifdef OAK_DEBUG
		for (unsigned int i = 0; i < MAX_UNIQUE_CALL_STACKS; i++)
		{
			StackAllocationStats &stats = Memory::allocationStats[i];
			if (stats.hash != 0)
			{
				printf("==== Stack #%d, %d bytes allocated ====\n", i, stats.totalSize);
				StackWalker::SymbolInfo symbolInfo;
				for (unsigned int j = 0; j < MAX_STACK_DEPTH; j++)
				{
					if (stats.stack[j] == 0)
						break;
					
					StackWalker::translateSymbol(stats.stack[j], &symbolInfo);
					printf("0x%x %s (%s:%d)\n", (unsigned int)stats.stack[j], symbolInfo.name, symbolInfo.file, symbolInfo.line);
				}
			}
		}
	#endif // OAK_DEBUG
}

#ifdef OAK_DEBUG

unsigned long Memory::trackAllocation(size_t size)
{
	void *stack[MAX_STACK_DEPTH] = {0};
	unsigned long stackHash;
	StackWalker::dumpStack(stack, MAX_STACK_DEPTH, &stackHash);
	
	// look for this call stack, starting at the hash index
	for (unsigned int i = 0; i < MAX_UNIQUE_CALL_STACKS; i++)
	{
		unsigned int index = (stackHash + i) % MAX_UNIQUE_CALL_STACKS;
		
		StackAllocationStats &stats = Memory::allocationStats[index];
		if (stats.hash == stackHash)
		{
			stats.totalSize += size;
			return stackHash;
		}
	}
	
	// find a free slot, and store the stack here
	// look for this call stack, starting at the hash index
	for (unsigned int i = 0; i < MAX_UNIQUE_CALL_STACKS; i++)
	{
		unsigned int index = (stackHash + i) % MAX_UNIQUE_CALL_STACKS;
		
		StackAllocationStats &stats = Memory::allocationStats[index];
		if (stats.hash == 0)
		{
			stats.hash = stackHash;
			memcpy(stats.stack, stack, sizeof(stack));
			stats.totalSize = size;
			return stackHash;
		}
	}
	
	// too many unique call stacks are being tracked, the array is full
	
	return stackHash;
}

void Memory::trackFree(size_t size, unsigned long stackHash)
{
	// look for this call stack, starting at the hash index
	for (unsigned int i = 0; i < MAX_UNIQUE_CALL_STACKS; i++)
	{
		unsigned int index = (stackHash + i) % MAX_UNIQUE_CALL_STACKS;
		
		StackAllocationStats &stats = Memory::allocationStats[index];
		if (stats.hash == stackHash)
		{
			stats.totalSize -= size;
			
			// free stack info if everything has been deallocated
			if (stats.totalSize == 0)
			{
				stats.hash = 0;
				break;
			}
		}
	}
}

#endif // OAK_DEBUG

} // oak namespace
