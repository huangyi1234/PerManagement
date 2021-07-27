#ifndef __SDH_QUEUE_CPP__
#define __SDH_QUEUE_CPP__

#include "sdh_queue.hpp"

void CSdhQueue::run()
{
	INT16U	msg_type = 0;

	while(1)
	{
		Notification::Ptr pNf(_read_queue.waitDequeueNotification());
		if (pNf)
		{
			WorkNotification::Ptr pWorkNf = pNf.cast<WorkNotification>();
			if (pWorkNf)
			{
				msg_type = MSG_TYPE(pWorkNf->get_buffer());

				std::cout << "msg_type: " << msg_type << std::endl;

				if(msg_type == MSG_TYPE_PORT_TYPE)
				{
					config_port_type(pWorkNf->get_buffer(), pWorkNf->get_len());
				}
				else if(msg_type == MSG_TYPE_OVERHEAD)
				{
                    config_overhead(pWorkNf->get_buffer(), pWorkNf->get_len());
				}
                else if(msg_type == MSG_TYPE_ALARM_REPORT)
                {
                    alarm_report_func(pWorkNf->get_buffer(), pWorkNf->get_len());
                }
			}
		}
	}
}

bool CSdhQueue::config_port_type(INT8U *buffer, INT32U len)
{
	SharedPtr<App_dev::CobjSdhId> tmp_obj;
	SharedPtr<App_dev::CsdhEntityBase> tmp_entity;
    MSG_PORT_TYPE   port_msg;
	MSG_PORT_TYPE_NOHEAD port_msg_nohead;
	INT8U 				tmp;
	INT8U				*send_buffer = NULL;
    INT8U               port_num = 0;
    std::map<int, MSG_PORT_TYPE_NOHEAD> map_data;
    INT32U  send_len = 0;

	if(!buffer || len <= 0)
		return false;

	memset(&port_msg, 0, sizeof(port_msg));
	memcpy(&port_msg, buffer, sizeof(port_msg));
    port_num = 0;

	_mtx.lock();
	for(auto &it : _sdh_obj_map)
	{
        tmp_obj = (it.first);
        tmp_entity = (it.second);
        
        if(port_msg.op_code == OP_ALL_GET)
        {
            if(len != (sizeof(MSG_PORT_TYPE) + 2))
            {
                printf("[ERROR get all]%s()    recv len error!!!\r\n", __func__);
                return false;
            }
            if(tmp_obj && tmp_obj->getSdhIdType() == App_dev::OBJ_SDHID_PORT)
            {
                if(tmp_obj->getRack() == port_msg.rack_index && tmp_obj->getShelf() ==  port_msg.shelf_index && \
                                        tmp_obj->getSlot() ==  port_msg.slot_index)
                {
                    tmp_entity->getInstance();
                    memset(&port_msg_nohead, 0, sizeof(port_msg_nohead));
                    memcpy(&port_msg_nohead, (buffer + sizeof(MSG_HEAD)), sizeof(port_msg_nohead));
                    port_msg_nohead.port_type = tmp_entity->getPortType();
                    port_msg_nohead.port_index = tmp_obj->getPort();
                    map_data[port_num] = port_msg_nohead;
                    port_num++;                    
                }
                else 
                {
                    continue;
                }
            }                
        }
        else
        {
            if(len != (sizeof(MSG_HEAD) + (sizeof(MSG_PORT_TYPE_NOHEAD) * port_msg.msg_head.content_num)  + 2))
            {
                printf("[ERROR]%s()    recv len error!!!\r\n", __func__);
                return false;
            }
            for(int i = 0; i < port_msg.msg_head.content_num; i++)
            {
                memset(&port_msg_nohead, 0, sizeof(port_msg_nohead));
                memcpy(&port_msg_nohead, (buffer + sizeof(MSG_HEAD) + (sizeof(MSG_PORT_TYPE_NOHEAD) * i)), sizeof(port_msg_nohead));

                if(tmp_obj && tmp_obj->getSdhIdType() == App_dev::OBJ_SDHID_PORT)
                if(tmp_obj->getRack() == port_msg_nohead.rack_index && tmp_obj->getShelf() ==  port_msg_nohead.shelf_index && \
                        tmp_obj->getSlot() ==  port_msg_nohead.slot_index && tmp_obj->getPort() ==  port_msg_nohead.port_index)
                {
                    tmp_entity->getInstance();

                    if(port_msg_nohead.op_code == OP_SET)
                    {            
                        tmp_entity->setPortType(App_dev::Port_Transmission_Type(port_msg_nohead.port_type));
                    }    
                    else if(port_msg_nohead.op_code == OP_GET)
                    {            
                        port_msg_nohead.port_type = tmp_entity->getPortType();
                    }
                    map_data[port_num] = port_msg_nohead;
                    port_num++;
                }
            }
        }
	}
	_mtx.unlock();

    send_len = (sizeof(MSG_HEAD) + (sizeof(MSG_PORT_TYPE_NOHEAD) * port_num));
    send_buffer = new INT8U[send_len];
    memset(send_buffer, 0, send_len);

    printf("[DEBUG]%s()   map_data.num:%d \r\n", __func__, map_data.size());
    for(auto &it_data : map_data){
        memcpy(send_buffer + (sizeof(MSG_HEAD) + (sizeof(MSG_PORT_TYPE_NOHEAD) * it_data.first)), &(it_data.second), sizeof(MSG_PORT_TYPE_NOHEAD));
    }
        
    port_msg.msg_head.content_num = port_num;
    port_msg.msg_head.ruslt = 1;
    tmp = port_msg.msg_head.target;
    port_msg.msg_head.target = port_msg.msg_head.from;
    port_msg.msg_head.from = tmp;
    memcpy(send_buffer, &port_msg, sizeof(MSG_HEAD));
    _write_queue.enqueueNotification(new WorkNotification(send_buffer, send_len));
    delete[] send_buffer;
    send_buffer = NULL;

	return true;
}

bool CSdhQueue::config_overhead(INT8U *buffer, INT32U len)
{
	SharedPtr<App_dev::CobjSdhId> tmp_obj;
	SharedPtr<App_dev::CsdhEntityBase> tmp_entity;
    MSG_OVERHEAD msg_overhead;
    MSG_OVERHEAD_NOHEAD msg_overhead_nohead;
    INT8U 				tmp;
	INT8U				*send_buffer = NULL;
    INT8U               overhead_num = 0;
    std::map<int, MSG_OVERHEAD_NOHEAD> map_data;
    INT32U  send_len = 0;

    if(!buffer || !len)
        return false;

    memset(&msg_overhead, 0, sizeof(msg_overhead));
    memcpy(&msg_overhead, buffer, sizeof(msg_overhead));
    overhead_num = 0;

	_mtx.lock();
	for(auto &it : _sdh_obj_map)
	{
        tmp_obj = (it.first);
        tmp_entity = (it.second);

        if(msg_overhead.op_code == OP_ALL_GET)
        {
            if(len != (sizeof(MSG_OVERHEAD) + 2))
            {
                printf("[ERROR get all]%s()    recv len error!!!\r\n", __func__);
                return false;
            }
            if(tmp_obj && tmp_obj->getSdhIdType() == App_dev::OBJ_SDHID_RS && msg_overhead.oh_type == OVERHEAD_BYTE_J0)
            {
                if(tmp_obj->getRack() == msg_overhead.rack_index && tmp_obj->getShelf() ==  msg_overhead.shelf_index && \
                  tmp_obj->getSlot() ==  msg_overhead.slot_index)
                {
                    tmp_entity->getInstance();
                    memset(&msg_overhead_nohead, 0, sizeof(msg_overhead_nohead));
                    memcpy(&msg_overhead_nohead, (buffer + sizeof(MSG_HEAD)), sizeof(msg_overhead_nohead));
                    config_overhead_RS_J0_get_all(tmp_obj, tmp_entity, msg_overhead_nohead);
                    map_data[overhead_num] = msg_overhead_nohead;
                    overhead_num++;                    
                }
            }
            else if(tmp_obj && tmp_obj->getSdhIdType() == App_dev::OBJ_SDHID_MS && msg_overhead.oh_type == OVERHEAD_BYTE_S1)
            {
                if(tmp_obj->getRack() == msg_overhead.rack_index && tmp_obj->getShelf() ==  msg_overhead.shelf_index && \
                  tmp_obj->getSlot() ==  msg_overhead.slot_index)
                {
                    tmp_entity->getInstance();
                    memset(&msg_overhead_nohead, 0, sizeof(msg_overhead_nohead));
                    memcpy(&msg_overhead_nohead, (buffer + sizeof(MSG_HEAD)), sizeof(msg_overhead_nohead));
                    config_overhead_MS_S1_get_all(tmp_obj, tmp_entity, msg_overhead_nohead);
                    map_data[overhead_num] = msg_overhead_nohead;
                    overhead_num++;                    
                }
            }
            else if(msg_overhead.oh_type == OVERHEAD_BYTE_J1)
            {
                if(tmp_obj->getRack() == msg_overhead.rack_index && tmp_obj->getShelf() ==  msg_overhead.shelf_index && \
                  tmp_obj->getSlot() ==  msg_overhead.slot_index &&tmp_obj->getPort() == msg_overhead.port_index)
                {
                    tmp_entity->getInstance();
                    memset(&msg_overhead_nohead, 0, sizeof(msg_overhead_nohead));
                    memcpy(&msg_overhead_nohead, (buffer + sizeof(MSG_HEAD)), sizeof(msg_overhead_nohead));
                    if(config_overhead_J1_get_all(tmp_obj, tmp_entity, msg_overhead_nohead))
                    {
                        map_data[overhead_num] = msg_overhead_nohead;
                        overhead_num++;   
                    }
                }
            }
            else if(msg_overhead.oh_type == OVERHEAD_BYTE_C2)
            {
                if(tmp_obj->getRack() == msg_overhead.rack_index && tmp_obj->getShelf() ==  msg_overhead.shelf_index && \
                  tmp_obj->getSlot() ==  msg_overhead.slot_index &&tmp_obj->getPort() == msg_overhead.port_index)
                {
                    tmp_entity->getInstance();
                    memset(&msg_overhead_nohead, 0, sizeof(msg_overhead_nohead));
                    memcpy(&msg_overhead_nohead, (buffer + sizeof(MSG_HEAD)), sizeof(msg_overhead_nohead));
                    if(config_overhead_C2_get_all(tmp_obj, tmp_entity, msg_overhead_nohead))
                    {
                        map_data[overhead_num] = msg_overhead_nohead;
                        overhead_num++;   
                    }                 
                }
            }
            else if(msg_overhead.oh_type == OVERHEAD_BYTE_J2)
            {
                if(tmp_obj->getRack() == msg_overhead.rack_index && tmp_obj->getShelf() ==  msg_overhead.shelf_index && \
                  tmp_obj->getSlot() ==  msg_overhead.slot_index&&tmp_obj->getPort() == msg_overhead.port_index &&\
                  tmp_obj->getVc4Num() == msg_overhead.vc4_index && tmp_obj->getTug3Num() == msg_overhead.vc3_index)
                {
                    tmp_entity->getInstance();
                    memset(&msg_overhead_nohead, 0, sizeof(msg_overhead_nohead));
                    memcpy(&msg_overhead_nohead, (buffer + sizeof(MSG_HEAD)), sizeof(msg_overhead_nohead));
                    config_overhead_J2_get_all(tmp_obj, tmp_entity, msg_overhead_nohead);
                    map_data[overhead_num] = msg_overhead_nohead;
                    overhead_num++;                    
                }
            }
                
        }
        else
        {
            if(len != (sizeof(MSG_HEAD) + (sizeof(MSG_OVERHEAD_NOHEAD) * msg_overhead.msg_head.content_num)  + 2))
            {
                printf("[ERROR]%s()    recv len error!!!\r\n", __func__);
                return false;
            }
    		for(int i = 0; i < msg_overhead.msg_head.content_num; i++)
    		{
    			memset(&msg_overhead_nohead, 0, sizeof(msg_overhead_nohead));
    			memcpy(&msg_overhead_nohead, (buffer + sizeof(MSG_HEAD) + (sizeof(MSG_OVERHEAD_NOHEAD) * i)), sizeof(msg_overhead_nohead));
           
        		if(tmp_obj->getSdhIdType() == App_dev::OBJ_SDHID_RS && msg_overhead_nohead.oh_type == OVERHEAD_BYTE_J0)
                {
                    if(config_overhead_RS_J0(tmp_obj, tmp_entity, msg_overhead_nohead))
                    {
                        map_data[overhead_num] = msg_overhead_nohead;
                        overhead_num++; 
                    }
                }
                else if(tmp_obj->getSdhIdType() == App_dev::OBJ_SDHID_MS && msg_overhead_nohead.oh_type == OVERHEAD_BYTE_S1)
                {
                    if(config_overhead_MS_S1(tmp_obj, tmp_entity, msg_overhead_nohead))
                    {
                        map_data[overhead_num] = msg_overhead_nohead;
                        overhead_num++; 
                    }
                }else if(msg_overhead_nohead.oh_type == OVERHEAD_BYTE_J1)
                {
                    if(config_overhead_J1(tmp_obj, tmp_entity, msg_overhead_nohead))
                    {
                        map_data[overhead_num] = msg_overhead_nohead;
                        overhead_num++; 
                    }
                }
                else if(msg_overhead_nohead.oh_type == OVERHEAD_BYTE_C2)
                {
                    if(config_overhead_C2(tmp_obj, tmp_entity, msg_overhead_nohead))
                    {
                        map_data[overhead_num] = msg_overhead_nohead;
                        overhead_num++; 
                    }
                }
                else if(msg_overhead_nohead.oh_type == OVERHEAD_BYTE_J2)
                {
                    if(config_overhead_J2(tmp_obj, tmp_entity, msg_overhead_nohead))
                    {
                        map_data[overhead_num] = msg_overhead_nohead;
                        overhead_num++; 
                    }
                } 
           }
        }
   }

    send_len = (sizeof(MSG_HEAD) + (sizeof(MSG_OVERHEAD_NOHEAD) * overhead_num));
    send_buffer = new INT8U[send_len];
    memset(send_buffer, 0, send_len);

    printf("[DEBUG]%s()   map_data.num:%d \r\n", __func__, map_data.size());
    for(auto &it_data : map_data){
        printf("%s\r\n", it_data.second.array);
        memcpy(send_buffer + (sizeof(MSG_HEAD) + (sizeof(MSG_OVERHEAD_NOHEAD) * it_data.first)), &(it_data.second), sizeof(MSG_OVERHEAD_NOHEAD));
    }

    msg_overhead.msg_head.content_num = overhead_num;
    msg_overhead.msg_head.ruslt = 1;
	tmp = msg_overhead.msg_head.target;
	msg_overhead.msg_head.target = msg_overhead.msg_head.from;
	msg_overhead.msg_head.from = tmp;
	memcpy(send_buffer, &msg_overhead.msg_head, sizeof(msg_overhead.msg_head));
	_write_queue.enqueueNotification(new WorkNotification(send_buffer, send_len));
	delete[] send_buffer;
	_mtx.unlock();

	return true;
}

bool CSdhQueue::config_overhead_RS_J0(	SharedPtr<App_dev::CobjSdhId> &tmp_obj, 	SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
{
	if(tmp_obj->getRack() == msg_overhead_nohead.rack_index && tmp_obj->getShelf() ==  msg_overhead_nohead.shelf_index && \
			tmp_obj->getSlot() ==  msg_overhead_nohead.slot_index && tmp_obj->getPort() ==  msg_overhead_nohead.port_index)
	{
		tmp_entity->getInstance();

		if(msg_overhead_nohead.op_code == OP_SET)	//set
        {
            tmp_entity->setJ0Mode((App_dev::J0Mode)msg_overhead_nohead.byte_mode);
            if(msg_overhead_nohead.direction == TX_DIR_OVH)
            {
                tmp_entity->setTxJ0Trace(msg_overhead_nohead.array);
            }
            else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
            {
                tmp_entity->setExpJ0Trace(msg_overhead_nohead.array);
            }                    
        }         
		else if(msg_overhead_nohead.op_code == OP_GET)
        {
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
        }  

        return true;
	}        

	return false;
}

bool CSdhQueue::config_overhead_MS_S1(	SharedPtr<App_dev::CobjSdhId> &tmp_obj, 	SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
{
    INT8U   value = 0;

	if(tmp_obj->getRack() == msg_overhead_nohead.rack_index && tmp_obj->getShelf() ==  msg_overhead_nohead.shelf_index && \
			tmp_obj->getSlot() ==  msg_overhead_nohead.slot_index && tmp_obj->getPort() ==  msg_overhead_nohead.port_index)
	{
		tmp_entity->getInstance();

        value = msg_overhead_nohead.array[0];
		if(msg_overhead_nohead.op_code == OP_SET)	//set
        {
            tmp_entity->set_S1_value(value);                
        }         
		else if(msg_overhead_nohead.op_code == OP_GET)
        {
            msg_overhead_nohead.array[0] = tmp_entity->get_S1_value();
        }  

        return true;
	}        

	return false;
}

bool CSdhQueue::config_overhead_J1(	SharedPtr<App_dev::CobjSdhId> &tmp_obj, 	SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
{
    INT8U vc4_flag = 0;

    /*判断当前配置的对象是vc3还是vc4*/
    if(msg_overhead_nohead.vc3_index == 0xff)  
    {
        vc4_flag = 1;
    }
    else
    {
        vc4_flag = 0;
    }

    /*J1开销VC4和VC3都需要操作*/
	if(tmp_obj->getRack() == msg_overhead_nohead.rack_index && tmp_obj->getShelf() ==  msg_overhead_nohead.shelf_index && \
			tmp_obj->getSlot() ==  msg_overhead_nohead.slot_index && tmp_obj->getPort() ==  msg_overhead_nohead.port_index  &&\
			tmp_obj->getVc4Num() == msg_overhead_nohead.vc4_index)
	{
		tmp_entity->getInstance();

        if(vc4_flag)
        {
            if(msg_overhead_nohead.op_code == OP_SET)   //set
            {
                tmp_entity->setJ1Mode((App_dev::J0Mode)msg_overhead_nohead.byte_mode);
                if(msg_overhead_nohead.direction == TX_DIR_OVH)
                {
                    tmp_entity->setTxJ1Trace(msg_overhead_nohead.array);
                }
                else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
                {
                    tmp_entity->setExpJ1Trace(msg_overhead_nohead.array);
                }                    
            }         
            else if(msg_overhead_nohead.op_code == OP_GET)
            {
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
            }  

            return true;
        }
        else
        {
            if(tmp_obj->getTug3Num() == msg_overhead_nohead.vc3_index)
            {
                if(msg_overhead_nohead.op_code == OP_SET)   //set
                {
                    tmp_entity->setJ1Mode((App_dev::J0Mode)msg_overhead_nohead.byte_mode);
                    if(msg_overhead_nohead.direction == TX_DIR_OVH)
                    {
                        tmp_entity->setTxJ1Trace(msg_overhead_nohead.array);
                    }
                    else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
                    {
                        tmp_entity->setExpJ1Trace(msg_overhead_nohead.array);
                    }                    
                }         
                else if(msg_overhead_nohead.op_code == OP_GET)
                {
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
                }  
                
                return true;
            }
        }
	}        

	return false;
}

bool CSdhQueue::config_overhead_C2(	SharedPtr<App_dev::CobjSdhId> &tmp_obj, 	SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
{
    INT8U  value = 0;
    INT8U vc4_flag = 0;

    /*判断当前配置的对象是vc3还是vc4*/
    if(msg_overhead_nohead.vc3_index == 0xff)  
    {
        vc4_flag = 1;
    }
    else
    {
        vc4_flag = 0;
    }
	if(tmp_obj->getRack() == msg_overhead_nohead.rack_index && tmp_obj->getShelf() ==  msg_overhead_nohead.shelf_index && \
			tmp_obj->getSlot() ==  msg_overhead_nohead.slot_index && tmp_obj->getPort() ==  msg_overhead_nohead.port_index  &&\
			tmp_obj->getVc4Num() == msg_overhead_nohead.vc4_index)
	{
		tmp_entity->getInstance();

        if(vc4_flag)
        {
            if(msg_overhead_nohead.op_code == OP_SET)   //set
            {
                value = msg_overhead_nohead.array[0];
                if(msg_overhead_nohead.direction == TX_DIR_OVH)
                {
                    tmp_entity->setTxC2Trace(value);
                }
                else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
                {
                    tmp_entity->setExpC2Trace(value);
                }                    
            }         
            else if(msg_overhead_nohead.op_code == OP_GET)
            {
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
            }  

            return true;
        }
        else
        {
            if(tmp_obj->getTug3Num() == msg_overhead_nohead.vc3_index)
            {
                if(msg_overhead_nohead.op_code == OP_SET)   //set
                {
                    value = msg_overhead_nohead.array[0];
                    if(msg_overhead_nohead.direction == TX_DIR_OVH)
                    {
                        tmp_entity->setTxC2Trace(value);
                    }
                    else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
                    {
                        tmp_entity->setExpC2Trace(value);
                    }                    
                }         
                else if(msg_overhead_nohead.op_code == OP_GET)
                {
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
                }  
                
                return true;
            }
        }
	}        

	return false;
}

bool CSdhQueue::config_overhead_J2(	SharedPtr<App_dev::CobjSdhId> &tmp_obj, 	SharedPtr<App_dev::CsdhEntityBase> &tmp_entity,     MSG_OVERHEAD_NOHEAD &msg_overhead_nohead)
{
	if(tmp_obj->getRack() == msg_overhead_nohead.rack_index && tmp_obj->getShelf() ==  msg_overhead_nohead.shelf_index && \
			tmp_obj->getSlot() ==  msg_overhead_nohead.slot_index && tmp_obj->getPort() ==  msg_overhead_nohead.port_index &&\
			tmp_obj->getVc4Num() == msg_overhead_nohead.vc4_index &&tmp_obj->getTug3Num() == msg_overhead_nohead.vc3_index &&\
			tmp_obj->getVc12Num() == msg_overhead_nohead.vc12_index)
	{
		tmp_entity->getInstance();

		if(msg_overhead_nohead.op_code == OP_SET)	//set
        {
            tmp_entity->setJ2Mode((App_dev::J0Mode)msg_overhead_nohead.byte_mode);
            if(msg_overhead_nohead.direction == TX_DIR_OVH)
            {
                tmp_entity->setTxJ2Trace(msg_overhead_nohead.array);
            }
            else if(msg_overhead_nohead.direction == EXP_RX_DIR_OVH)
            {
                tmp_entity->setExpJ2Trace(msg_overhead_nohead.array);
            }                    
        }         
		else if(msg_overhead_nohead.op_code == OP_GET)
        {
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
        }  

        return true;
	}        

	return false;
}

bool CSdhQueue::alarm_report_func(INT8U *buffer, INT32U len)
{
    MSG_ALARM   alarm_msg;
    MSG_ALARM_NOHEAD alarm_msg_nohead;
    INT8U               tmp, len_tmp;
    INT8U               *send_buffer = NULL;
    INT8U               alarm_num = 0;
    std::map<int, MSG_ALARM_NOHEAD> map_data;
    INT32U  send_len = 0;
    std::map<INT32U, ALARM_INFO>::iterator itor;
    ALARM_INFO  alarm_info_tmp;

    if(!buffer || len <= 0)
        return false;

    map_data.clear();
    memset(&alarm_msg, 0, sizeof(alarm_msg));
    memcpy(&alarm_msg, buffer, sizeof(alarm_msg));
    alarm_num = 0;

    _alarm_mtx.lock();  
    for(itor = _report_alarm_map.begin(); itor!=_report_alarm_map.end();)
    {
        alarm_info_tmp = itor->second;
        //if(alarm_num < 10)
        {
            alarm_msg_nohead.rack_index = alarm_info_tmp.alarm_index.rack;
            alarm_msg_nohead.shelf_index = alarm_info_tmp.alarm_index.shelf;
            alarm_msg_nohead.slot_index = alarm_info_tmp.alarm_index.slot;
            alarm_msg_nohead.op_code = alarm_info_tmp.op_code == 2 ? OP_CLEAR : OP_SET;
            alarm_msg_nohead.nc_value = alarm_info_tmp.nc_value;
            alarm_msg_nohead.na_flag = alarm_info_tmp.na_flag;
            alarm_msg_nohead.alarm_code = alarm_info_tmp.alarm_code;
            memset(alarm_msg_nohead.alarm_obj, 0, sizeof(alarm_msg_nohead.alarm_obj));
            len_tmp = alarm_info_tmp.alarm_obj.length() > sizeof(alarm_msg_nohead.alarm_obj) ? sizeof(alarm_msg_nohead.alarm_obj) : alarm_info_tmp.alarm_obj.length();            
            memcpy(alarm_msg_nohead.alarm_obj, alarm_info_tmp.alarm_obj.c_str(), len_tmp);            
            memset(alarm_msg_nohead.time, 0, sizeof(alarm_msg_nohead.time));
            len_tmp = alarm_info_tmp.time.length() > sizeof(alarm_msg_nohead.time) ? sizeof(alarm_msg_nohead.time) : alarm_info_tmp.time.length();            
            memcpy(alarm_msg_nohead.time, alarm_info_tmp.time.c_str(), len_tmp);
            
            map_data[alarm_num] = alarm_msg_nohead;
            alarm_num++;
            _report_alarm_map.erase(itor++);
        }            
        //else{
        //    break;
       // }
    }

    _alarm_mtx.unlock();

    send_len = (sizeof(MSG_HEAD) + (sizeof(MSG_ALARM_NOHEAD) * alarm_num));
    send_buffer = new INT8U[send_len];
    memset(send_buffer, 0, send_len);

    printf("[DEBUG]%s()   map_data.num:%d \r\n", __func__, map_data.size());
    for(auto &it_data : map_data){
        memcpy(send_buffer + (sizeof(MSG_HEAD) + (sizeof(MSG_ALARM_NOHEAD) * it_data.first)), &(it_data.second), sizeof(MSG_ALARM_NOHEAD));
    }

    alarm_msg.msg_head.content_num = alarm_num;
    alarm_msg.msg_head.ruslt = 1;
    tmp = alarm_msg.msg_head.target;
    alarm_msg.msg_head.target = alarm_msg.msg_head.from;
    alarm_msg.msg_head.from = tmp;
    memcpy(send_buffer, &alarm_msg, sizeof(MSG_HEAD));
    _write_queue.enqueueNotification(new WorkNotification(send_buffer, send_len));
    delete[] send_buffer;
    send_buffer = NULL;

    return true;

}



#endif // DEBUG
