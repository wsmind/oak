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

#include <engine/startup/_android/AndroidActivity.hpp>

#include <engine/system/Log.hpp>

// ugly hack;
AAssetManager *assetManager = NULL;

namespace oak {

void AndroidActivity::run(android_app *androidApp)
{
	Log::info("Oak application startup");
	
	this->initialized = false;
	this->animating = false;
	
	androidApp->userData = this; // save this for later, when receiving the android_app in callbacks
	
	// hook to various events
	androidApp->onAppCmd = AndroidActivity::onAppCmd;
	androidApp->onInputEvent = AndroidActivity::onInputEvent;
	
	// ugly hack
	assetManager = androidApp->activity->assetManager;
	
	// main loop
	bool running = true;
	while (running)
	{
		int events;
		android_poll_source *source;
		
		// handle application events
		// this call is blocking only when not animating, to avoid
		// wasting battery life when the app is not active (e.g out of focus)
		while (ALooper_pollAll(this->animating ? 0 : -1, NULL, &events, (void **)&source) >= 0)
		{
			// process this event
			if (source)
				source->process(androidApp, source);
			
			// check if we should exit
			if (androidApp->destroyRequested)
				running = false;
		}
		
		// run a frame
		if (this->initialized)
			this->gameApplication.update(0.0f);
		
		// swap front and back buffers
		eglSwapBuffers(this->eglDisplay, this->eglSurface);
	}
}

void AndroidActivity::createWindow(ANativeWindow *window)
{
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 4,
		EGL_GREEN_SIZE, 4,
		EGL_RED_SIZE, 4,
		EGL_DEPTH_SIZE, 16,
		EGL_NONE
	};
	
	this->eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(this->eglDisplay, 0, 0);
	
	EGLConfig config;
	EGLint configCount;
	eglChooseConfig(this->eglDisplay, attribs, &config, 1, &configCount);
	
	EGLint format;
	eglGetConfigAttrib(this->eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
	ANativeWindow_setBuffersGeometry(window, 0, 0, format);
	
	this->eglSurface = eglCreateWindowSurface(this->eglDisplay, config, window, NULL);
	
	const EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	this->eglContext = eglCreateContext(this->eglDisplay, config, NULL, contextAttribs);
	
	if (eglMakeCurrent(this->eglDisplay, this->eglSurface, this->eglSurface, this->eglContext) == EGL_FALSE) {
		Log::error("Failed to set the current EGL context");
	}
	
	// the context is ready, start application
	this->gameApplication.initialize("hello");
	this->initialized = true;
}

void AndroidActivity::destroyWindow()
{
	// uninitialize application before killing the context
	this->gameApplication.shutdown();
	this->initialized = false;
	
	eglMakeCurrent(this->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(this->eglDisplay, this->eglContext);
	eglDestroySurface(this->eglDisplay, this->eglSurface);
	eglTerminate(this->eglDisplay);
}

void AndroidActivity::startAnimating()
{
	this->animating = true;
}

void AndroidActivity::stopAnimating()
{
	this->animating = false;
}

void AndroidActivity::onAppCmd(android_app *androidApp, int32_t command)
{
	AndroidActivity *activity = (AndroidActivity *)androidApp->userData;
	
	switch (command)
	{
		case APP_CMD_INIT_WINDOW:
		{
			activity->createWindow(androidApp->window);
			break;
		}
		
		case APP_CMD_TERM_WINDOW:
		{
			activity->destroyWindow();
			break;
		}
		
		case APP_CMD_GAINED_FOCUS:
		{
			activity->startAnimating();
			break;
		}
		
		case APP_CMD_LOST_FOCUS:
		{
			activity->stopAnimating();
			break;
		}
	}
}

int32_t AndroidActivity::onInputEvent(android_app *androidApp, AInputEvent* event)
{
	return 0;
}

} // oak namespace
