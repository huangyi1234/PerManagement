//////////////////////////////////////////////////////////////////
// sdhAu4Entity.hpp
//
// Date: 2021/03/29
// Author: yoxia
// Change history:
//      2021/03/29: This is the initialization version.
//
// Discription:
// Implematation class(CsdhAu4Entity)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef SDH_AU4_ENTITY_HPP
#define SDH_AU4_ENTITY_HPP
#include<memory>
#include "sdhDef.hpp"
#include "typeDef.hpp"
#include "objSdhId.hpp"
#include "sdhEntityBase.hpp"
#include "sdhAbstractApi.hpp"

namespace App_dev{

class CsdhAu4Entity : public CsdhEntityBase
{
public:
	CsdhAu4Entity();
	CsdhAu4Entity(const CobjSdhId &myObj);
	virtual ~CsdhAu4Entity();
	virtual void getInstance();

    //J1 process
    virtual RetVal setJ1Mode(const J0Mode &mode);
	virtual RetVal setTxJ1Trace(App_dev::UCHAR *provJ1Msg);
	virtual RetVal getRxJ1Trace(App_dev::UCHAR *recvJ1Msg);
	virtual RetVal setExpJ1Trace(App_dev::UCHAR *expJ1Msg);
	virtual RetVal setJ1Comp(const ComparisonType &mode);
    virtual JoMode getJ1Mode(){return _j1_mode;}
    virtual RetVal getTxJ1Trace(App_dev::UCHAR *recvJ1Msg);
    virtual RetVal getExpJ1Trace(App_dev::UCHAR *recvJ1Msg);

    
    //C2 process
	virtual RetVal setTxC2Trace(App_dev::UCHAR &provC2Msg);
	virtual RetVal getRxC2Trace(App_dev::UCHAR &recvC2Msg);
	virtual RetVal setExpC2Trace(App_dev::UCHAR &expC2Msg);
	virtual RetVal setC2Comp(const ComparisonType &mode);
    virtual RetVal getTxC2Trace(App_dev::UCHAR &recvC2Msg);
    virtual RetVal getExpC2Trace(App_dev::UCHAR &recvC2Msg);

protected:

private:
    JoMode      		_j1_mode;
    char       	        _tx_bytes[16];
    char       	        _exp_bytes[16];
    char                _c2_tx;
    char                _c2_exp;
	//define implementation
	std::shared_ptr<CsdhAbstrApi> _impl;
};

}//App_dev

#endif
