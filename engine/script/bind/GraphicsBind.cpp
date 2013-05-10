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

#include <engine/script/bind/GraphicsBind.hpp>

#include <engine/graphics/GraphicsEngine.hpp>
#include <engine/system/Log.hpp>

#include <lua.hpp>

namespace oak {

void GraphicsBind::registerFunctions(lua_State *L, GraphicsEngine *graphics)
{
	// temp function
	/*luaL_Reg api[] = {
		{"setBackgroundColor", GraphicsBind::setBackgroundColor},
		{NULL, NULL}
	};
	luaL_register(L, "graphics", api);*/
	lua_register(L, "graphics_setBackgroundColor", GraphicsBind::setBackgroundColor);
	
	// save pointer for later
	lua_pushlightuserdata(L, graphics);
	lua_setfield(L, LUA_REGISTRYINDEX, "__oak_graphics");
}

int GraphicsBind::setBackgroundColor(lua_State *L)
{
	Log::info("Set background color!!!");
	
	lua_getfield(L, LUA_REGISTRYINDEX, "__oak_graphics");
	GraphicsEngine *graphics = (GraphicsEngine *)lua_touserdata(L, -1);
	lua_pop(L, 1);
	
	double r = lua_tonumber(L, -3);
	double g = lua_tonumber(L, -2);
	double b = lua_tonumber(L, -1);
	lua_pop(L, 3);
	
	graphics->setBackgroundColor(glm::vec3((float)r, (float)g, (float)b));
	
	return 3;
}

} // oak namespace
