//////////////////////////////////////////////////////////////////
// consoleLogger.cpp
//
// Date: 2021/02/28
// Author: yoxia
// Change history:
//      2021/02/28: This is the initialization version.
//
// Discription:
// Implematation class(CconsoleLogger)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#include "consoleLogger.hpp"
#include "Poco/PatternFormatter.h"
#include "Poco/ConsoleChannel.h"
namespace App_dev{
//define class CconsoleLogger
std::shared_ptr<CconsoleLogger> CconsoleLogger::_pconsoleLog;
std::shared_ptr<CfileLogger> CfileLogger::_pfileLog;

CconsoleLogger::CconsoleLogger(const std::string &name):
	Clogger(name)
{
	//Initialize the console output
    _format = "%L%H:%M:%S:%i %p %s(Line%u %t)";
    _pFormatC = new Poco::FormattingChannel(new Poco::PatternFormatter(_format));
    _pFormatC->setChannel(new Poco::ConsoleChannel);
    _pFormatC->open();

    Poco::Logger::create(_nameLogger, _pFormatC,
                        parseLevel(_level));
    _pLogger = Poco::Logger::has(_nameLogger);
	
}

CconsoleLogger::~CconsoleLogger()
{
}

std::shared_ptr<CconsoleLogger> CconsoleLogger::getInstance()
{
    if(!_pconsoleLog){
        _pconsoleLog =  std::make_shared<CconsoleLogger>("FM_ALARM_LOG");
    }
    return(_pconsoleLog);
}

CfileLogger::CfileLogger(const std::string &name):
    Clogger(name)
{
    //Initialize the console output
    _format = "%L%H:%M:%S:%i %p %s(Line%u %t)";
    _pFormatC = new Poco::FormattingChannel(new Poco::PatternFormatter(_format));
    _pFormatC->setChannel(new FileChannel("fm_alarm.log"));
    _pFormatC->open();

    Poco::Logger::create(_nameLogger, _pFormatC,
                        parseLevel(_level));
    _pLogger = Poco::Logger::has(_nameLogger);
    
}

CfileLogger::~CfileLogger()
{
}

std::shared_ptr<CfileLogger> CfileLogger::getInstance()
{
    if(!_pfileLog){
        _pfileLog =  std::make_shared<CfileLogger>("FM_ALARM_LOG_FILE");
    }
    return(_pfileLog);
}

}
