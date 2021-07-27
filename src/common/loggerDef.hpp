//////////////////////////////////////////////////////////////////
// loggerDef.hpp
//
// Date: 2021/02/26
// Author: yoxia
// Change history:
//      2021/02/26: This is the initialization version.
//
// Discription:
// Define type for logger
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef LOGGER_DEF_HPP
#define LOGGER_DEF_HPP
//////////////////////////////////////////////////////////////////
//priority definition
//A NONE. The application will turn off the logger output.
#define LOGGER_PRIORITY_NONE  "NONE"
//A fatal error. The application will most likely terminate.
//This is the highest priority.
#define LOGGER_PRIORITY_FATAL  "FATAL"
//A critical error. The application might not be able to 
//continue running successfully.
#define LOGGER_PRIORITY_CRITICAL "CRITICAL"
//An error. An operation did not complete successfully, 
//but the application as a whole is not affected.
#define LOGGER_PRIORITY_ERROR  "ERROR"
//A warning. An operation completed with an unexpected result.
#define LOGGER_PRIORITY_WARNING  "WARNING"
//A notice, which is an information with just a higher priority.
#define LOGGER_PRIORITY_NOTICE  "NOTICE"
//An informational message, usually denoting the successful 
//completion of an operation.
#define LOGGER_PRIORITY_INFORMATION  "INFORMATION"
//A debugging message.
#define LOGGER_PRIORITY_DEBUG  "DEBUG"
//A tracing message. This is the lowest priority.
#define LOGGER_PRIORITY_TRACE  "TRACE"


#endif
