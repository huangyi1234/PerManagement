//////////////////////////////////////////////////////////////////
// sdhDef.hpp
//
// Date: 2021/02/22
// Author: yoxia
// Change history:
//      2021/02/22: This is the initialization version.
//
// Discription:
// Implematation for object type define 
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef SDH_DEF_HPP
#define SDH_DEF_HPP

#include <map>

namespace App_dev{
#define SDH_JX_LEN 16
typedef enum J0Mode{
    unknown = 0,
	J0_1BYTE = 1,
	J0_16BYTE = 2
}JoMode;

typedef enum ComparisonType{
	ENABLE = 1,
	DISABLE = 2
	}ComparisonType;




typedef enum Port_Transmission_Type{
	TYPE_DEFINE_1 = 1,
	UNKNOWN = 2,
	TYPE_DEFINE_2 = 66,

}Port_Transmission_Type;

}//App_dev

#pragma pack(1)

typedef enum Notification_Code_{
    UNKOWN_LEVEL = 0,
    CRITICAL_LEVEL = 1,
    MAJOR_LEVEL,
    MINOR_LEVEL,
    WARNING_LEVEL,
    NOT_REPORT_LEVEL,
    NON_ALARM_LEVEL,
}Notification_Code;

typedef enum Alarm_Op_Code_{
    Alarm_OP_UNKOWN = 0,
    Alarm_OP_SET = 1,
    Alarm_OP_CLEAR,
}Alarm_Op_Code;

/*
alarm_index说明
31~27bit    保留
26~25bit    2b'rack_index
24~23bit    2b'shelf_index
22~18bit    5b'slot_index
17~14bit    4b'port_index
13~7bit     7b'vc4_index
6~5bit      2b'vc3_index
4~2bit      3b'tug2_index
1~0bit      2b'vc12_index
*/
typedef struct{
    unsigned int  reserved_27:5;
    unsigned int  rack:2;
    unsigned int  shelf:2;
    unsigned int  slot:5;
    unsigned int  port:4;
    unsigned int  vc4:7;
    unsigned int  vc3:2;
    unsigned int  tug2:3;
    unsigned int  vc12:2;
}ALARM_CHECK_INDEX;

typedef struct _ALARM_INTO_{
    std::string         alarm_obj;
    std::string         time;
    bool                na_flag;
    Notification_Code   nc_value;
    Alarm_Op_Code       op_code;
    ALARM_CHECK_INDEX   alarm_index;
    unsigned int        alarm_code;
}ALARM_INFO, *PALARM_INFO;


#pragma pack()


#endif
