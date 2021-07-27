#ifndef _SELF_APPLICATION_HPP__
#define _SELF_APPLICATION_HPP__

#include "init_application.hpp"
#include "tcpsever.hpp"
#include "eqm_module/eqm_module.hpp"
#include "cm_client/cm_client.hpp"
#include "process_queue/sdh_queue.hpp"
#include "alarm_module/alarm_check.hpp"

#include "Poco/Thread.h"
#include "Poco/Runnable.h"

using Poco::Net::TCPServer;

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::SharedPtr;

#ifdef __cplusplus

extern "C" {

#endif /* __cplusplus */

/*.................................

* do something here

*.................................

*/

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <signal.h>       /* for signal */  
#include <execinfo.h>     /* for backtrace() */  
      
#define BACKTRACE_SIZE   16  
      
void dump(void)  
{  
    int j, nptrs;  
    void *buffer[BACKTRACE_SIZE];  
    char **strings;  
      
    nptrs = backtrace(buffer, BACKTRACE_SIZE);  
      
    printf("backtrace() returned %d addresses\n", nptrs);  
  
    strings = backtrace_symbols(buffer, nptrs);  
    if (strings == NULL) {  
        perror("backtrace_symbols");  
        exit(EXIT_FAILURE);  
    }  
  
    for (j = 0; j < nptrs; j++)  
        printf("  [%02d] %s\n", j, strings[j]);  
  
    free(strings);  
}  
  
void signal_handler(int signo)  
{  
      
#if 0     
    char buff[64] = {0x00};  
          
    sprintf(buff,"cat /proc/%d/maps", getpid());  
          
    system((const char*) buff);  
#endif    
  
    printf("\n=========>>>catch signal %d <<<=========\n", signo);  
      
    printf("Dump stack start...\n");  
    dump();  
    printf("Dump stack end...\n");  
  
    signal(signo, SIG_DFL); /* 恢复信号默认处理 */  
    raise(signo);           /* 重新发送信号 */  
}  



INT8U	eqm_start = 0;

void set_eqm_start(INT8U value) {eqm_start = value;}
INT8U	get_eqm_start(){return eqm_start;}
 
#ifdef __cplusplus

}

#endif /* __cplusplus */

class CSelfApplication : public CAppInitialize
{
public :

	virtual void initialize(Application& self) override
	{
		Application::initialize(self);
		// add your own initialization code here

        /*gl_myLog_file = App_dev::CfileLogger::getInstance();
        gl_myLog_file->setLevel(LOGGER_PRIORITY_INFORMATION);

        gl_myLog = App_dev::CconsoleLogger::getInstance();
        gl_myLog->setLevel(LOGGER_PRIORITY_INFORMATION);

        logger_warning(gl_myLog_file, "this is warning test");
        logger_warning(gl_myLog, "this is warning test");*/


#ifdef __cplusplus
	std::cout << "__cplusplus = " << __cplusplus << std::endl;
#endif

        signal(SIGSEGV, signal_handler);  /* 为SIGSEGV 11信号安装新的处理函数 */
        signal(SIGABRT, signal_handler);  /* 为SIGABRT 6信号安装新的处理函数 */


		_eqm_runnable = new EqmRunnable(_sdh_obj_map, _mtx);
		_eqm_thread = new Thread();
		_eqm_thread->start(_eqm_runnable);

		_cm_runnable = new CMAgentClient(_queue_mutex, _read_queue, _write_queue);
		_cm_thread = new Thread();
		_cm_thread->start(_cm_runnable);

		_queue_runnable = new CSdhQueue(_sdh_obj_map, _mtx, _queue_mutex, _read_queue, _write_queue, _report_alarm_map, _alarm_mtx);
		_queue_thread = new Thread();
		_queue_thread->start(_queue_runnable);

        _alarm_check_runnable = new cAlarmCheck(_sdh_obj_map, _mtx, _report_alarm_map, _alarm_mtx, _write_queue);
		_alarm_check_thread = new Thread();
		_alarm_check_thread->start(_alarm_check_runnable);
	}

	virtual void uninitialize()override
	{
		// add your own uninitialization code here
		Application::uninitialize();
	}

	virtual void reinitialize(Application& self)override
	{
		Application::reinitialize(self);
		// add your own reinitialization code here
	}
	
	int main(const ArgVec& args)
	{
		if (!_helpRequested)
		{
			logger().information("Command line:");
			std::ostringstream ostr;
			for (ArgVec::const_iterator it = argv().begin(); it != argv().end(); ++it)
			{
				ostr << *it << ' ';
			}
			logger().information(ostr.str());
			logger().information("Arguments to main():");
			for (ArgVec::const_iterator it = args.begin(); it != args.end(); ++it)
			{
				logger().information(*it);
			}
			logger().information("Application properties:");
			printProperties("");
		}
		_cm_thread->join();

		return Application::EXIT_OK;
	}

	void printProperties(const std::string& base)
	{
		AbstractConfiguration::Keys keys;
		config().keys(base, keys);
		if (keys.empty())
		{
			if (config().hasProperty(base))
			{
				std::string msg;
				msg.append(base);
				msg.append(" = ");
				msg.append(config().getString(base));
				logger().information(msg);
			}
		}
		else
		{
			for (AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it)
			{
				std::string fullKey = base;
				if (!fullKey.empty()) fullKey += '.';
				fullKey.append(*it);
				printProperties(fullKey);
			}
		}
	}
private:
	Thread	  				*_cm_thread;
	CMAgentClient			*_cm_runnable;

	Thread	  				*_eqm_thread;
	EqmRunnable				*_eqm_runnable;

	Thread	  				*_queue_thread;
	CSdhQueue				*_queue_runnable;

    Thread	  				*_alarm_check_thread;
	cAlarmCheck				*_alarm_check_runnable;

	std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>> _sdh_obj_map;
	Poco::Mutex 			_mtx;

	FastMutex   			_queue_mutex;
	NotificationQueue 		_read_queue;
	NotificationQueue 		_write_queue;

    std::map<INT32U, ALARM_INFO> _report_alarm_map;
    Poco::Mutex 			_alarm_mtx;
};

POCO_APP_MAIN(CSelfApplication);

#endif // !_SELF_APPLICATION_HPP__ 
