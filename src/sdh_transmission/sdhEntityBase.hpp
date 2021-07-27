//////////////////////////////////////////////////////////////////
// sdhEntityBase.hpp
//
// Date: 2021/03/29
// Author: yoxia
// Change history:
//      2021/03/29: This is the initialization version.
//
// Discription:
// Implematation class(CsdhEntityBase)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef SDH_ENTITY_BASE_HPP
#define SDH_ENTITY_BASE_HPP
#include "objSdhId.hpp"
#include "sdhDef.hpp"


namespace App_dev{

class CsdhEntityBase
{
public:
	CsdhEntityBase();
	CsdhEntityBase(const CobjSdhId &myObj);
	virtual ~CsdhEntityBase();
	virtual void getInstance(){};

	/*port type*/
	virtual RetVal setPortType(const Port_Transmission_Type &type){return RV_SUCCESS;}
	virtual Port_Transmission_Type getPortType() const{return UNKNOWN;}

    /*RS overhead*/
    virtual RetVal setJ0Mode(const J0Mode &mode){return RV_SUCCESS;}
	virtual RetVal setTxJ0Trace(App_dev::UCHAR *provJ0Msg){return RV_SUCCESS;}
	virtual RetVal getRxJ0Trace(App_dev::UCHAR *recvJ0Msg){return RV_SUCCESS;}
	virtual RetVal setExpJ0Trace(App_dev::UCHAR *expJ0Msg){return RV_SUCCESS;}
	virtual RetVal setJ0Comp(const ComparisonType &mode){return RV_SUCCESS;}
    virtual JoMode getJ0Mode(){return unknown;}
    virtual RetVal getTxJ0Trace(App_dev::UCHAR *recvJ0Msg){return RV_SUCCESS;}
    virtual RetVal getExpJ0Trace(App_dev::UCHAR *recvJ0Msg){return RV_SUCCESS;}

    /*MS overhead*/
    virtual RetVal set_S1_value(const App_dev::UCHAR &value){return RV_SUCCESS;}
    virtual App_dev::UCHAR get_S1_value(){return 0xff;}

    //J1 overhead
    virtual RetVal setJ1Mode(const J0Mode &mode){return RV_SUCCESS;}
	virtual RetVal setTxJ1Trace(App_dev::UCHAR *provJ1Msg){return RV_SUCCESS;}
	virtual RetVal getRxJ1Trace(App_dev::UCHAR *recvJ1Msg){return RV_SUCCESS;}
	virtual RetVal setExpJ1Trace(App_dev::UCHAR *expJ1Msg){return RV_SUCCESS;}
	virtual RetVal setJ1Comp(const ComparisonType &mode){return RV_SUCCESS;}
    virtual JoMode getJ1Mode(){return unknown;}
    virtual RetVal getTxJ1Trace(App_dev::UCHAR *recvJ1Msg){return RV_SUCCESS;}
    virtual RetVal getExpJ1Trace(App_dev::UCHAR *recvJ1Msg){return RV_SUCCESS;}
   

    //C2 overhead
	virtual RetVal setTxC2Trace(App_dev::UCHAR &provJ1Msg){return RV_SUCCESS;}
	virtual RetVal getRxC2Trace(App_dev::UCHAR &recvJ1Msg){return RV_SUCCESS;}
	virtual RetVal setExpC2Trace(App_dev::UCHAR &expJ1Msg){return RV_SUCCESS;}
	virtual RetVal setC2Comp(const ComparisonType &mode){return RV_SUCCESS;}
    virtual RetVal getTxC2Trace(App_dev::UCHAR &recvJ1Msg){return RV_SUCCESS;}
    virtual RetVal getExpC2Trace(App_dev::UCHAR &recvJ1Msg){return RV_SUCCESS;}

    //vc12 J2
    virtual RetVal setJ2Mode(const J0Mode &mode){return RV_SUCCESS;}
	virtual RetVal setTxJ2Trace(App_dev::UCHAR *provJ2Msg){return RV_SUCCESS;}
	virtual RetVal getRxJ2Trace(App_dev::UCHAR *recvJ2Msg){return RV_SUCCESS;}
	virtual RetVal setExpJ2Trace(App_dev::UCHAR *expJ2Msg){return RV_SUCCESS;}
	virtual RetVal setJ2Comp(const ComparisonType &mode){return RV_SUCCESS;}
    virtual JoMode getJ2Mode(){return unknown;}
    virtual RetVal getTxJ2Trace(App_dev::UCHAR *recvJ2Msg){return RV_SUCCESS;}
    virtual RetVal getExpJ2Trace(App_dev::UCHAR *recvJ2Msg){return RV_SUCCESS;}


    //alarm
    virtual std::map<unsigned int, ALARM_INFO> &get_alarm_map(){return _alarm_map;}

    //asap configure
    virtual RetVal set_asap_info(const App_dev::UCHAR &asap_index, App_dev::UCHAR *asap_name);
    virtual App_dev::UCHAR get_asap_index(){return _asap_index;}
    virtual RetVal get_asap_name(App_dev::UCHAR *asap_name, App_dev::UINT len)
    {
        App_dev::UINT cp_len;

        cp_len = (len >= sizeof(_asap_name)) ? sizeof(_asap_name) : len;
        memcpy(asap_name, _asap_name, cp_len);

        return RV_SUCCESS;
    }
protected:
	RetVal calcCrc(App_dev::UCHAR *jxMsg);
    RetVal insertCrc7(App_dev::UCHAR *jxMsg);
	CobjSdhId _myObj;
    std::map<unsigned int, ALARM_INFO>   _alarm_map;
    App_dev::UCHAR _asap_index;
    App_dev::UCHAR _asap_name[16];
};

}//App_dev

#endif
