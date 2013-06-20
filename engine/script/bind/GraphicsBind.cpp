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
#include <engine/graphics/View.hpp>
#include <engine/graphics/components/DemoQuad.hpp>
#include <engine/script/bind/Bind.hpp>

namespace oak {

OAK_BIND_POINTER_TYPE(Camera)
OAK_BIND_POINTER_TYPE(DemoQuad)
OAK_BIND_POINTER_TYPE(View)
OAK_BIND_POINTER_TYPE(World)

OAK_BIND_MODULE(GraphicsEngine)
OAK_BIND_WRET_FUNCTION0(GraphicsEngine, getBackgroundColor)
OAK_BIND_VOID_FUNCTION1(GraphicsEngine, setBackgroundColor, glm::vec3)
OAK_BIND_WRET_FUNCTION1(GraphicsEngine, createView, World *)
OAK_BIND_VOID_FUNCTION1(GraphicsEngine, destroyView, View *)

OAK_BIND_WRET_METHOD0(DemoQuad, getColor)
OAK_BIND_VOID_METHOD1(DemoQuad, setColor, glm::vec3)

OAK_BIND_WRET_METHOD0(View, getPriority)
OAK_BIND_VOID_METHOD1(View, setPriority, int)
OAK_BIND_WRET_METHOD0(View, isEnabled)
OAK_BIND_VOID_METHOD1(View, setEnabled, bool)
OAK_BIND_WRET_METHOD0(View, getCamera)
OAK_BIND_VOID_METHOD1(View, setCamera, Camera *)

void GraphicsBind::registerFunctions(lua_State *L, GraphicsEngine *graphics)
{
	OAK_REGISTER_MODULE(L, GraphicsEngine, graphics, graphics)
	OAK_REGISTER_FUNCTION(L, GraphicsEngine, graphics, getBackgroundColor)
	OAK_REGISTER_FUNCTION(L, GraphicsEngine, graphics, setBackgroundColor)
	OAK_REGISTER_FUNCTION(L, GraphicsEngine, graphics, createView)
	OAK_REGISTER_FUNCTION(L, GraphicsEngine, graphics, destroyView)
	
	OAK_REGISTER_CLASS(L, DemoQuad)
	OAK_REGISTER_METHOD(L, DemoQuad, getColor)
	OAK_REGISTER_METHOD(L, DemoQuad, setColor)
	
	OAK_REGISTER_CLASS(L, View)
	OAK_REGISTER_METHOD(L, View, getPriority)
	OAK_REGISTER_METHOD(L, View, setPriority)
	OAK_REGISTER_METHOD(L, View, isEnabled)
	OAK_REGISTER_METHOD(L, View, setEnabled)
	OAK_REGISTER_METHOD(L, View, getCamera)
	OAK_REGISTER_METHOD(L, View, setCamera)
}

} // oak namespace
