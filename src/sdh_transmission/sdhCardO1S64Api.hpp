//////////////////////////////////////////////////////////////////
// sdhCardO1S64Api.hpp
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
#ifndef SDH_CARD_O1S64_API_HPP
#define SDH_CARD_O1S64_API_HPP
#include<memory>
#include "sdhAbstractApi.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<map>
#include "typeDef.hpp"
#include "../alarm_module/alarm_def.hpp"
namespace App_dev{

#define SDH_CARD_O1S64_PORT_NUM      1
#define SDH_CARD_O1S64_VC4_NUM      64

class CsdhCardO1S64Api :public CsdhAbstrApi
{
public:
	static std::shared_ptr<CsdhCardO1S64Api> _pCardO1S64;
	CsdhCardO1S64Api();
	virtual ~CsdhCardO1S64Api();
	///////////////////////////////////////////
	//                RS start               //
	///////////////////////////////////////////
	//                J0                     //
	///////////////////////////////////////////
	virtual RetVal setJ0Mode(const CobjSdhId &Obj, const J0Mode &mode);
	virtual RetVal setTxJ0Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *provJ0Msg);
	virtual RetVal getRxJ0Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *recvJ0Msg);
	virtual RetVal setExpJ0Trace(const CobjSdhId &Obj,
								 App_dev::UCHAR *expJ0Msg);
	virtual RetVal setJ0Comp(const CobjSdhId &Obj,
							 const ComparisonType &mode);

	///////////////////////////////////////////
	//				port type				 //
	virtual RetVal setPortType(const CobjSdhId &Obj,
								 const Port_Transmission_Type &port_type);


    ///////////////////////////////////////////
    //                MS start               //
    ///////////////////////////////////////////
    //                S1                     //
    ///////////////////////////////////////////
    virtual RetVal set_S1_value(const CobjSdhId &Obj, const App_dev::UCHAR &value);

	///////////////////////////////////////////
	//                J1                  //
	///////////////////////////////////////////
	virtual RetVal setJ1Mode(const CobjSdhId &Obj, const J0Mode &mode);
	virtual RetVal setTxJ1Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *provJ1Msg);
	virtual RetVal getRxJ1Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *recvJ1Msg);
	virtual RetVal setExpJ1Trace(const CobjSdhId &Obj,
								 App_dev::UCHAR *expJ1Msg);
	virtual RetVal setJ1Comp(const CobjSdhId &Obj,
							 const ComparisonType &mode);


    ///////////////////////////////////////////
	//                C2                  //
	///////////////////////////////////////////
    virtual RetVal setTxC2Trace(const CobjSdhId &Obj, 
								App_dev::UCHAR &provC2Msg);
    virtual RetVal getRxC2Trace(const CobjSdhId &Obj,
								App_dev::UCHAR &recvC2Msg);
    virtual RetVal setExpC2Trace(const CobjSdhId &Obj, 
								 App_dev::UCHAR &expC2Msg);
    virtual RetVal setC2Comp(const CobjSdhId &Obj,
							 const ComparisonType &mode);  


    //                J2    VC12                 //
	///////////////////////////////////////////
	virtual RetVal setJ2Mode(const CobjSdhId &Obj, const J0Mode &mode);
	virtual RetVal setTxJ2Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *provJ2Msg);
	virtual RetVal getRxJ2Trace(const CobjSdhId &Obj,
								App_dev::UCHAR *recvJ2Msg);
	virtual RetVal setExpJ2Trace(const CobjSdhId &Obj,
								 App_dev::UCHAR *expJ2Msg);
	virtual RetVal setJ2Comp(const CobjSdhId &Obj,
							 const ComparisonType &mode);
	static std::shared_ptr<CsdhCardO1S64Api> getInstance();


    //alarm get
    virtual RetVal get_alarm_check_map(const CobjSdhId &Obj,
    								 std::map<unsigned int, ALARM_INFO>  &alarm_map);  
};

}//App_dev

#endif
