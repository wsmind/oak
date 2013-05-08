#pragma once

#include <string>

namespace oak {

class Log
{
	public:
		static void info(const std::string &message);
		static void warning(const std::string &message);
		static void error(const std::string &message);
		//static void assert(bool condition, const std::string &conditionString, const std::string &message);
};

#define OAK_ASSERT(condition, message) \
	Log::assert(condition, #condition, #message)

} // oak namespace
