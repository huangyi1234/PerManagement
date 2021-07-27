#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <string>

static std::string version_str="ver00.00.0a";

#define __DEBUG__	1					//for debug
//#define __SHARED_DEBUG__	1			//for debug
#define DATABASE_FILE_PATH              "./FM_ALARM.db"

#define CM_SERVER_IP                    "127.0.0.1"
#define CM_SERVER_LISTEN_PORT           2001

#endif // !__CONFIG_H__
