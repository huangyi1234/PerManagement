//////////////////////////////////////////////////////////////////
// sdhTu12Entity.cpp
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
#include<string.h>
#include "sdhTu12Entity.hpp"
#include "sdhDef.hpp"
#include "sdhApiFactory.hpp"
#include "assertDef.hpp"

namespace App_dev{

CsdhTu12Entity::CsdhTu12Entity() :
	CsdhEntityBase()
{
}

CsdhTu12Entity::CsdhTu12Entity(const CobjSdhId &myObj) :
	CsdhEntityBase(myObj)
{
    _j2_mode = J0_16BYTE;
    memset(_tx_bytes, 0, sizeof(_tx_bytes));
    memset(_exp_bytes, 0, sizeof(_exp_bytes));

    sprintf(_tx_bytes, "HD SDH VC12");
    sprintf(_exp_bytes, "HD SDH VC12");

}

CsdhTu12Entity::~CsdhTu12Entity()
{
}

void CsdhTu12Entity::getInstance()
{
	_impl = CsdhApiFactory::getInstance(_myObj);
}

RetVal CsdhTu12Entity::setJ2Mode(const J0Mode &mode)
{
    return RV_SUCCESS;
}

RetVal CsdhTu12Entity::setTxJ2Trace(App_dev::UCHAR *provJ2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu12Entity::getRxJ2Trace(App_dev::UCHAR *recvJ2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu12Entity::setExpJ2Trace(App_dev::UCHAR *expJ2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu12Entity::setJ2Comp(const ComparisonType &mode) 
{
    return RV_SUCCESS;
}

RetVal CsdhTu12Entity::getTxJ2Trace(App_dev::UCHAR *recvJ2Msg)
{
    return RV_SUCCESS;
}

RetVal CsdhTu12Entity::getExpJ2Trace(App_dev::UCHAR *recvJ2Msg)
{
    return RV_SUCCESS;
}


}//App_dev

