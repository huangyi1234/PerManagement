//////////////////////////////////////////////////////////////////
// logger.cpp
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
#include "logger.hpp"
#include "loggerDef.hpp"
#include "Poco/PatternFormatter.h"
#include "Poco/ConsoleChannel.h"

namespace App_dev{

//define class logger
Clogger::Clogger(const std::string &name):
	Poco::Configurable(),
	_pFormatC(0),
	_level(LOGGER_PRIORITY_ERROR),
	_pLogger(0),
	_nameLogger(name)
{

}

int Clogger::parseLevel(const std::string &level)
{
	if(level.c_str() == std::string(LOGGER_PRIORITY_NONE)){
		return(0);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_FATAL)){
		return(Poco::Message::PRIO_FATAL);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_CRITICAL)){
		return(Poco::Message::PRIO_CRITICAL);
	}else if(level == std::string(LOGGER_PRIORITY_ERROR)){
		return(Poco::Message::PRIO_ERROR);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_WARNING)){
		return(Poco::Message::PRIO_WARNING);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_NOTICE)){
		return(Poco::Message::PRIO_NOTICE);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_INFORMATION)){ 
		return(Poco::Message::PRIO_INFORMATION);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_DEBUG)){ 
		return(Poco::Message::PRIO_DEBUG);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_TRACE)){ 
		return(Poco::Message::PRIO_TRACE);
	}else{
		return(0);
	}
}

int Clogger::parseLevel(const std::string &&level)
{
	if(level.c_str() == std::string(LOGGER_PRIORITY_NONE)){
		return(0);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_FATAL)){
		return(Poco::Message::PRIO_FATAL);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_CRITICAL)){
		return(Poco::Message::PRIO_CRITICAL);
	}else if(level == std::string(LOGGER_PRIORITY_ERROR)){
		return(Poco::Message::PRIO_ERROR);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_WARNING)){
		return(Poco::Message::PRIO_WARNING);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_NOTICE)){
		return(Poco::Message::PRIO_NOTICE);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_INFORMATION)){ 
		return(Poco::Message::PRIO_INFORMATION);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_DEBUG)){ 
		return(Poco::Message::PRIO_DEBUG);
	}else if(level.c_str() == std::string(LOGGER_PRIORITY_TRACE)){ 
		return(Poco::Message::PRIO_TRACE);
	}else{
		return(0);
	}
}

void Clogger::setLevel(const std::string& name)
{
	_level = name;
	if(_pLogger){
		_pLogger->setLevel(parseLevel(_level));
	}
}

std::string Clogger::getLevel() const
{
	return(_level);
}

void Clogger::setProperty(const std::string& name, const std::string& value)
{
	//TBD
}

std::string Clogger::getProperty(const std::string& name) const
{
    return "unknow";
	//TBD
}

void Clogger::error(const std::string& msg)
{
	if(_pLogger){
		_pLogger->error(msg);
	}
}

Poco::Logger::Ptr Clogger::getLogger() const
{
	return(_pLogger);
}

Clogger::~Clogger()
{
}

}
