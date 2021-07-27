#ifndef __SDH_QUEUE_HPP__
#define __SDH_QUEUE_HPP__



#include "../init_application.hpp"


/**
 *

 *
 *
 * */

class CSdhQueue : public Poco::Runnable
{
public:
	CSdhQueue(std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>> &obj_map, Mutex &mtx, FastMutex &queue_mutex, NotificationQueue &read_queue, NotificationQueue &write_queue,\
        std::map<INT32U, ALARM_INFO> &report_alarm_map, Mutex &alarm_mtx):
		_sdh_obj_map(obj_map),
            _mtx(mtx),
            _queue_mutex(queue_mutex),
            _read_queue(read_queue),
            _write_queue(write_queue),
            _report_alarm_map(report_alarm_map),
            _alarm_mtx(alarm_mtx){};
	~CSdhQueue()
	{
	};

	virtual void run();

	bool config_port_type(INT8U *buffer, INT32U len);
	bool config_overhead(INT8U *buffer, INT32U len);
    bool config_overhead_RS_J0(	SharedPtr<App_dev::CobjSdhId> &tmp_obj, 	SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead);
    bool config_overhead_MS_S1( SharedPtr<App_dev::CobjSdhId> &tmp_obj,     SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead);
    bool config_overhead_J1( SharedPtr<App_dev::CobjSdhId> &tmp_obj,     SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead);
    bool config_overhead_C2( SharedPtr<App_dev::CobjSdhId> &tmp_obj,     SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead);
    bool config_overhead_J2(	SharedPtr<App_dev::CobjSdhId> &tmp_obj, 	SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead);

    bool config_overhead_RS_J0_get_all(	SharedPtr<App_dev::CobjSdhId> &tmp_obj, 	SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
    {
        msg_overhead_nohead.port_index = tmp_obj->getPort();
        msg_overhead_nohead.byte_mode = tmp_entity->getJ0Mode();
        if(msg_overhead_nohead.direction == TX_DIR_OVH)
        {
            tmp_entity->getTxJ0Trace(msg_overhead_nohead.array);
        }
        else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
        {
            tmp_entity->getExpJ0Trace(msg_overhead_nohead.array);
        }
        else if(msg_overhead_nohead.direction == REL_RX_DIR_OVH)
        {
            tmp_entity->getRxJ0Trace(msg_overhead_nohead.array);
        }

        return true;
    }
    bool config_overhead_MS_S1_get_all( SharedPtr<App_dev::CobjSdhId> &tmp_obj,     SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
    {
        msg_overhead_nohead.port_index = tmp_obj->getPort();
        msg_overhead_nohead.array[0] = tmp_entity->get_S1_value();

        return true;

    }
    bool config_overhead_J1_get_all( SharedPtr<App_dev::CobjSdhId> &tmp_obj,     SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
    {
        INT8U vc4_flag = 0;
        
        /*判断当前配置的对象是vc3还是vc4*/
        if(msg_overhead_nohead.vc4_index == 0xff)  
        {
            vc4_flag = 1;
        }
        else
        {
            vc4_flag = 0;
        }
        
        if(vc4_flag)
        {
            if(tmp_obj->getVc4Num() == 0xff)
            {
                return false;
            }
            msg_overhead_nohead.vc4_index = tmp_obj->getVc4Num();
            msg_overhead_nohead.byte_mode = tmp_entity->getJ1Mode();
            if(msg_overhead_nohead.direction == TX_DIR_OVH)
            {
                tmp_entity->getTxJ1Trace(msg_overhead_nohead.array);
            }
            else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
            {
                tmp_entity->getExpJ1Trace(msg_overhead_nohead.array);
            }
            else if(msg_overhead_nohead.direction == REL_RX_DIR_OVH)
            {
                tmp_entity->getRxJ1Trace(msg_overhead_nohead.array);
            }

            return true;
        }
        else
        {
            if(tmp_obj->getVc4Num() != msg_overhead_nohead.vc4_index)
            {
                return false;
            }

            if(tmp_obj->getTug3Num() == 0xff)
            {
                return false;
            }
            msg_overhead_nohead.vc3_index = tmp_obj->getTug3Num();
            msg_overhead_nohead.byte_mode = tmp_entity->getJ1Mode();
            if(msg_overhead_nohead.direction == TX_DIR_OVH)
            {
                tmp_entity->getTxJ1Trace(msg_overhead_nohead.array);
            }
            else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
            {
                tmp_entity->getExpJ1Trace(msg_overhead_nohead.array);
            }
            else if(msg_overhead_nohead.direction == REL_RX_DIR_OVH)
            {
                tmp_entity->getRxJ1Trace(msg_overhead_nohead.array);
            }    

            return true;
        }
        
        return false;
    }
    bool config_overhead_C2_get_all( SharedPtr<App_dev::CobjSdhId> &tmp_obj,     SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
    {
        INT8U  value = 0;
        INT8U vc4_flag = 0;

        /*判断当前配置的对象是vc3还是vc4*/
        if(msg_overhead_nohead.vc4_index == 0xff)  
        {
            vc4_flag = 1;
        }
        else
        {
            vc4_flag = 0;
        }
        
        if(vc4_flag)
        {
            if(tmp_obj->getVc4Num() == 0xff)
            {
                return false;
            }

            msg_overhead_nohead.vc4_index = tmp_obj->getVc4Num();
            if(msg_overhead_nohead.direction == TX_DIR_OVH)
            {
                tmp_entity->getTxC2Trace(value);
            }
            else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
            {
                tmp_entity->getExpC2Trace(value);
            }
            else if(msg_overhead_nohead.direction == REL_RX_DIR_OVH)
            {
                tmp_entity->getRxC2Trace(value);
            }
            msg_overhead_nohead.array[0] = value;

            return true;
        }
        else
        {
            if(tmp_obj->getVc4Num() != msg_overhead_nohead.vc4_index)
            {
                return false;
            }

            if(tmp_obj->getTug3Num() == 0xff)
            {
                return false;
            }
            
            msg_overhead_nohead.vc3_index = tmp_obj->getTug3Num();
            if(msg_overhead_nohead.direction == TX_DIR_OVH)
            {
                tmp_entity->getTxC2Trace(value);
            }
            else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
            {
                tmp_entity->getExpC2Trace(value);
            }
            else if(msg_overhead_nohead.direction == REL_RX_DIR_OVH)
            {
                tmp_entity->getRxC2Trace(value);
            }
            msg_overhead_nohead.array[0] = value;

            return true;
        }
        
        return false;
    }
    bool config_overhead_J2_get_all(	SharedPtr<App_dev::CobjSdhId> &tmp_obj, 	SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
    {
        msg_overhead_nohead.vc12_index = tmp_obj->getVc12Num();
        msg_overhead_nohead.byte_mode = tmp_entity->getJ0Mode();
        if(msg_overhead_nohead.direction == TX_DIR_OVH)
        {
            tmp_entity->getTxJ2Trace(msg_overhead_nohead.array);
        }
        else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
        {
            tmp_entity->getExpJ2Trace(msg_overhead_nohead.array);
        }
        else if(msg_overhead_nohead.direction == REL_RX_DIR_OVH)
        {
            tmp_entity->getRxJ2Trace(msg_overhead_nohead.array);
        }

        return true;
    }

    bool alarm_report_func(INT8U *buffer, INT32U len);



private:
	std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>> &_sdh_obj_map;
	Poco::Mutex 			&_mtx;
	FastMutex   			&_queue_mutex;
	NotificationQueue 		&_read_queue;
	NotificationQueue 		&_write_queue;
    std::map<INT32U, ALARM_INFO> &_report_alarm_map;
    Poco::Mutex 			&_alarm_mtx;
	//send list
	//recv list
};

#endif // DEBUG
