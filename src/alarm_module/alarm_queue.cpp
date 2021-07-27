/***********************************************************************************
 * 文 件 名   : alarm_queue.cpp
 * 负 责 人   : sunl
 * 创建日期   : 2021年7月6日
 * 文件描述   : 将来自设备级的告警map和业务级的告警map�-
                    ��队列中读出来进行处理
 * 版权说明   : Copyright (c) 2008-2021   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#ifndef __ALARM_QUEUE_CPP__
#define __ALARM_QUEUE_CPP__
#include "alarm_queue.hpp"

/*****************************************************************************
 * 函 数 名  : cWorkerAlarm.run
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 线程回调函数，读取队列中的告警进行处理
                   
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void
 * 调用关系  : 
 * 其    它  : 处理流程
            1。读取出来的map进行遍历，对比exist_alarm_map中的告警。索引值位alarm_code + alarm_index，alarm_index高位有保留不会溢出
            2。如果告警在exist_alarm_map有记录已存在，判断主动清除还是被高级告警抑制掉了需要清除
            3。经过告警抑制处理
            4。经过asap模块得到nc value
            5。是否上报，记录已经上报的告警
*****************************************************************************/
INT32U cAlarmNode::count = 0;

void cWorkerAlarm::run()
{
    ALARM_INFO  *alarm_tmp = NULL;
    std::map<INT32U, ALARM_INFO> exist_alarm_map;           //索引为alarm_code + alarm_index
    std::map<INT32U, ALARM_INFO>::iterator iter;
    std::map<INT32U, ALARM_INFO>::iterator iter_clear;
    INT32U  map_alarm_index = 0;
    INT32U  tmp_index;
    cAlarmControl alarm_control;

    exist_alarm_map.clear();
    
    while(1)
    {

        
        Notification::Ptr pNf(_queue.waitDequeueNotification());
        if (pNf)
        {    

            cAlarmNode::Ptr pWorkNf = pNf.cast<cAlarmNode>();
            if (pWorkNf)
            {
                for(auto &it : pWorkNf->get_alarm_map())
                {
                    alarm_tmp = &it.second;
                    if(!alarm_tmp)
                    {
                        continue; 
                    }

                    //上报或清除                    
                    memcpy(&tmp_index, &alarm_tmp->alarm_index, sizeof(INT32U));
                    map_alarm_index = alarm_tmp->alarm_code + tmp_index;
                    iter = exist_alarm_map.find(map_alarm_index);                     
                    if(iter != exist_alarm_map.end())
                    {
                        //std::cout << "Find！！！ alarm already exist." << std::endl;
                        //主动清除告警        ||   抑制清除告警(已经产生的告警被抑制了)
                        if((alarm_tmp->nc_value == NON_ALARM_LEVEL) || \
                            alarm_control.get_alarm_control_status(tmp_index, alarm_tmp->alarm_code))
                        {
                            std::cout << "clear[" << alarm_tmp->time << "]: --" << alarm_tmp->alarm_obj << std::endl;
                            alarm_control.update_alarm_control_relation(tmp_index, alarm_tmp->alarm_code, true);
                            exist_alarm_map.erase(iter);
                            alarm_tmp->op_code = Alarm_OP_CLEAR;
                            this->write_queue_alarm_info(*alarm_tmp);

                            _alarm_mtx.lock();
                            iter_clear = _report_alarm_map.find(map_alarm_index);  
                            //存在该告警键值对
                            if(iter_clear != _report_alarm_map.end())
                            {
                                _report_alarm_map.erase(iter_clear);             
                            }
                            else//不存在
                            {
                                _report_alarm_map[map_alarm_index] = *alarm_tmp; 
                            }                            
                            
                            _alarm_mtx.unlock();
                            printf("alarm queue size: %d\r\n", _report_alarm_map.size());
                        }
                        
                        continue;
                    }

                    //告警抑制模块
                    alarm_control.update_alarm_control_relation(tmp_index, alarm_tmp->alarm_code, false);   //更新当前告警的关系
                    if(alarm_control.get_alarm_control_status(tmp_index, alarm_tmp->alarm_code))
                        continue;
                    
                    //ASAP处理

                    
                    //记录已经上报的告警
                    exist_alarm_map[map_alarm_index] = *alarm_tmp;
                    this->write_queue_alarm_info(*alarm_tmp);
                    //上报告警                    
                    _alarm_mtx.lock();
                    _report_alarm_map[map_alarm_index] = *alarm_tmp;                    
                    std::cout << "report[" << alarm_tmp->time << "]: --" << alarm_tmp->alarm_obj << std::endl;
                    _alarm_mtx.unlock();     
                    printf("alarm queue size: %d\r\n", _report_alarm_map.size());
                }                
            }
        }
    }
}

/*****************************************************************************
 * 函 数 名  : cWorkerAlarm.write_queue_alarm_info
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月26日
 * 函数功能  : 主动上传告警，netconf会实时获取告警信息
 * 输入参数  : ALARM_INFO &alarm_info  上报告警的信息
 * 输出参数  : 无
 * 返 回 值  : void
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void cWorkerAlarm::write_queue_alarm_info(ALARM_INFO &alarm_info)
{
    MSG_ALARM   alarm_msg;
    INT8U               len_tmp;
    INT8U               *send_buffer = NULL;
    INT32U  send_len = 0;

    alarm_msg.rack_index = alarm_info.alarm_index.rack;
    alarm_msg.shelf_index = alarm_info.alarm_index.shelf;
    alarm_msg.slot_index = alarm_info.alarm_index.slot;
    alarm_msg.op_code = alarm_info.op_code == 2 ? OP_CLEAR : OP_SET;
    alarm_msg.nc_value = alarm_info.nc_value;
    alarm_msg.na_flag = alarm_info.na_flag;
    alarm_msg.alarm_code = alarm_info.alarm_code;
    memset(alarm_msg.alarm_obj, 0, sizeof(alarm_msg.alarm_obj));
    len_tmp = alarm_info.alarm_obj.length() > sizeof(alarm_msg.alarm_obj) ? sizeof(alarm_msg.alarm_obj) : alarm_info.alarm_obj.length();            
    memcpy(alarm_msg.alarm_obj, alarm_info.alarm_obj.c_str(), len_tmp);            
    memset(alarm_msg.time, 0, sizeof(alarm_msg.time));
    len_tmp = alarm_info.time.length() > sizeof(alarm_msg.time) ? sizeof(alarm_msg.time) : alarm_info.time.length();            
    memcpy(alarm_msg.time, alarm_info.time.c_str(), len_tmp);

    send_len = sizeof(MSG_ALARM);
    send_buffer = new INT8U[send_len];
    memset(send_buffer, 0, send_len);
    //printf("[DEBUG]%s() write alarm \r\n", __func__);


    alarm_msg.msg_head.content_num = 1;
    alarm_msg.msg_head.ruslt = 1;
    alarm_msg.msg_head.target = 0x02;
    alarm_msg.msg_head.from = 0x06;
    memcpy(send_buffer, &alarm_msg, sizeof(MSG_ALARM));
    //_write_queue.enqueueNotification(new WorkNotification(send_buffer, send_len));
    delete[] send_buffer;
    send_buffer = NULL;

    return;
}

#endif // DEBUG
