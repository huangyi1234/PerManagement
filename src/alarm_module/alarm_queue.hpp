#ifndef __ALARM_QUEUE_HPP__
#define __ALARM_QUEUE_HPP__



#include "../init_application.hpp"
#include "alarm_control.hpp"

/**
 *

 *
 *
 * */

class cWorkerAlarm: public Poco::Runnable
	// A worker thread that gets work items
	// from a NotificationQueue.
{
public:
	cWorkerAlarm(const std::string& name, NotificationQueue& queue, std::map<INT32U, ALARM_INFO> &report_alarm_map, Mutex &alarm_mtx, NotificationQueue &write_queue):
		_name(name),_queue(queue),
        _report_alarm_map(report_alarm_map),
        _alarm_mtx(alarm_mtx),
        _write_queue(write_queue){};

	virtual void run();
    void write_queue_alarm_info(ALARM_INFO &alarm_info);

private:

	std::string        _name;
	NotificationQueue& _queue;
    std::map<INT32U, ALARM_INFO> &_report_alarm_map;
    Poco::Mutex 			&_alarm_mtx;
    NotificationQueue       &_write_queue;
};

class cAlarmNode: public Notification
	// The notification sent to worker threads.
{
public:
	typedef AutoPtr<cAlarmNode> Ptr;
    static INT32U count;

	cAlarmNode(std::map<unsigned int, ALARM_INFO> alarm_map):_alarm_map(alarm_map)
	{
	    count++;
	}

	~cAlarmNode(){count--;}

	std::map<unsigned int, ALARM_INFO>   &get_alarm_map(){return _alarm_map;}
    static INT32U  get_cAlarmNode_size(){return count;}

private:
	std::map<unsigned int, ALARM_INFO>   _alarm_map;
};



#endif // DEBUG
