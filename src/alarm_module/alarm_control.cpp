/***********************************************************************************
 * 文 件 名   : alarm_control.cpp
 * 负 责 人   : sunl
 * 创建日期   : 2021年7月6日
 * 文件描述   : 告警抑制模块的相关代码
 * 版权说明   : Copyright (c) 2008-2021   xx xx xx xx 技术有限公司
 * 其    他   : 
 * 修改日志   : 
***********************************************************************************/

#ifndef __ALARM_CONTROL_CPP__
#define __ALARM_CONTROL_CPP__

#include "alarm_control.hpp"

static INT32U  g_array_relation[][16] = {
    {2, ALARM_SLOT_MISMATCH, ALARM_SLOT_OFFLINE},
    {3, ALARM_SLOT_OFFLINE, ALARM_POWER_FUSE, ALARM_HARDWARE},
    {2, ALARM_OVERTEMPERATURE, ALARM_SLOT_OFFLINE},
    {3, ALARM_SHELF_COMMUNICATION, ALARM_FPGA_VERSION_MISMATCH, ALARM_SLOT_OFFLINE},

};


/*****************************************************************************
 * 函 数 名  : cAlarmControlSub.operator==
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 重载cAlarmControlSub对象的‘==’
 * 输入参数  : cAlarmControlSub &temp  需要做比较的对象
 * 输出参数  : 无
 * 返 回 值  : bool
 * 调用关系  : update_alarm_control_relation函数中调用
 * 其    它  : 

*****************************************************************************/
bool cAlarmControlSub::operator==(const cAlarmControlSub &temp) const
{
    if(temp.alarm_code == alarm_code && temp.alarm_index == alarm_index)
    {
        return true;
    }
    else
        return false;
}

/*****************************************************************************
 * 函 数 名  : cAlarmControl.cAlarmControl
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 告警抑制的构造函数，加载抑制关系
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : cAlarmControl
 * 调用关系  : alarm_queue.cpp 中处理告警时实例化
 * 其    它  : 

*****************************************************************************/
cAlarmControl::cAlarmControl()
{
    for(INT8U i = 0; i < (sizeof(g_array_relation)/sizeof(g_array_relation[0])); i++)
        add_alarm_control_relation(&g_array_relation[i][1], g_array_relation[i][0]);

    //printf("map size:%d\r\n", _alarm_control_map.size());
    //debug_info();
}

/*****************************************************************************
 * 函 数 名  : cAlarmControl.init_alarm_control_info
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 初始化需要存储告警抑制关系的容器
 * 输入参数  : ALARM_CONTROL_INFO &alarm_control_info  告警抑制容器
 * 输出参数  : 无
 * 返 回 值  : void
 * 调用关系  : add_alarm_control_relation函数调用
 * 其    它  : 

*****************************************************************************/
void cAlarmControl::init_alarm_control_info(ALARM_CONTROL_INFO &alarm_control_info)
{
    alarm_control_info.alarm_vc.clear();
    alarm_control_info.parent_alarm_vc.clear();
    alarm_control_info.child_alarm_vc.clear();
    alarm_control_info.already_parent_alarm_vc.clear();
    alarm_control_info.already_child_alarm_vc.clear();
}

/*****************************************************************************
 * 函 数 名  : cAlarmControl.add_alarm_control_relation
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 填充告警抑制容器中的parent_vc  和  child_vc
 * 输入参数  : INT32U *alarm_array  一条告警抑制关系
               INT32U alarm_num     这条抑制关系中的告警个数
 * 输出参数  : 无
 * 返 回 值  : bool
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
bool cAlarmControl::add_alarm_control_relation(INT32U *alarm_array, INT32U alarm_num)
{
    std::map<INT32U, ALARM_CONTROL_INFO>::iterator iter;
    ALARM_CONTROL_INFO  *alarm_ele = NULL;
        
    if(!alarm_array || alarm_num < 2)
    {
        printf("add_alarm_control_relation error!\r\n");
        return false;
    }

    for(INT8U i = 0; i < alarm_num; i++)
    {
        iter = _alarm_control_map.find(alarm_array[i]);  
        //存在该告警的抑制键值对
        if(iter != _alarm_control_map.end())
        {
            alarm_ele = &(iter->second);            
        }
        else
        {
            alarm_ele = new ALARM_CONTROL_INFO();
            init_alarm_control_info(*alarm_ele);
        }

        if(i == 0)  //第一个
        {
            alarm_ele->child_alarm_vc.push_back(alarm_array[i + 1]);
        }
        else if(i == (alarm_num - 1))        //最后一个
        {
            alarm_ele->parent_alarm_vc.push_back(alarm_array[i - 1]);
        }
        else    //中间的
        {
            alarm_ele->child_alarm_vc.push_back(alarm_array[i + 1]);
            alarm_ele->parent_alarm_vc.push_back(alarm_array[i - 1]);
        }

        //不存在加入键值对
        if(iter == _alarm_control_map.end())
        {            
            _alarm_control_map.insert(std::pair<INT32U, ALARM_CONTROL_INFO>(alarm_array[i], *alarm_ele));
            delete alarm_ele;
        }
    }

    return true;
}

/*****************************************************************************
 * 函 数 名  : cAlarmControl.update_alarm_control_relation
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 根据监测到的告警和存储的告警抑制关系�-
                   �将产生告警的位置记录下来
 * 输入参数  : INT32U alarm_index  产生告警的位置
               INT32U alarm_code   告警码
               bool clear          true：清除   false：添加
 * 输出参数  : 无
 * 返 回 值  : bool
 * 调用关系  : alarm_queue.cpp 中处理告警map时调用
 * 其    它  : 

*****************************************************************************/
bool cAlarmControl::update_alarm_control_relation(INT32U alarm_index, INT32U alarm_code, bool clear)
{
    std::map<INT32U, ALARM_CONTROL_INFO>::iterator iter, iter2;
    std::vector<INT32U>::iterator iter_vc;
    std::vector<cAlarmControlSub>::iterator iter_already_vc;
    cAlarmControlSub alarm_control_sub;
    bool exist_flag = false;
    
    if(!alarm_index || !alarm_code)
    {
        printf("update_alarm_control_relation error!\r\n");
        return false;
    }

    alarm_control_sub.alarm_code = alarm_code;
    alarm_control_sub.alarm_index= alarm_index;
    
    iter = _alarm_control_map.find(alarm_code);
    if(iter != _alarm_control_map.end())
    {
        iter_vc = find((iter->second).alarm_vc.begin(), (iter->second).alarm_vc.end(), alarm_index);
        if ( iter_vc != (iter->second).alarm_vc.end())  //该位置告警已存在
        {
            if(clear)
                (iter->second).alarm_vc.erase(iter_vc);         //删除产生告警的位置
        }
        else
        {
            if(!clear)
                (iter->second).alarm_vc.push_back(alarm_index); //插入产生告警的位置
        }


        //根据当前parent_alarm_vc 更新它们的already_child_alarm_vc
        for(auto &it : (iter->second).parent_alarm_vc)
        {
            iter2 = _alarm_control_map.find(it);
            if(iter2 != _alarm_control_map.end())
            {
                exist_flag = false;
                for(auto &_already_vc : ((iter2->second).already_child_alarm_vc))
                {
                    if(_already_vc == alarm_control_sub)
                    {
                        exist_flag = true;
                        break;
                    }                    
                }                

                if(exist_flag)
                {
                    if(clear)
                    {
                        iter_already_vc = find((iter2->second).already_child_alarm_vc.begin(), (iter2->second).already_child_alarm_vc.end(), alarm_control_sub);                        
                        (iter2->second).already_child_alarm_vc.erase(iter_already_vc);          //删除被抑制告警中的子告警位置
                    }  
                }
                else
                {
                    if(!clear)
                        (iter2->second).already_child_alarm_vc.push_back(alarm_control_sub);    
                }
            }
        }

        //根据当前child_alarm_vc 更新它们的already_parent_alarm_vc
        for(auto &it : (iter->second).child_alarm_vc)
        {
            iter2 = _alarm_control_map.find(it);
            if(iter2 != _alarm_control_map.end())
            {
                exist_flag = false;
                for(auto &_already_vc : ((iter2->second).already_parent_alarm_vc))
                {
                    if(_already_vc == alarm_control_sub)
                    {
                        exist_flag = true;
                        break;
                    }                    
                }    

                if(exist_flag)
                {
                    if(clear)
                    {
                        iter_already_vc = find((iter2->second).already_parent_alarm_vc.begin(), (iter2->second).already_parent_alarm_vc.end(), alarm_control_sub);                        
                        (iter2->second).already_parent_alarm_vc.erase(iter_already_vc);//删除已抑制告警中的父告警位置
                    } 
                }
                else
                {
                    if(!clear)
                        (iter2->second).already_parent_alarm_vc.push_back(alarm_control_sub);    
                }
            }
        }
    }
    
    return true;
}

/*****************************************************************************
 * 函 数 名  : cAlarmControl.debug_info
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 打印容器中存储的抑制信息
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void cAlarmControl::debug_info()
{
    for(auto &it : _alarm_control_map)
    {
        printf("alarm code: %d\n", it.first);

        printf("\t alarm_vc(%d):\n\t\t", it.second.alarm_vc.size());
        for(auto &it2 : it.second.alarm_vc)
            printf(" %d\n", it2);
        printf("\n");
        
        printf("\t parent_alarm_vc(%d):\n\t\t", it.second.parent_alarm_vc.size());
        for(auto &it3 : it.second.parent_alarm_vc)
            printf(" %d", it3);
        printf("\n");
        
        printf("\t child_alarm_vc(%d):\n\t\t", it.second.child_alarm_vc.size());
        for(auto &it4 : it.second.child_alarm_vc)
            printf(" %d\n", it4);
        printf("\n");

        printf("\t a_parent_alarm_vc(%d):\n\t\t", it.second.already_parent_alarm_vc.size());
        for(auto &it5 : it.second.already_parent_alarm_vc)
            printf("  index:%d code:%d   ", it5.alarm_index, it5.alarm_code);
        printf("\n");
        
        printf("\t a_child_alarm_vc(%d):\n\t\t", it.second.already_child_alarm_vc.size());
        for(auto &it6 : it.second.already_child_alarm_vc)
            printf("  index:%d code:%d  ", it6.alarm_index, it6.alarm_code);
        printf("\n");
    }
}

/*****************************************************************************
 * 函 数 名  : cAlarmControl.get_alarm_control_status
 * 负 责 人  : sunl
 * 创建日期  : 2021年7月6日
 * 函数功能  : 获取当前告警是否被抑制
 * 输入参数  : INT32U alarm_index  产生告警位置
               INT32U alarm_code   告警码
 * 输出参数  : 无
 * 返 回 值  : bool
 * 调用关系  : alarm_queue.cpp 中处理告警map时调用
 * 其    它  : 
                1、告警的抑制遵从高级别告警抑制低级别告警

*****************************************************************************/
bool cAlarmControl::get_alarm_control_status(INT32U alarm_index, INT32U alarm_code)
{
    ALARM_CHECK_INDEX  tmp_index1, tmp_index2;
    std::map<INT32U, ALARM_CONTROL_INFO>::iterator iter;
    std::vector<INT32U>::iterator iter_vc;
    std::vector<cAlarmControlSub>::iterator iter_already_vc;
    cAlarmControlSub alarm_control_sub;
    
    if(!alarm_index || !alarm_code)
    {
        printf("get_alarm_control_status error!\r\n");
        return false;
    }
    
    memset(&tmp_index1, 0, sizeof(tmp_index1));
    memset(&tmp_index2, 0, sizeof(tmp_index2));
    memcpy(&tmp_index1, &alarm_index, sizeof(ALARM_CHECK_INDEX));
    alarm_control_sub.alarm_code = alarm_code;
    alarm_control_sub.alarm_index = alarm_index;
    
    iter = _alarm_control_map.find(alarm_code);
    if(iter != _alarm_control_map.end())
    {
        iter_vc = find((iter->second).alarm_vc.begin(), (iter->second).alarm_vc.end(), alarm_index);
        if ( iter_vc != (iter->second).alarm_vc.end())  //该位置告警已存在
        {           
            for(auto &it : (iter->second).already_parent_alarm_vc)
            {
                memcpy(&tmp_index2, &it.alarm_index, sizeof(ALARM_CHECK_INDEX));
                if(tmp_index1.rack == tmp_index2.rack && tmp_index1.shelf == tmp_index2.shelf \
                    && tmp_index1.slot == tmp_index2.slot)
                {
                    if(tmp_index2.port && (tmp_index1.port == tmp_index2.port))
                    {
                        if(tmp_index2.vc4 && (tmp_index1.vc4 == tmp_index2.vc4))
                        {
                            if(tmp_index2.vc3 && (tmp_index1.vc3 == tmp_index2.vc3))
                            {
                                if(tmp_index2.tug2 && (tmp_index1.tug2 == tmp_index2.tug2))
                                {
                                    if(tmp_index2.vc12 && (tmp_index1.vc12 == tmp_index2.vc12))
                                    {
                                        //printf("alarm control level:vc12\r\n");
                                        return true;
                                    }
                                    else if(!tmp_index2.vc12)
                                    {
                                        //printf("alarm control level:tug2\r\n");
                                        return true;
                                    }
                                }
                                else if(!tmp_index2.tug2)
                                {
                                    //printf("alarm control level:vc3\r\n");
                                    return true;
                                }
                            }
                            else if(!tmp_index2.vc3)
                            {
                                //printf("alarm control level:vc4\r\n");
                                return true;
                            }
                        }
                        else if(!tmp_index2.vc4)
                        {
                            //printf("alarm control level:port\r\n");
                            return true;
                        }
                    }
                    else if(!tmp_index2.port)
                    {
                        //printf("alarm control level:slot\r\n");
                        return true;
                    }
                }
            }
        }                        
    }

    return false;
}


#endif // DEBUG

