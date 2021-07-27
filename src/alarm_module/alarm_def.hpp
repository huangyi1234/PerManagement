#ifndef __ALARM_DEF_HPP__
#define __ALARM_DEF_HPP__


#include "../init_application.hpp"

//设备级告警1~255
#define ALARM_UNKNOWN                           0      //unknown 
#define ALARM_EQUIPMENT_EXCEPTION               1      //已经在位，但是不能配置或者已经被删除，但是单板、模块物理上未拔出
#define ALARM_OVERTEMPERATURE                   2      //设备检测到的温度超过一个指定的门限
#define ALARM_SHELF_COMMUNICATION               3      //主备子框通信异常
#define ALARM_MASTER_COMMUNICATION              4      //主备主控通信异常
#define ALARM_POWER_FUSE                        5      //保险管熔断/电源告警
#define ALARM_HARDWARE                          6      //硬件失效
#define ALARM_INITIALZATION                     7      //初始化失败
#define ALARM_REMOVE_LOGIC_DEVICE               8      //系统配置了单板或者模块，但是实际上并没有在位
#define ALARM_TRANSMMISION_BUS                  9      //检测到单板之间的业务总线故障
#define ALARM_SYNC_DATA_MASTER_SLAVER           10     //主、备主控之间正在进行数据库同步
#define ALARM_SOFTWARE_UPDATE                   11     //系统正在进行软件下载
#define ALARM_SLOT_MISMATCH                     12     //实际在位的单板或者模块与配置的单板、模块不一致
#define ALARM_FPGA_VERSION_MISMATCH             13     //FPGA版本与当前软件(系统)支持的不一致
#define ALARM_UNKNOWN_SLOT                      14     //不能识别当前硬件的类型
#define ALARM_SLOT_OFFLINE                      15     //配置的单板或者模块，不在配置的槽位


//业务级告警256~512
#define ALARM_RS_R_OOF                          256     //接收侧帧失步
#define ALARM_RS_R_LOF                          257     //接收侧帧定位丢失
#define ALARM_RS_BBE                            258     //再生段背景误码块性能事件


#define ALARM_MS_RDI                            270     //复用段远端缺陷指示
#define ALARM_MS_AIS                            271     //复用段告警指示信号
#define ALARM_MS_BBE                            272     //复用段背景误码块性能事件
#define ALARM_MS_EXC                            273     //复用段过误码告警
#define ALARM_MS_REI                            274     //复用段远端误码指示告警

#define ALARM_AU_AIS                            280     //管理单元告警指示信号
#define ALARM_AU_LOP                            281     //管理单元指针丢失告警

#define ALARM_HP_BBE                            290     //高阶通道背景误码块性能事件
#define ALARM_HP_TIM                            291     //高阶通道追踪识别符失配
#define ALARM_HP_SLM                            292     //高阶通道信号标记失配
#define ALARM_HP_UNEQ                           293     //高阶通道业务未装载
#define ALARM_HP_FEBBE                          294     //高阶通道远端背景误码块性能事件
#define ALARM_HP_REI                            295     //高阶通道远端误码指示告警
#define ALARM_HP_RDI                            296     //高阶通道远端缺陷指示告警


#define ALARM_TU_AIS                            310     //支路单元告警指示信号
#define ALARM_TU_LOP                            311     //支路单元指针丢失告警


#define ALARM_LP_BBE                            320     //低阶通道背景误码块性能事件
#define ALARM_LP_TIM                            321     //低阶通道追踪识别符失配
#define ALARM_LP_SLM                            322     //低阶通道信号标记失配
#define ALARM_LP_UNEQ                           323     //低阶通道业务未装载
#define ALARM_LP_FEBBE                          324     //低阶通道远端背景误码块性能事件
#define ALARM_LP_REI                            325     //低阶通道远端误码指示告警
#define ALARM_LP_RDI                            326     //低阶通道远端缺陷指示告警



#endif // DEBUG
