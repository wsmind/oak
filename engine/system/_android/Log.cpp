#include <engine/system/Log.hpp>

#include <android/log.h>

namespace oak {

void Log::info(const std::string &message)
{
	__android_log_write(ANDROID_LOG_INFO, "oak", message.c_str());
}

void Log::warning(const std::string &message)
{
	__android_log_write(ANDROID_LOG_WARN, "oak", message.c_str());
}

void Log::error(const std::string &message)
{
	__android_log_write(ANDROID_LOG_ERROR, "oak", message.c_str());
}

} // oak namespace
