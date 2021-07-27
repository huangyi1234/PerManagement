#ifndef __ALARM_CHECK_HPP__
#define __ALARM_CHECK_HPP__


#include "../init_application.hpp"
#include "../eqm_module/eqptSHM.hpp"
#include "alarm_timer.hpp"
#include<sys/msg.h>
#include<sys/ipc.h>

extern "C" INT8U	get_eqm_start();


class cAlarmCheck : public Poco::Runnable
{
public:
	cAlarmCheck(std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>> &obj_map, Mutex &mtx, std::map<INT32U, ALARM_INFO> &report_alarm_map, Mutex &alarm_mtx, NotificationQueue &write_queue):
		    _sdh_obj_map(obj_map),
            _mtx(mtx),
            _report_alarm_map(report_alarm_map),
            _alarm_mtx(alarm_mtx),
            _write_queue(write_queue)
    {
    };
	~cAlarmCheck()
	{
		std::cout << "cAlarmCheck  Exception " << std::endl;
	};

    bool  ipc_eqm_device_alarm_init();

	virtual void run();
private:
    std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>> &_sdh_obj_map;
	Poco::Mutex 			&_mtx;
    std::map<INT32U, ALARM_INFO> &_report_alarm_map;
    Poco::Mutex 			&_alarm_mtx;
    NotificationQueue 		&_write_queue;
    NotificationQueue 		_alarm_queue;
    SharedPtr<SharedMemory> _eqm_shared_memory;
    int                     _msq_id;
};



#endif // DEBUG
