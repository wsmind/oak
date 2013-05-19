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

#include <engine/script/bind/SystemBind.hpp>

#include <engine/script/bind/Bind.hpp>
#include <engine/script/bind/SystemWrapper.hpp>

namespace oak {

OAK_BIND_MODULE(SystemWrapper)
OAK_BIND_VOID_FUNCTION1(SystemWrapper, logInfo, std::string)
OAK_BIND_VOID_FUNCTION1(SystemWrapper, logWarning, std::string)
OAK_BIND_VOID_FUNCTION1(SystemWrapper, logError, std::string)

void SystemBind::registerFunctions(lua_State *L, SystemWrapper *system)
{
	OAK_REGISTER_MODULE(L, SystemWrapper, system, system)
	OAK_REGISTER_FUNCTION(L, SystemWrapper, system, logInfo)
	OAK_REGISTER_FUNCTION(L, SystemWrapper, system, logWarning)
	OAK_REGISTER_FUNCTION(L, SystemWrapper, system, logError)
}

} // oak namespace
