//////////////////////////////////////////////////////////////////
// sdhTu12Entity.hpp
//
// Date: 2021/03/29
// Author: yoxia
// Change history:
//      2021/03/29: This is the initialization version.
//
// Discription:
// Implematation class(CsdhTu12Entity)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef SDH_TU12_ENTITY_HPP
#define SDH_TU12_ENTITY_HPP
#include<memory>
#include "sdhDef.hpp"
#include "typeDef.hpp"
#include "objSdhId.hpp"
#include "sdhEntityBase.hpp"
#include "sdhAbstractApi.hpp"

namespace App_dev{

class CsdhTu12Entity : public CsdhEntityBase
{
public:
	CsdhTu12Entity();
	CsdhTu12Entity(const CobjSdhId &myObj);
	virtual ~CsdhTu12Entity();
	virtual void getInstance();

    //////////////////////////////////////////////
	//J2 process
	//////////////////////////////////////////////
	virtual RetVal setJ2Mode(const J0Mode &mode);
	virtual RetVal setTxJ2Trace(App_dev::UCHAR *provJ2Msg);
	virtual RetVal getRxJ2Trace(App_dev::UCHAR *recvJ2Msg);
	virtual RetVal setExpJ2Trace(App_dev::UCHAR *expJ2Msg);
	virtual RetVal setJ2Comp(const ComparisonType &mode);
    virtual JoMode getJ2Mode(){return _j2_mode;}
    virtual RetVal getTxJ2Trace(App_dev::UCHAR *recvJ2Msg);
    virtual RetVal getExpJ2Trace(App_dev::UCHAR *recvJ2Msg);

protected:

private:
    JoMode      		_j2_mode;
    char       	        _tx_bytes[16];
    char       	        _exp_bytes[16];
	//define implementation
	std::shared_ptr<CsdhAbstrApi> _impl;
};

}//App_dev

#endif
