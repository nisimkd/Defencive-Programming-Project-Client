#include "Logger.h"

#include <iostream>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

void Logger::initLogger(const std::string& logFileName)
{
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

    boost::log::add_file_log(boost::log::keywords::file_name = logFileName, boost::log::keywords::auto_flush = true, boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%");

    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);    

    boost::log::add_common_attributes();    
}

void Logger::debug(const std::string& message)
{
    BOOST_LOG_TRIVIAL(debug) << message;
}

void Logger::info(const std::string& message)
{
    BOOST_LOG_TRIVIAL(info) << message;
}

void Logger::warning(const std::string& message)
{
    BOOST_LOG_TRIVIAL(warning) << message;
}

void Logger::error(const std::string& message)
{
    BOOST_LOG_TRIVIAL(error) << message;
}