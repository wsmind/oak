// embed the native activity glue from the NDK
extern "C" {
#include <android_native_app_glue.c>
}

#include <engine/system/Log.hpp>

using namespace oak;

void android_main(android_app *state)
{
	Log::info("plop from android!\n");
}
