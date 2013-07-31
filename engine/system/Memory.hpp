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

#pragma once

#include <cstdlib>

namespace oak {

class Memory
{
	public:
		static void *allocate(size_t size);
		static void free(void *ptr);
		
		static void dumpUsedMemory();
		
	private:
		#ifdef OAK_DEBUG
			// additional info stored with each allocated pointer
			struct PointerHeader
			{
				size_t size;
				unsigned long stackHash;
			};
			
			static const unsigned int MAX_STACK_DEPTH = 128;
			static const unsigned int MAX_UNIQUE_CALL_STACKS = 1000;
			
			// agregated allocation stats for one unique back trace
			struct StackAllocationStats
			{
				unsigned long hash;
				void *stack[MAX_STACK_DEPTH];
				size_t totalSize;
				
				StackAllocationStats()
					: hash(0)
					, totalSize(0)
				{}
			};
			static StackAllocationStats allocationStats[MAX_UNIQUE_CALL_STACKS];
			
			// allocation tracking
			static unsigned long trackAllocation(size_t size); // returns call stack hash
			static void trackFree(size_t size, unsigned long stackHash);
		#endif // OAK_DEBUG
};

} // oak namespace
