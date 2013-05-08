#include <engine/system/Log.hpp>

#include <stdio.h>

namespace oak {

void Log::info(const std::string &message)
{
	printf("[INFO] %s", message.c_str());
}

void Log::warning(const std::string &message)
{
	printf("[WARN] %s", message.c_str());
}

void Log::error(const std::string &message)
{
	printf("[ERR ] %s", message.c_str());
}

} // oak namespace
