#include <engine/system/Log.hpp>

#include <windows.h>

namespace oak {

void Log::info(const std::string &message)
{
	OutputDebugString(message.c_str());
}

void Log::warning(const std::string &message)
{
	OutputDebugString(message.c_str());
}

void Log::error(const std::string &message)
{
	OutputDebugString(message.c_str());
}

} // oak namespace
