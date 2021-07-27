#ifndef __ALARM_TIMER_HPP__
#define __ALARM_TIMER_HPP__



#include "../init_application.hpp"
#include "alarm_queue.hpp"


using Poco::Timer;
using Poco::TimerCallback;
using Poco::Stopwatch;


/**
 *

 *
 *
 * */

class TimerAlarmCheck
{
public:
	TimerAlarmCheck(INT8U slot_index, std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>> &obj_map, Mutex &mtx, NotificationQueue 		&queue):
        _slot_index(slot_index),
        _sdh_obj_map(obj_map),
        _mtx(mtx),
        _alarm_queue(queue)
	{
		_sw.start();
	}	


    void onTimer(Timer& timer);
private:
	Stopwatch _sw;
    INT8U   _slot_index;
    std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>> &_sdh_obj_map;
	Poco::Mutex 			&_mtx;
    NotificationQueue 		&_alarm_queue;
    
};


#endif // DEBUG
