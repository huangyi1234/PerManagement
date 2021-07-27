#ifndef __ALARM_CONTROL_HPP__
#define __ALARM_CONTROL_HPP__


#include "../init_application.hpp"
#include "../eqm_module/eqptSHM.hpp"
#include "alarm_def.hpp"
#include<algorithm>
#include<vector>
#include<map>

class cAlarmControlSub{
public:
    
    cAlarmControlSub(){alarm_index = 0;alarm_code = 0;}
    ~cAlarmControlSub(){}

    INT32U      alarm_index;
    INT32U      alarm_code;

    bool operator==(const cAlarmControlSub &temp) const;
};


typedef struct ALARM_CONTROL_INFO_{
    std::vector<INT32U> alarm_vc;                                   //产生告警位置
    std::vector<INT32U> parent_alarm_vc;                            //被哪些告警抑制 
    std::vector<INT32U> child_alarm_vc;                             //抑制了哪些告警
    std::vector<cAlarmControlSub> already_parent_alarm_vc;         //已经被哪些告警对象给抑制了
    std::vector<cAlarmControlSub> already_child_alarm_vc;          //已经抑制了的告警对象
}ALARM_CONTROL_INFO, *PALARM_CONTROL_INFO;


class cAlarmControl{
public:
    cAlarmControl();
    ~cAlarmControl()
    {
        std::cout << "cAlarmControl  Exception " << std::endl;
    }

    void init_alarm_control_info(ALARM_CONTROL_INFO &alarm_control_info);
    bool add_alarm_control_relation(INT32U *alarm_array, INT32U alarm_num);
    bool update_alarm_control_relation(INT32U alarm_index, INT32U alarm_code, bool clear);
    void debug_info();
    bool get_alarm_control_status(INT32U alarm_index, INT32U alarm_code);

    
private:
    std::map<INT32U, ALARM_CONTROL_INFO> _alarm_control_map;
};


#endif // DEBUG
