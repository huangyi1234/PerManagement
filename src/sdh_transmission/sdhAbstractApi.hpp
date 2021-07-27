//////////////////////////////////////////////////////////////////
// sdhAbstractApi.hpp
//
// Date: 2021/03/29
// Author: yoxia
// Change history:
//      2021/03/29: This is the initialization version.
//
// Discription:
// Implematation class(CsdhAbstrApi)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef SDH_ABSTRACT_API_HPP
#define SDH_ABSTRACT_API_HPP
#include "sdhDef.hpp"
#include "typeDef.hpp"
#include "objSdhId.hpp"

namespace App_dev{

class CsdhAbstrApi
{
public:
	CsdhAbstrApi(){};
	virtual ~CsdhAbstrApi(){};	
	///////////////////////////////////////////
	//                RS start               // 
	///////////////////////////////////////////
	//                J0                     //
	///////////////////////////////////////////
	virtual RetVal setJ0Mode(const CobjSdhId &Obj, const J0Mode &mode) = 0;
	virtual RetVal setTxJ0Trace(const CobjSdhId &Obj, 
								App_dev::UCHAR *provJ0Msg) = 0;
    virtual RetVal getRxJ0Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *recvJ0Msg) = 0;
    virtual RetVal setExpJ0Trace(const CobjSdhId &Obj, 
								 App_dev::UCHAR *expJ0Msg) = 0;
    virtual RetVal setJ0Comp(const CobjSdhId &Obj,
							 const ComparisonType &mode) = 0;
    virtual RetVal setPortType(const CobjSdhId &Obj,
    								 const Port_Transmission_Type &port_type) = 0;                                 
    
    //                  S1                  //
    virtual RetVal set_S1_value(const CobjSdhId &Obj, const App_dev::UCHAR &value) = 0;
    
	//                J1                   //
	virtual RetVal setJ1Mode(const CobjSdhId &Obj, const J0Mode &mode) = 0;
	virtual RetVal setTxJ1Trace(const CobjSdhId &Obj, 
								App_dev::UCHAR *provJ1Msg) = 0;
    virtual RetVal getRxJ1Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *recvJ1Msg) = 0;
    virtual RetVal setExpJ1Trace(const CobjSdhId &Obj, 
								 App_dev::UCHAR *expJ1Msg) = 0;
    virtual RetVal setJ1Comp(const CobjSdhId &Obj,
							 const ComparisonType &mode) = 0;


    //                C2                 //
	virtual RetVal setTxC2Trace(const CobjSdhId &Obj, 
								App_dev::UCHAR &provC2Msg) = 0;
    virtual RetVal getRxC2Trace(const CobjSdhId &Obj,
								App_dev::UCHAR &recvC2Msg) = 0;
    virtual RetVal setExpC2Trace(const CobjSdhId &Obj, 
								 App_dev::UCHAR &expC2Msg) = 0;
    virtual RetVal setC2Comp(const CobjSdhId &Obj,
							 const ComparisonType &mode) = 0;
    
    //J2
    virtual RetVal setJ2Mode(const CobjSdhId &Obj, const J0Mode &mode) = 0;
	virtual RetVal setTxJ2Trace(const CobjSdhId &Obj, 
								App_dev::UCHAR *provJ2Msg) = 0;
    virtual RetVal getRxJ2Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *recvJ2Msg) = 0;
    virtual RetVal setExpJ2Trace(const CobjSdhId &Obj, 
								 App_dev::UCHAR *expJ2Msg) = 0;
    virtual RetVal setJ2Comp(const CobjSdhId &Obj,
							 const ComparisonType &mode) = 0;

    //alarm get
    
    virtual RetVal get_alarm_check_map(const CobjSdhId &Obj,
    								 std::map<unsigned int, ALARM_INFO>  &alarm_map) = 0;
};
}//App_dev

#endif
