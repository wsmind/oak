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

#include <GL/glfw.h>

#include <engine/app/Application.hpp>
#include <engine/system/Log.hpp>

using namespace oak;

#ifdef EMSCRIPTEN
#include <emscripten.h>

Application *app = NULL;
void main_loop()
{
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);
	
	app->update(dt);
	
	glfwSwapBuffers();
}
#endif

int main()
{
	Log::info("plop from glfw platforms!");
	
	glfwInit();
	
	if (glfwOpenWindow(1280, 800, 8, 8, 8, 8, 24, 8, GLFW_WINDOW) == GL_FALSE)
	{
		Log::error("Failed to open a GLFW window! Exiting...");
		glfwTerminate();
		
		return 1;
	}
	
	Application *application = new Application;
	
	application->initialize("D:/proj/oak/samples/hello");
	
	glfwSetTime(0.0);
	
	#ifdef EMSCRIPTEN
		app = application;
		emscripten_set_main_loop(main_loop, 0, 0);
		return 0;
	#else
		while (glfwGetWindowParam(GLFW_OPENED))
		{
			float dt = (float)glfwGetTime();
			glfwSetTime(0.0);
			
			application->update(dt);
			
			glfwSwapBuffers();
		}
	#endif
	
	application->shutdown();
	
	delete application;
	
	glfwCloseWindow();
	glfwTerminate();
	
	return 0;
}
