#pragma once
#include <string>

class Logger
{
public:
	static void initLogger(const std::string&);
	static void debug(const std::string&);
	static void info(const std::string&);
	static void warning(const std::string&);
	static void error(const std::string&);
};