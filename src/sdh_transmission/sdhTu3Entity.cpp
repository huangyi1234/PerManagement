//////////////////////////////////////////////////////////////////
// sdhTu3Entity.cpp
//
// Date: 2021/03/29
// Author: yoxia
// Change history:
//      2021/03/29: This is the initialization version.
//
// Discription:
// Implematation class(CsdhTu3Entity)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#include<string.h>
#include "sdhTu3Entity.hpp"
#include "sdhDef.hpp"
#include "sdhApiFactory.hpp"
#include "assertDef.hpp"

namespace App_dev{

CsdhTu3Entity::CsdhTu3Entity() :
	CsdhEntityBase()
{
}

CsdhTu3Entity::CsdhTu3Entity(const CobjSdhId &myObj) :
	CsdhEntityBase(myObj)
{
    _j1_mode = J0_16BYTE;
    memset(_tx_bytes, 0, sizeof(_tx_bytes));
    memset(_exp_bytes, 0, sizeof(_exp_bytes));

    sprintf(_tx_bytes, "HD SDH VC3");
    sprintf(_exp_bytes, "HD SDH VC3");

    _c2_exp = 0xff;
    _c2_tx = 0xff;

}

CsdhTu3Entity::~CsdhTu3Entity()
{
}

void CsdhTu3Entity::getInstance()
{
	_impl = CsdhApiFactory::getInstance(_myObj);
}

RetVal CsdhTu3Entity::setJ1Mode(const J0Mode &mode)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::setTxJ1Trace(App_dev::UCHAR *provJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::getRxJ1Trace(App_dev::UCHAR *recvJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::setExpJ1Trace(App_dev::UCHAR *expJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::setJ1Comp(const ComparisonType &mode) 
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::getTxJ1Trace(App_dev::UCHAR *recvJ1Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::getExpJ1Trace(App_dev::UCHAR *recvJ1Msg)
{
    return RV_SUCCESS;
}

//C2  
RetVal CsdhTu3Entity::setTxC2Trace(App_dev::UCHAR &provC2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::getRxC2Trace(App_dev::UCHAR &recvC2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::setExpC2Trace(App_dev::UCHAR &expC2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::setC2Comp(const ComparisonType &mode)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::getTxC2Trace(App_dev::UCHAR &recvC2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu3Entity::getExpC2Trace(App_dev::UCHAR &recvC2Msg)
{
    return RV_SUCCESS;
}


}//App_dev

