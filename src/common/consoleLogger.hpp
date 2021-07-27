//////////////////////////////////////////////////////////////////
// consoleLogger.hpp
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
#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP
#include "logger.hpp"

using Poco::FileChannel;


namespace App_dev{
//support logger output to console
class CconsoleLogger: public Clogger
{
public:
	using Ptr = Poco::AutoPtr<CconsoleLogger>;
    static std::shared_ptr<CconsoleLogger> _pconsoleLog;
	///////////////////////////////////////////
    //construction functions
    //CconsoleLogger(): output to console
    CconsoleLogger(const std::string &name);
	~CconsoleLogger();

    static std::shared_ptr<CconsoleLogger> getInstance();
private:
protected:
};

class CfileLogger: public Clogger
{
public:
    using Ptr = Poco::AutoPtr<CfileLogger>;
    static std::shared_ptr<CfileLogger> _pfileLog;
    ///////////////////////////////////////////
    //construction functions
    //CconsoleLogger(): output to console
    CfileLogger(const std::string &name);
    ~CfileLogger();

    static std::shared_ptr<CfileLogger> getInstance();
private:
protected:
};


}

#endif
