#ifndef __INIT_APPLICATION_HPP__
#define __INIT_APPLICATION_HPP__



#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <map>

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "Poco/Net/Socket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/AutoPtr.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/SharedMemory.h"
#include "Poco/Timer.h"
#include "Poco/Stopwatch.h"



#include "config.h"
#include "sdh_transmission/objEqmId.hpp"
#include "sdh_transmission/objSdhId.hpp"
#include "sdh_transmission/objIdDef.hpp"
#include "sdh_transmission/typeDef.hpp"
#include "sdh_transmission/sdhAbstractApi.hpp"
#include "sdh_transmission/sdhApiFactory.hpp"
#include "sdh_transmission/sdhCardO1S64Api.hpp"
#include "sdh_transmission/sdhEntityBase.hpp"
#include "sdh_transmission/sdhRsEntity.hpp"
#include "sdh_transmission/sdhMsEntity.hpp"
#include "sdh_transmission/sdhAu4Entity.hpp"
#include "sdh_transmission/sdhPortEntity.hpp"
#include "sdh_transmission/sdhTu3Entity.hpp"
#include "sdh_transmission/sdhTu12Entity.hpp"

#include "common/consoleLogger.hpp"

using Poco::Notification;
using Poco::NotificationQueue;
using Poco::FastMutex;
using Poco::SharedMemory;


using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Net::NetException;
using Poco::Net::ConnectionRefusedException;
using Poco::Net::InvalidSocketException;
using Poco::Timespan;
using Poco::TimeoutException;
using Poco::IOException;
using Poco::Thread;
using Poco::Mutex;
using Poco::SharedPtr;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;

typedef unsigned long long  INT64U;
typedef unsigned int		INT32U;
typedef unsigned char		INT8U;
typedef unsigned short		INT16U;

typedef long long    INT64S;
typedef int			INT32S;
typedef char			INT8S;
typedef short		INT16S;


static std::shared_ptr<App_dev::CconsoleLogger> gl_myLog;
static std::shared_ptr<App_dev::CfileLogger> gl_myLog_file;


class WorkNotification: public Notification
	// The notification sent to worker threads.
{
public:
	typedef AutoPtr<WorkNotification> Ptr;

	WorkNotification(INT8U *buffer, INT32U len)
	{
		_buffer = new INT8U[len];
		memset(_buffer, 0, len);
		memcpy(_buffer, buffer, len);
		_len = len;
	}

	~WorkNotification(){delete[] _buffer; _buffer = NULL;}

	unsigned short get_len() const
	{return _len;}

	INT8U *get_buffer() const
	{return _buffer;}
private:
	INT8U *_buffer = NULL;
	unsigned short _len = 0;
};

class CAppInitialize: public Application
	/// This sample demonstrates some of the features of the Util::Application class,
	/// such as configuration file handling and command line arguments processing.
	///
	/// Try SampleApp --help (on Unix platforms) or SampleApp /help (elsewhere) for
	/// more information.
{
public:
	CAppInitialize(): _helpRequested(false)
	{
	}
protected:	
	virtual void initialize(Application& self)
    {
         loadConfiguration(); // load default configuration files, if present

		Application::initialize(self);
		// add your own initialization code here
       
    }

	virtual void uninitialize()
    {
        // add your own uninitialization code here
		Application::uninitialize();
    }
	
	virtual void reinitialize(Application& self)
    {
        Application::reinitialize(self);
		// add your own reinitialization code here
    }
	
	virtual void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<CAppInitialize>(this, &CAppInitialize::handleHelp)));

		options.addOption(
			Option("define", "D", "define a configuration property")
				.required(false)
				.repeatable(true)
				.argument("name=value")
				.callback(OptionCallback<CAppInitialize>(this, &CAppInitialize::handleDefine)));
				
		options.addOption(
			Option("config-file", "f", "load configuration data from a file")
				.required(false)
				.repeatable(true)
				.argument("file")
				.callback(OptionCallback<CAppInitialize>(this, &CAppInitialize::handleConfig)));

		options.addOption(
			Option("bind", "b", "bind option value to test.property")
				.required(false)
				.repeatable(false)
				.argument("value")
				.binding("test.property"));
	}

    void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}
	
	void handleDefine(const std::string& name, const std::string& value)
	{
		defineProperty(value);
	}
	
	void handleConfig(const std::string& name, const std::string& value)
	{
		loadConfiguration(value);
	}
		
	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A sample application that demonstrates some of the features of the Poco::Util::Application class.");
		helpFormatter.format(std::cout);
	}
	
	void defineProperty(const std::string& def)
	{
		std::string name;
		std::string value;
		std::string::size_type pos = def.find('=');
		if (pos != std::string::npos)
		{
			name.assign(def, 0, pos);
			value.assign(def, pos + 1, def.length() - pos);
		}
		else name = def;
		config().setString(name, value);
	}
	
	
	
protected:
	bool _helpRequested;
};


#define MSG_ID(x)			((x[0]) | (x[1] << 8) | (x[2] << 16) | (x[3] << 24))
#define MSG_TYPE(x)			((x[4]) | (x[5] << 8))


typedef enum OP_CODE_{
	OP_SET = 0x00,
	OP_GET = 0x01,
	OP_PART_SET = 0x02,
	OP_PART_GET = 0x03,
	OP_ALL_SET = 0x04,
	OP_ALL_GET = 0x05,
	OP_CLEAR = 0x06,
}OP_CODE;

#pragma pack(1)

//byte order
typedef struct MSG_HEAD_{
	INT32U		msg_id;
	INT16U		msg_type;
	INT8U		target;
	INT8U		from;
	INT32U		ruslt;
	INT8U		content_num;
    INT16U      port_num;
}MSG_HEAD, *PMSG_HEAD;

typedef struct MSG_PORT_TYPE_{
	MSG_HEAD	msg_head;
	INT8U		rack_index;
	INT8U 		shelf_index;
	INT8U 		slot_index;
	INT8U 		port_index;
	INT8U		port_type;
	INT8U 		op_code;
}MSG_PORT_TYPE, *PMSG_PORT_TYPE;

typedef struct MSG_PORT_TYPE_NOHEAD_{
	INT8U		rack_index;
	INT8U 		shelf_index;
	INT8U 		slot_index;
	INT8U 		port_index;
	INT8U		port_type;
	INT8U 		op_code;
}MSG_PORT_TYPE_NOHEAD, *PMSG_PORT_TYPE_NOHEAD;

typedef struct MSG_OVERHEAD_{
	MSG_HEAD	msg_head;
	INT8U		rack_index;
	INT8U 		shelf_index;
	INT8U 		slot_index;
	INT8U 		port_index;
	INT8U		vc4_index;
	INT8U		vc3_index;
	INT8U		vc12_index;
	INT8U 		op_code;
	INT8U		oh_type;
	INT8U		direction;
	INT8U		byte_mode;
	INT8U		array[16];
}MSG_OVERHEAD, *PMSG_OVERHEAD;

typedef struct MSG_OVERHEAD_NOHEAD_{
    INT8U		rack_index;
	INT8U 		shelf_index;
	INT8U 		slot_index;
	INT8U 		port_index;
	INT8U		vc4_index;
	INT8U		vc3_index;
	INT8U		vc12_index;
	INT8U 		op_code;
	INT8U		oh_type;
	INT8U		direction;
	INT8U		byte_mode;
	INT8U		array[16];
}MSG_OVERHEAD_NOHEAD,*PMSG_OVERHEAD_NOHEAD;

typedef struct MSG_CROSS_{
	MSG_HEAD	msg_head;
}MSG_CROSS;

typedef struct MSG_ALARM_{      
    MSG_HEAD	        msg_head;       //msg_type:0x600
    INT8U		        rack_index;
	INT8U 		        shelf_index;
	INT8U 		        slot_index;
	INT8U 		        op_code;
    INT8U		        alarm_obj[32];      //产生告警位置
    INT8U		        time[32];           //产生告警时间    
    INT8U               nc_value;           //notification code
    INT8U                na_flag;            //是否影响业务
    INT32U              alarm_code;         //告警ID
}MSG_ALARM,*PMSG_ALARM;

typedef struct MSG_ALARM_NOHEAD_{
    INT8U                rack_index;
    INT8U               shelf_index;
    INT8U               slot_index;
    INT8U               op_code;
    INT8U               alarm_obj[32];      //产生告警位置
    INT8U               time[32];           //产生告警时间    
    INT8U               nc_value;       //notification code
    INT8U                na_flag;        //是否影响业务
    INT32U              alarm_code;     //告警ID
}MSG_ALARM_NOHEAD,*PMSG_ALARM_NOHEAD;

#pragma pack()

typedef enum OVERHEAD_BYTE_TYPE_{
    OVERHEAD_BYTE_J0 = 1,       /*RS overhead*/
    OVERHEAD_BYTE_S1,           /*MS overhead*/
    OVERHEAD_BYTE_J1,           /*vc3/vc4 channel overhead*/
    OVERHEAD_BYTE_C2,           /*vc3/vc4 channel overhead*/
    OVERHEAD_BYTE_J2,           /*vc12 channel overhead*/
}OVERHEAD_BYTE_TYPE;

enum OVERHEAD_BYTE_DIRECTION_{
    TX_DIR_OVH = 1,
    EXP_RX_DIR_OVH,
    REL_RX_DIR_OVH,
};
    

typedef enum CM_MSG_TYPE_{
	MSG_TYPE_PORT_TYPE = 0x200,
	MSG_TYPE_OVERHEAD,
	MSG_TYPE_CROSS,
	MSG_TYPE_ALARM_REPORT = 0x600,
}CM_MSG_TYPE;

#endif // DEBUG
