//////////////////////////////////////////////////////////////////
// sdhRsEntity.hpp
//
// Date: 2021/03/29
// Author: yoxia
// Change history:
//      2021/03/29: This is the initialization version.
//
// Discription:
// Implematation class(CsdhRsEntity)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef SDH_RS_ENTITY_HPP
#define SDH_RS_ENTITY_HPP
#include<memory>
#include "sdhDef.hpp"
#include "typeDef.hpp"
#include "objSdhId.hpp"
#include "sdhEntityBase.hpp"
#include "sdhAbstractApi.hpp"

namespace App_dev{

class CsdhRsEntity : public CsdhEntityBase
{
public:
	CsdhRsEntity();
	CsdhRsEntity(const CobjSdhId &myObj);
	virtual ~CsdhRsEntity();
	virtual void getInstance();
	//////////////////////////////////////////////
	//JO process
	//////////////////////////////////////////////
	virtual RetVal setJ0Mode(const J0Mode &mode);
	virtual RetVal setTxJ0Trace(App_dev::UCHAR *provJ0Msg);
	virtual RetVal getRxJ0Trace(App_dev::UCHAR *recvJ0Msg);
	virtual RetVal setExpJ0Trace(App_dev::UCHAR *expJ0Msg);
	virtual RetVal setJ0Comp(const ComparisonType &mode);
    virtual JoMode getJ0Mode(){return _j0_mode;}
    virtual RetVal getTxJ0Trace(App_dev::UCHAR *recvJ0Msg);
    virtual RetVal getExpJ0Trace(App_dev::UCHAR *recvJ0Msg);
    
protected:

private:
    JoMode      		_j0_mode;
    char       	        _tx_bytes[16];
    char       	        _exp_bytes[16];
	//define implementation
	std::shared_ptr<CsdhAbstrApi> _impl;
};

}//
#endif
