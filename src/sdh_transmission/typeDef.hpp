//////////////////////////////////////////////////////////////////
// typeDef.hpp
//
// Date: 2021/02/20
// Author: yoxia
// Change history:
//      2021/02/20: This is the initialization version.
//
// Discription:
// Define basic type
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef TYPE_DEF_HPP
#define TYPE_DEF_HPP
#include<iostream>

#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"

using Poco::LocalDateTime;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;


namespace App_dev{
//Define the return type of function
typedef enum RetVal
{
	RV_SUCCESS = 0,
	RV_FAILURE = 1,
	RV_ERROR = -1,
	RV_UNSUPPORTED = -2
}RetVal;

typedef unsigned char UCHAR;
typedef unsigned short int USHORT;
typedef unsigned int UINT;

class cMyTime{
public:
    cMyTime(std::string &time):_time(time){
    }
    ~cMyTime(){};
    std::string &current_time()
    {
        LocalDateTime now;
        //_time = DateTimeFormatter::format(now, DateTimeFormat::SORTABLE_FORMAT);
        char time_str[128] = {0};

        sprintf(time_str, "%d-%02d-%02d %02d:%02d:%02d.%d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(), now.millisecond());
        _time = time_str;
        
        return _time;
    }

private:
    std::string &_time;
};

}

#endif
