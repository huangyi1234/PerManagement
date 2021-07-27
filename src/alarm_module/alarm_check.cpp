/***********************************************************************************
 * 文 件 名   : alarm_check.cpp
 * 负 责 人   : sunl
 * 创建日期   : 2021年7月6日
 * 文件描述   : 告警监测的顶层类描述
 * 版权说明   : Copyright (c) 2008-2021   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#ifndef __ALARM_CHECK_CPP__
#define __ALARM_CHECK_CPP__

#include "alarm_check.hpp"


/*****************************************************************************
 * 函 数 名  : cAlarmCheck.run
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : app中起的告警监测线程回调函数
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void
 * 调用关系  : 
 * 其    它  : 
                1.等待eqm数据读取到
                2.初始化消息队列用于获取设备级告警
                3.初始化定时器获取slot告警    （初始化CST30XX_MAX_SLOT/2个定时器，间隔200ms取相邻两个slot告警状态）
                4.读取消息队列中的设备告警，将监测到的告警转化为alarm_queue中处理的map结构写入_alarm_queue

*****************************************************************************/
void cAlarmCheck::run()
{
    INT8U slot_index = 0, alarm_num = 0;
    std::map<INT32U, ALARM_INFO>   device_alarm_map;
    Timer *timer[CST30XX_MAX_SLOT];
    TimerAlarmCheck *timer_slot[CST30XX_MAX_SLOT];
    int rtn = 0;
    ST_EQM_IPC_MSG  eqm_msg;
    struct msqid_ds msq_buf;
    ALARM_INFO  alarm_info;
    INT8S       obj_str[32];

    while(get_eqm_start() == 0){Thread::sleep(1);}

    if(ipc_eqm_device_alarm_init() == false)
    {
        printf("ipc eqm init error\r\n");
        return;
    }

    _alarm_queue.clear();
    _report_alarm_map.clear();
	cWorkerAlarm worker_alarm("Alarm queue", _alarm_queue, _report_alarm_map, _alarm_mtx, _write_queue);
	Poco::ThreadPool::defaultPool().start(worker_alarm);

    for(INT8U i = 0; i < CST30XX_MAX_SLOT; i++)
    {
        slot_index = i + 1;
        if((slot_index % 2) == 1)
        {
            timer[i] = new Timer(100*slot_index, 200);
            timer_slot[i] = new TimerAlarmCheck(slot_index, _sdh_obj_map, _mtx, _alarm_queue);
            timer[i]->start(TimerCallback<TimerAlarmCheck>(*timer_slot[i], &TimerAlarmCheck::onTimer));
        }
    }

	while(1)
    {
        Thread::sleep(2000);
        //检测设备级告警
        device_alarm_map.clear();
        memset(&msq_buf, 0, sizeof(msq_buf));
        if(msgctl(_msq_id, IPC_STAT, &msq_buf) < 0)
        {
            printf("get msqid_ds error!! \n");
            continue;
        }

        printf("eqm msq msg num:%ld\r\n", msq_buf.msg_qnum);
        for(alarm_num = 0; alarm_num < msq_buf.msg_qnum; alarm_num++)
        {
            //block recv
            rtn = msgrcv(_msq_id,(void *)&eqm_msg, sizeof(ST_EQM_ALARM), 0, 0);
    		if(rtn != sizeof(ST_EQM_ALARM))
    		{
    		    perror("msgrcv:");
    			printf("eqm queue receive msg error rtn:%d\n", rtn);
    			continue;
    		}
    

            memset(obj_str, 0, sizeof(obj_str));
            if(eqm_msg.stData.alarm_index.rack != 0)
                sprintf(obj_str, "Rack%d", eqm_msg.stData.alarm_index.rack);
            
            if(eqm_msg.stData.alarm_index.shelf != 0)
                sprintf(obj_str+strlen(obj_str), "-Shelf%d", eqm_msg.stData.alarm_index.shelf);

            if(eqm_msg.stData.alarm_index.slot != 0)
                sprintf(obj_str+strlen(obj_str), "-Slot%d", eqm_msg.stData.alarm_index.slot);
            
            if(eqm_msg.stData.alarm_index.port != 0)
                sprintf(obj_str+strlen(obj_str), "-Port%d", eqm_msg.stData.alarm_index.port);
                      
            memset(&alarm_info, 0, sizeof(alarm_info));
            alarm_info.alarm_obj = obj_str;
            alarm_info.alarm_code = eqm_msg.stData.cNo;
            alarm_info.alarm_index = eqm_msg.stData.alarm_index;          
            alarm_info.time = (char *)eqm_msg.stData.strDate;
            alarm_info.nc_value = MAJOR_LEVEL;
            alarm_info.op_code = eqm_msg.stData.cAlarmStatus == ALARM_OPEN ? Alarm_OP_SET : Alarm_OP_CLEAR;

            device_alarm_map.insert(std::pair<INT32U, ALARM_INFO>(alarm_num, alarm_info));            
        }

        if(device_alarm_map.size() > 0)
        {
            _alarm_queue.enqueueNotification(new cAlarmNode(device_alarm_map));
        }      
 
    }   

    for(INT8U i = 0; i < CST30XX_MAX_SLOT; i++)
    {
        slot_index = i + 1;
        if((slot_index % 2) == 1)
        {
            timer[i]->stop();
            delete timer_slot[i];
            delete timer[i];
        }        
    }
}

/*****************************************************************************
 * 函 数 名  : cAlarmCheck.ipc_eqm_device_alarm_init
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 初始化消息队列，与eqm进程进行通信获取�-
                   �备级告警
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : bool
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
bool cAlarmCheck::ipc_eqm_device_alarm_init()
{
    key_t key;

    //init msg queue
	key = ftok(CST30XX_MSGQ_PATH, CST30XX_MSGQ_PROJID);
    if(key < 0)
    {
        printf("%s()  ftok  error.\r\n", __func__);
        return false;
    }

    
	_msq_id = msgget(key,0666|IPC_CREAT);
	if(_msq_id == -1)
	{
		printf("open msg error \n");
		return false;
	}

    return true;
}


#endif // DEBUG

