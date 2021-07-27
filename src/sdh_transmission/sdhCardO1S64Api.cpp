//////////////////////////////////////////////////////////////////
// sdhCardO1S64Api.cpp
//
// Date: 2021/03/29
// Author: yoxia
// Change history:
//      2021/03/29: This is the initialization version.
//
// Discription:
// Implematation class(CsdhCardO1S64Api)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#include "sdhCardO1S64Api.hpp"

namespace App_dev{


std::shared_ptr<CsdhCardO1S64Api> CsdhCardO1S64Api::_pCardO1S64;

CsdhCardO1S64Api::CsdhCardO1S64Api() :
	CsdhAbstrApi()
{

}

CsdhCardO1S64Api::~CsdhCardO1S64Api()	
{
}

///////////////////////////////////////////
//                RS start               // 
///////////////////////////////////////////
//                J0                     //
///////////////////////////////////////////
RetVal CsdhCardO1S64Api::setJ0Mode(const CobjSdhId &Obj, const J0Mode &mode)
{
	return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setTxJ0Trace(const CobjSdhId &Obj, 
								App_dev::UCHAR *provJ0Msg)
{
	return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::getRxJ0Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *recvJ0Msg)
{
	return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setExpJ0Trace(const CobjSdhId &Obj, 
								 App_dev::UCHAR *expJ0Msg)
{
	return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setJ0Comp(const CobjSdhId &Obj,
							 const ComparisonType &mode)
{
	return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setPortType(const CobjSdhId &Obj,
								 const Port_Transmission_Type &port_type)
{
    printf("[O1S64_driver]%s rack:%d shelf:%d slot:%d port:%d value:%d\r\n", __func__, Obj.getRack(),\
        Obj.getShelf(), Obj.getSlot(), Obj.getPort(), port_type);
	return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::set_S1_value(const CobjSdhId &Obj, const App_dev::UCHAR &value)
{
	printf("[O1S64_driver]%s rack:%d shelf:%d slot:%d port:%d value:%d\r\n", __func__, Obj.getRack(),\
        Obj.getShelf(), Obj.getSlot(), Obj.getPort(), value);
	return RV_SUCCESS;
}

std::shared_ptr<CsdhCardO1S64Api> CsdhCardO1S64Api::getInstance()
{
	if(!_pCardO1S64){
		_pCardO1S64 =  std::make_shared<CsdhCardO1S64Api>();
	}
	return(_pCardO1S64);
}

//                J1                     //
///////////////////////////////////////////
RetVal CsdhCardO1S64Api::setJ1Mode(const CobjSdhId &Obj, const J0Mode &mode)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setTxJ1Trace(const CobjSdhId &Obj, 
                                App_dev::UCHAR *provJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::getRxJ1Trace(const CobjSdhId &Obj,
                                App_dev::UCHAR *recvJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setExpJ1Trace(const CobjSdhId &Obj, 
                                 App_dev::UCHAR *expJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setJ1Comp(const CobjSdhId &Obj,
                             const ComparisonType &mode)
{
    return RV_SUCCESS;
}
                             


//                C2                     //
///////////////////////////////////////////
RetVal CsdhCardO1S64Api::setTxC2Trace(const CobjSdhId &Obj, 
                                App_dev::UCHAR &provJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::getRxC2Trace(const CobjSdhId &Obj,
                                App_dev::UCHAR &recvJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setExpC2Trace(const CobjSdhId &Obj, 
                                 App_dev::UCHAR &expJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setC2Comp(const CobjSdhId &Obj,
                             const ComparisonType &mode)
{
    return RV_SUCCESS;
}                            

//                J2   vc12               //
///////////////////////////////////////////
RetVal CsdhCardO1S64Api::setJ2Mode(const CobjSdhId &Obj, const J0Mode &mode)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setTxJ2Trace(const CobjSdhId &Obj, 
                                App_dev::UCHAR *provJ2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::getRxJ2Trace(const CobjSdhId &Obj,
                                App_dev::UCHAR *recvJ2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setExpJ2Trace(const CobjSdhId &Obj, 
                                 App_dev::UCHAR *expJ2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhCardO1S64Api::setJ2Comp(const CobjSdhId &Obj,
                             const ComparisonType &mode)
{
    return RV_SUCCESS;
}



RetVal CsdhCardO1S64Api::get_alarm_check_map(const CobjSdhId &Obj,
                                         std::map<unsigned int, ALARM_INFO>  &alarm_map)
{
    ALARM_INFO alarm_tmp;
    unsigned int   alarm_id;
    unsigned char   rack_index, shelf_index, slot_index, port_index, vc4_index;
    char obj_str[32], obj_str1[32], obj_str2[32];
    std::string current_time;
    cMyTime my_time(current_time);
    static INT8U    xxx;

    memset(&alarm_tmp, 0, sizeof(alarm_tmp));
    memset(obj_str, 0, sizeof(obj_str));
    alarm_map.clear();

    alarm_id = 0;
    rack_index = Obj.getRack();
    shelf_index = Obj.getShelf();
    slot_index = Obj.getSlot();
    sprintf(obj_str, "Rack%d-Shelf%d-Slot%d", rack_index, shelf_index, slot_index);

    
    /*检测子板的告警寄存器，将告警信息存入map*/
    for(port_index = 1; port_index < SDH_CARD_O1S64_PORT_NUM + 1; port_index++)
    {
        vc4_index = 0;
        memset(obj_str1, 0, sizeof(obj_str1));
        sprintf(obj_str1, "%s-Port%d", obj_str, port_index);
        alarm_tmp.alarm_index.rack = rack_index;
        alarm_tmp.alarm_index.shelf = shelf_index;
        alarm_tmp.alarm_index.slot = slot_index;
        alarm_tmp.alarm_index.port =  port_index;
        alarm_tmp.alarm_obj = obj_str1;
        alarm_tmp.alarm_code = ALARM_SLOT_MISMATCH;
        alarm_tmp.time = my_time.current_time();
        alarm_tmp.nc_value = CRITICAL_LEVEL;
        alarm_tmp.na_flag = 1;
        alarm_tmp.op_code = Alarm_OP_SET;

        //printf("xxxx:%d\r\n", xxx);
        if(++xxx > 1 )
        {
            xxx = 0;
            alarm_map.insert(std::pair<unsigned int, ALARM_INFO>(alarm_id++, alarm_tmp));
        }
        
        for(vc4_index = 1; vc4_index < SDH_CARD_O1S64_VC4_NUM + 1; vc4_index++)
        {
            memset(obj_str2, 0, sizeof(obj_str2));
            sprintf(obj_str2, "%s-VC4_%d", obj_str1, vc4_index);           
            alarm_tmp.alarm_index.vc4 = vc4_index;
            alarm_tmp.alarm_obj = obj_str2;
            alarm_tmp.alarm_code = ALARM_SLOT_OFFLINE;  
            alarm_tmp.time = my_time.current_time();
            alarm_map.insert(std::pair<unsigned int, ALARM_INFO>(alarm_id++, alarm_tmp));
        }        
    }
    
    //printf("O1S64 map size:%d\r\n", alarm_map.size());
    return RV_SUCCESS;
}


}//App_dev

