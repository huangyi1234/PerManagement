/***********************************************************************************
 * 文 件 名   : alarm_timer.cpp
 * 负 责 人   : sunl
 * 创建日期   : 2021年7月6日
 * 文件描述   : 定时器回调类的定义
 * 版权说明   : Copyright (c) 2008-2021   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#ifndef __ALARM_TIMER_CPP__
#define __ALARM_TIMER_CPP__

#include "alarm_timer.hpp"


/*****************************************************************************
 * 函 数 名  : TimerAlarmCheck.onTimer
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 定时器回调函数，间隔200ms检测两个slot的告警信息
 * 输入参数  : Timer& timer  alarm_check中定义的定时器
 * 输出参数  : 无
 * 返 回 值  : void
 * 调用关系  : 
 * 其    它  : 
                处理两个slot的告警
                1.定时检测告警信息针对port级，存入队列不做处理
                2.当CST30XX_MAX_SLOT的数量为奇数时，slot + 1不存在，不做处理
*****************************************************************************/
void TimerAlarmCheck::onTimer(Timer& timer)
{
    //printf("[Slot:%d,%d] ", _slot_index, _slot_index + 1);
	//std::cout << "Callback called after " << _sw.elapsed()/1000 << " milliseconds." << std::endl;

    SharedPtr<App_dev::CobjSdhId> tmp_obj;
	SharedPtr<App_dev::CsdhEntityBase> tmp_entity;
    std::map<unsigned int, ALARM_INFO> alarm_map;

	_mtx.lock();
	for(auto &it : _sdh_obj_map)
	{
        tmp_obj = (it.first);
        tmp_entity = (it.second);
        if(tmp_obj && ((tmp_obj->getSlot() == _slot_index) || (tmp_obj->getSlot() == _slot_index + 1)) && \
           (tmp_obj->getSdhIdType() == App_dev::OBJ_SDHID_PORT))
        {
            tmp_entity->getInstance();
            alarm_map = tmp_entity->get_alarm_map();
            _alarm_queue.enqueueNotification(new cAlarmNode(alarm_map));
        }        
    }   
	_mtx.unlock();
}

#endif // DEBUG
