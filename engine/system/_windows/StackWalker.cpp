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

#ifdef OAK_DEBUG

#include <engine/system/StackWalker.hpp>

// Unfortunately, the DbgHelp APIs are not available under MinGW. Thus, stack walking
// and symbol translation is only available in MSVC on windows.
#ifdef _MSC_VER
#	include <windows.h>
#	include <dbghelp.h>
#endif

namespace oak {

void StackWalker::dumpStack(void **backTrace, unsigned int backTraceSize, unsigned long *backTraceHash)
{
	#ifdef _MSC_VER
		CaptureStackBackTrace(3, backTraceSize, backTrace, backTraceHash);
	#endif
}

void StackWalker::translateSymbol(void *address, SymbolInfo *info)
{
	#ifdef _MSC_VER
		static bool dbgHelpInitialized = false;
		
		// initialize library if not done yet
		if (!dbgHelpInitialized)
		{
			SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
			SymInitialize(GetCurrentProcess(), NULL, TRUE);
			
			dbgHelpInitialized = true;
		}
		
		DWORD64 displacement = 0;
		
		char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)];
		PSYMBOL_INFO symbol = (PSYMBOL_INFO)symbolBuffer;
		
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME;
		
		if (SymFromAddr(GetCurrentProcess(), (DWORD64)address, &displacement, symbol))
		{
			strncpy(info->name, symbol->Name, sizeof(info->name) - 1);
		}
		else
		{
			strcpy(info->name, "Symbol not found");
		}
		
		IMAGEHLP_LINE64 line;
		DWORD displacement2;
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
		
		if (SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)address, &displacement2, &line))
		{
			strncpy(info->file, line.FileName, sizeof(info->file) - 1);
			info->line = line.LineNumber;
		}
		else
		{
			info->file[0] = '\0';
			info->line = 0;
		}
	#endif // _MSC_VER
}

} // oak namespace

#endif // OAK_DEBUG
