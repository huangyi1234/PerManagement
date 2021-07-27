//////////////////////////////////////////////////////////////////
// eqptFSM.hpp
//
// Date: 2021/03/19
// Author: yoxia
// Change history:
//      2021/03/19: This is the initialization version.
//
// Discription:
// Implematation eqpt common part
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef EQPT_SHM_HPP
#define EQPT_SHM_HPP

/* SHM struct only manage own shelf
ST_EQM_SHELF + ST_EQM_SHELF (CST30XX_MAX_SHELF)
ST_EQM_SLOT + ST_EQM_SLOT + .....(CST30XX_MAX_SLOT)
ST_EQM_PORT + ST_EQM_PORT + ......(CST30XX_MAX_PORT)(SLOT1)
ST_EQM_PORT + ST_EQM_PORT + ......(CST30XX_MAX_PORT)(SLOT2)
......................................................
ST_EQM_PORT + ST_EQM_PORT + ......(CST30XX_MAX_PORT)(SLOT[CST30XX_MAX_SLOT])
*/
#define CST30XX_MAX_SHELF   2
#define CST30XX_MAX_SLOT   17
#define CST30XX_MAX_PORT   12

#define SHM_ADMIN_STATUS_UNSIGNED   0
#define SHM_ADMIN_STATUS_IS_AINS    1
#define SHM_ADMIN_STATUS_IS         2
#define SHM_ADMIN_STATUS_OOS        3
#define SHM_ADMIN_STATUS_MT         4

#define SHM_PYH_OFFLINE             0
#define SHM_PYH_ONLINE              1

#define SHM_PYH_STATE_NORMAL             0
#define SHM_PYH_STATE_ABNORMAL           1
#define SHM_PYH_STATE_FAILED             2
#define SHM_PYH_STATE_MISMATCH           3


#define CST30XX_SHM_NAME    "EQM_SHM"

#define CST30XX_MSGQ_PATH   "."
#define CST30XX_MSGQ_PROJID 'p'
#define CST30XX_MSGQ_ALARM_TYPE 1
#pragma pack(1)

typedef struct{

}ST_EQM_SLOT_RI;

typedef struct{

}ST_EQM_PORY_RI;

typedef unsigned char	Uchar;
typedef unsigned short	Ushort;
typedef unsigned int	Uint32;
typedef long int	    Long;


typedef struct{
	Uchar	cNo;				/* 子架序列号 */
	Ushort	sType;				/* 子架类型 */
	Uchar	cMS;				/* 子架角色(主、从) */
	Ushort	sSlotNum;			/* 子架槽位个数 */
}ST_EQM_SHELF;

typedef struct{
	Uchar	cNo;				/* 槽位序列号 */
    Uchar	cOnline;			/* 槽位在位线 */
    Ushort	cAdminState;		/* 槽位Unassigned/Inservice/OutOfService/Maintenance */
	Ushort	sRealType;			/* 槽位实际类型 */
	Ushort	sCfgType;			/* 槽位配置类型 */
	Ushort	sPortNum;			/* 单板端口个数 */
}ST_EQM_SLOT;

typedef struct{
	Uchar	cNo;				/* 端口序列号 */
    Uchar	cOnline;			/* 端口在位线 */
    Ushort	cAdminState;		/* 端口Unassigned/Inservice/OutOfService/Maintenance */
	Ushort	sRealType;			/* 端口实际类型 */
	Ushort	sCfgType;			/* 端口配置类型 */
}ST_EQM_PORT;

typedef struct{
    Uint32  reserved_27:5;
    Uint32  rack:2;
    Uint32  shelf:2;
    Uint32  slot:5;
    Uint32  port:4;
    Uint32  reserved_0:14;
}ST_EQM_ALARM_INDEX;


typedef struct{
	Uchar	cNo;				    /* alarm 号 */
#define ALARM_NO_CARD_MISMATH    0    
    Uchar   cAlarmStatus;           /* alarm状态1:产生告警0:告警消失 */
#define ALARM_OPEN          1
#define ALARM_CLOSE         0
	Uchar	strDate[20];			/* 时间 */
    ALARM_CHECK_INDEX  alarm_index;
}ST_EQM_ALARM;

typedef struct{
    Long    msgType;
    ST_EQM_ALARM stData;
}ST_EQM_IPC_MSG;
#pragma pack()

#define EQM_SHM_LEN (sizeof(ST_EQM_SHELF) * CST30XX_MAX_SHELF + (sizeof(ST_EQM_SLOT) + (sizeof(ST_EQM_PORT) * CST30XX_MAX_PORT)) * CST30XX_MAX_SLOT)
#define EQM_SHM_LEN_SHELF_OFFSET(x)		(sizeof(ST_EQM_SHELF) * (x))
#define EQM_SHM_LEN_SLOT_OFFSET(x)		((sizeof(ST_EQM_SHELF) * CST30XX_MAX_SHELF) + (sizeof(ST_EQM_SLOT)) * (x))
#define EQM_SHM_LEN_PORT_OFFSET(x)		((sizeof(ST_EQM_SHELF) * CST30XX_MAX_SHELF) + (sizeof(ST_EQM_SLOT) * (CST30XX_MAX_SLOT)) + (sizeof(ST_EQM_PORT) * (x)))


#endif
