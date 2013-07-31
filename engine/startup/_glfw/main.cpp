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
//#include <engine/system/Memory.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using namespace oak;

#ifdef EMSCRIPTEN
#include <emscripten.h>

Application *app = NULL;
void main_loop()
{
	app->update();
	
	glfwSwapBuffers();
}
#endif

int main(int argc, char **argv)
{
	// the engine must be given a game folder
	if (argc < 2)
	{
		Log::error("No game folder given on the command-line, aborting");
		std::cout << "Usage: " << argv[0] << " <game folder>" << std::endl;
		return 1;
	}
	
	// normalize game folder
	std::string gameFolder = std::string(argv[1]);
	std::replace(gameFolder.begin(), gameFolder.end(), '\\', '/');
	if (gameFolder[gameFolder.size() - 1] != '/')
		gameFolder += '/';
	
	// the game must contain at least a file named main.lua
	std::string mainScriptFilename = gameFolder + "main.lua";
	std::ifstream mainScriptFile(mainScriptFilename.c_str());
	if (mainScriptFile.fail())
	{
		Log::error("Entry script main.lua not found in game folder, aborting");
		std::cout << "The game folder must contain a main.lua file" << std::endl;
		return 1;
	}
	mainScriptFile.close();
	
	glfwInit();
	
	if (glfwOpenWindow(1280, 800, 8, 8, 8, 8, 24, 8, GLFW_WINDOW) == GL_FALSE)
	{
		Log::error("Failed to open a GLFW window! Exiting...");
		glfwTerminate();
		
		return 1;
	}
	
	// events will be polled from the input engine; no need to poll them automatically each frame
	glfwDisable(GLFW_AUTO_POLL_EVENTS);
	
	// clear possible GL errors internal to glfw
	glGetError();
	
	Application *application = new Application;
	
	application->initialize(gameFolder);
	
	#ifdef EMSCRIPTEN
		app = application;
		emscripten_set_main_loop(main_loop, 0, 0);
		return 0;
	#else
		while (glfwGetWindowParam(GLFW_OPENED))
		{
			application->update();
			
			glfwSwapBuffers();
			
			//Memory::dumpUsedMemory();
		}
	#endif
	
	application->shutdown();
	
	delete application;
	
	glfwCloseWindow();
	glfwTerminate();
	
	return 0;
}
