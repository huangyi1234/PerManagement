//////////////////////////////////////////////////////////////////
// logger.hpp
//
// Date: 2021/02/26
// Author: yoxia
// Change history:
//      2021/02/26: This is the initialization version.
//
// Discription:
// Implematation class(Clogger)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef LOGGER_HPP
#define LOGGER_HPP
#include<iostream>
#include<string>
#include "Poco/Configurable.h"
#include "Poco/Mutex.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Logger.h"
#include "loggerDef.hpp"
#include "Poco/FileChannel.h"


namespace App_dev{
#define logger_fatal(logger, message) \
	if (logger->getLogger()) \
		(logger->getLogger())->fatal(message); \
	else (void) 0
#define logger_fatal_f(logger, fmt, ...) \
	if (logger->getLogger()) \
		poco_fatal_f(*(logger->getLogger()), fmt, __VA_ARGS__); \
	else (void) 0
#define logger_critical(logger, message) \
	if (logger->getLogger()) \
		(logger->getLogger())->critical(message); \
	else (void) 0
#define logger_critical_f(logger, fmt, ...) \
	if (logger->getLogger()) \
		poco_critical_f(*(logger->getLogger()), fmt, __VA_ARGS__); \
	else (void) 0
#define logger_error(logger, message) \
	if (logger->getLogger()) \
		(logger->getLogger())->error(message); \
	else (void) 0
#define logger_error_f(logger, fmt, ...) \
	if (logger->getLogger()) \
		poco_error_f(*(logger->getLogger()), fmt, __VA_ARGS__); \
	else (void) 0
#define logger_warning(logger, message) \
	if (logger->getLogger()) \
		(logger->getLogger())->warning(message); \
	else (void) 0
#define logger_warning_f(logger, fmt, ...) \
	if (logger->getLogger()) \
		poco_warning_f(*(logger->getLogger()), fmt, __VA_ARGS__); \
	else (void) 0
#define logger_notice(logger, message) \
	if (logger->getLogger()) \
		(logger->getLogger())->notice(message); \
	else (void) 0
#define logger_notice_f(logger, fmt, ...) \
	if (logger->getLogger()) \
		poco_notice_f(*(logger->getLogger()), fmt, __VA_ARGS__); \
	else (void) 0
#define logger_information(logger, message) \
	if (logger->getLogger()) \
		(logger->getLogger())->information(message); \
	else (void) 0
#define logger_information_f(logger, fmt, ...) \
	if (logger->getLogger()) \
		poco_information_f(*(logger->getLogger()), fmt, __VA_ARGS__); \
	else (void) 0
#define logger_debug(logger, message) \
	if (logger->getLogger()) \
		(logger->getLogger())->debug(message); \
	else (void) 0
#define logger_debug_f(logger, fmt, ...) \
	if (logger->getLogger()) \
		poco_debug_f(*(logger->getLogger()), fmt, __VA_ARGS__); \
	else (void) 0
#define logger_trace(logger, message) \
	if (logger->getLogger()) \
		(logger->getLogger())->trace(message); \
	else (void) 0
#define logger_trace_f(logger, fmt, ...) \
	if (logger->getLogger()) \
		poco_trace_f(*(logger->getLogger()), fmt, __VA_ARGS__); \
	else (void) 0

//Clogger class
class Clogger: public Poco::Configurable, public Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<Clogger>;
	///////////////////////////////////////////
	//construction functions
	//Clogger(): output to console
	Clogger(const std::string &name);
	virtual ~Clogger();
	//////////////////////////////////////////
	//configuration property
	void setProperty(const std::string& name, const std::string& value);
	// Returns the value of the property with the given name.
	// See setProperty() for a description of the supported
	// properties.
	std::string getProperty(const std::string& name) const;
	//set level
	void setLevel(const std::string& name);
	std::string getLevel() const;
	///////////////////////////////////////////
	//output log
	void error(const std::string& msg);
	template <typename T, typename... Args>
	void error(const std::string& msg, T& agr1, Args&... args);
	Poco::Logger::Ptr getLogger() const;

private:

protected:
	Poco::FormattingChannel::Ptr _pFormatC;
	std::string _format;
	std::string _level;
	Poco::Logger::Ptr _pLogger;
	std::string _nameLogger;
	int parseLevel(const std::string &&level);
	int parseLevel(const std::string &level);
	
};

template <typename T, typename... Args>
void Clogger::error(const std::string& msg, T& agr1, Args&... args)
{
	if(_pLogger){
		_pLogger->error(msg, agr1, std::forward<Args>(args)...);
	}
}

}

#endif
