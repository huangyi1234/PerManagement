//////////////////////////////////////////////////////////////////
// sdhAu4Entity.cpp
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
#include<string.h>
#include "sdhAu4Entity.hpp"
#include "sdhDef.hpp"
#include "sdhApiFactory.hpp"
#include "assertDef.hpp"

namespace App_dev{

CsdhAu4Entity::CsdhAu4Entity() :
	CsdhEntityBase()
{
    _j1_mode = J0_16BYTE;
    memset(_tx_bytes, 0, sizeof(_tx_bytes));
    memset(_exp_bytes, 0, sizeof(_exp_bytes));

    sprintf(_tx_bytes, "HD SDH VC4");
    sprintf(_exp_bytes, "HD SDH VC4");

    _c2_exp = 0xff;
    _c2_tx = 0xff;

}

CsdhAu4Entity::CsdhAu4Entity(const CobjSdhId &myObj) :
	CsdhEntityBase(myObj)
{
    _j1_mode = J0_16BYTE;
    memset(_tx_bytes, 0, sizeof(_tx_bytes));
    memset(_exp_bytes, 0, sizeof(_exp_bytes));

    sprintf(_tx_bytes, "HD SDH VC4");
    sprintf(_exp_bytes, "HD SDH VC4");

    _c2_exp = 0xff;
    _c2_tx = 0xff;

}

CsdhAu4Entity::~CsdhAu4Entity()
{
}

void CsdhAu4Entity::getInstance()
{
	_impl = CsdhApiFactory::getInstance(_myObj);
}

RetVal CsdhAu4Entity::setJ1Mode(const J0Mode &mode)
{
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::setTxJ1Trace(App_dev::UCHAR *provJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::getRxJ1Trace(App_dev::UCHAR *recvJ1Msg)
{
    sprintf((char *)recvJ1Msg, "huidian xxx");
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::setExpJ1Trace(App_dev::UCHAR *expJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::setJ1Comp(const ComparisonType &mode) 
{
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::getTxJ1Trace(App_dev::UCHAR *recvJ1Msg)
{
    memcpy(recvJ1Msg, _tx_bytes, sizeof(_tx_bytes));
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::getExpJ1Trace(App_dev::UCHAR *recvJ1Msg)
{
    memcpy(recvJ1Msg, _exp_bytes, sizeof(_exp_bytes));
    return RV_SUCCESS;
}


//C2
RetVal CsdhAu4Entity::setTxC2Trace(App_dev::UCHAR &provC2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::getRxC2Trace(App_dev::UCHAR &recvC2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::setExpC2Trace(App_dev::UCHAR &expC2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::setC2Comp(const ComparisonType &mode)
{
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::getTxC2Trace(App_dev::UCHAR &recvC2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhAu4Entity::getExpC2Trace(App_dev::UCHAR &recvC2Msg)
{
    return RV_SUCCESS;
}

}//App_dev

