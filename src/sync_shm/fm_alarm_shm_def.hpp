
#ifndef __FM_ALARM_SHM_HPP__
#define __FM_ALARM_SHM_HPP__

#include "../eqm_module/eqptSHM.hpp"

/* SHM struct only manage asap and fm info
*shm head*
*8 asap tables*
*fm info{sdh map、cross}*
*/

#define ASAP_FILE_NUM_MAX            8
#define ASAP_INFO_NUM_MAX            20
#define CST30XX_ASAP_SHM_NAME       "FM_ALARM_SHM"

#define EACH_PORT_MAX_VC4_NUM       64          
#define EACH_VC4_MAX_VC3_NUM        3
#define EACH_VC3_MAX_VC12_NUM       21

#define CROSS_CONNECT_MAX_NUM       100

#pragma pack(1)

typedef struct _ASAP_INFO_{
    INT32U              cond_type;              //告警类型
    char                description[64];        //告警描述
    char                cond_class[16];         //告警对象
    INT8U               off_norm;               //告警是否为event ，如果为event 不需要清除
    INT8U               no_asap;                //是否支持asap配置
    INT8U               direction;              //接收或发送
    INT8U               nsa_level;              //不影响业务告警级别
    INT8U               sa_level;               //影响业务告警级别
}ASAP_INFO, *PASAP_INFO;

typedef struct _ASAP_TABLE_INFO_{
    char                asap_name[16];
    INT32U              asap_index;

    ASAP_INFO           asap_list[ASAP_INFO_NUM_MAX];
}ASAP_TABLE_INFO, *PASAP_TABLE_INFO;

typedef struct _ASAP_SHM_{
    char                shm_name[16];
    INT32U              shm_len;
    INT32U              flag;

    ASAP_TABLE_INFO     asap_table[ASAP_FILE_NUM_MAX];
}ASAP_SHM, *PASAP_SHM;

typedef struct _FM_INFO_VC12_{
    INT8U               vc12_index;

    INT8U               J2_send_mode;
    char                J2_send_array[16];
    INT8U               J2_expect_mode;
    char                J2_expect_array[16];
}FM_INFO_VC12, *PFM_INFO_VC12;


typedef struct _FM_INFO_VC3_{
    INT8U               vc3_index;

    INT8U               J1_send_mode;
    char                J1_send_array[16];
    INT8U               J1_expect_mode;
    char                J1_expect_array[16];
    INT8U               C2_send;
    INT8U               C2_expect;

    FM_INFO_VC12        vc12_list[EACH_VC3_MAX_VC12_NUM];
}FM_INFO_VC3, *PFM_INFO_VC3;


typedef struct _FM_INFO_VC4_{
    INT8U               vc4_index;

    INT8U               J1_send_mode;
    char                J1_send_array[16];
    INT8U               J1_expect_mode;
    char                J1_expect_array[16];
    INT8U               C2_send;
    INT8U               C2_expect;

    FM_INFO_VC3         vc3_list[EACH_VC4_MAX_VC3_NUM];
}FM_INFO_VC4, *PFM_INFO_VC4;


typedef struct _FM_INFO_PORT_{
    INT8U               port_index;
    INT8U               port_type;
    
    INT8U               J0_send_mode;
    char                J0_send_array[16];
    INT8U               J0_expect_mode;
    char                J0_expect_array[16];
    INT8U               S0_value;

    FM_INFO_VC4         vc4_list[EACH_PORT_MAX_VC4_NUM];
}FM_INFO_PORT, *PFM_INFO_PORT;

typedef struct _FM_INFO_SLOT_{
    INT8U               slot_index;
    INT8U               slot_type;
    INT8U               asap_index;
    char                asap_name[16];
    
    FM_INFO_PORT        port_list[CST30XX_MAX_PORT];
}FM_INFO_SLOT, *PFM_INFO_SLOT;

typedef struct _FM_SHM_{
    INT8U               rack_index;
    INT8U               shelf_index;
    
    FM_INFO_SLOT        slot_list[CST30XX_MAX_SLOT];    
}FM_SHM, *PFM_SHM;



typedef struct _CROSS_INFO_{
    INT32U          cross_index;

    INT8U           level;
    INT8U           direction;
    INT8U           source_band_index;
    INT8U           source_port_index;
    INT8U           source_vc4_index;
    INT32U          source_time_slot;       //源时隙范围

    INT8U           destination_band_index;
    INT8U           destination_port_index;
    INT8U           destination_vc4_index;
    INT32U          destination_time_slot;       //宿时隙范围
}CROSS_INFO, *PCROSS_INFO;


typedef struct _CROSS_SHM_{  
    CROSS_INFO      cross_list[CROSS_CONNECT_MAX_NUM];
}CROSS_SHM, *PCROSS_SHM;

#pragma pack()


#define CST30XX_OUR_ASAP_SHM_LEN    (sizeof(ASAP_SHM))
#define CST30XX_OUR_FM_SHM_LEN      ((sizeof(FM_SHM)) + (sizeof(CROSS_SHM)))
#define CST30XX_OUR_TOTAL_SHM_LEN   ((CST30XX_OUR_ASAP_SHM_LEN) + (CST30XX_OUR_FM_SHM_LEN))

#endif
