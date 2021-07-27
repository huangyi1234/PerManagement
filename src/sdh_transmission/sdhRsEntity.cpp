//////////////////////////////////////////////////////////////////
// sdhRsEntity.cpp
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
#include<string.h>
#include "sdhRsEntity.hpp"
#include "sdhDef.hpp"
#include "sdhApiFactory.hpp"
#include "assertDef.hpp"

namespace App_dev{

CsdhRsEntity::CsdhRsEntity() :
	CsdhEntityBase()
{
    _j0_mode = J0_16BYTE;
    memset(_tx_bytes, 0, sizeof(_tx_bytes));
    memset(_exp_bytes, 0, sizeof(_exp_bytes));

    sprintf(_tx_bytes, "HD SDH Device");
    sprintf(_exp_bytes, "HD SDH Device");
}

CsdhRsEntity::CsdhRsEntity(const CobjSdhId &myObj) :
	CsdhEntityBase(myObj)
{
    _j0_mode = J0_16BYTE;
    memset(_tx_bytes, 0, sizeof(_tx_bytes));
    memset(_exp_bytes, 0, sizeof(_exp_bytes));

    sprintf(_tx_bytes, "HD SDH Device");
    sprintf(_exp_bytes, "HD SDH Device");
}

CsdhRsEntity::~CsdhRsEntity()
{
}

//////////////////////////////////////////////
//JO process
//////////////////////////////////////////////
void CsdhRsEntity::getInstance()
{
	_impl = CsdhApiFactory::getInstance(_myObj);

}

RetVal CsdhRsEntity::setJ0Mode(const J0Mode &mode)
{
    _j0_mode = mode;
	return RV_SUCCESS;
}

RetVal CsdhRsEntity::setTxJ0Trace(App_dev::UCHAR *provJ0Msg)
{
    memset(_tx_bytes, 0, sizeof(_tx_bytes));
    memcpy(_tx_bytes, provJ0Msg, sizeof(_tx_bytes));
	return RV_SUCCESS;
}

RetVal CsdhRsEntity::getRxJ0Trace(App_dev::UCHAR *recvJ0Msg)
{
    sprintf((char *)recvJ0Msg, "huidian xxx");
	return RV_SUCCESS;
}

RetVal CsdhRsEntity::setExpJ0Trace(App_dev::UCHAR *expJ0Msg)
{
    memset(_exp_bytes, 0, sizeof(_exp_bytes));
    memcpy(_exp_bytes, expJ0Msg, sizeof(_exp_bytes));
	return RV_SUCCESS;
}

RetVal CsdhRsEntity::setJ0Comp(const ComparisonType &mode) 
{
	return RV_SUCCESS;
}

RetVal CsdhRsEntity::getTxJ0Trace(App_dev::UCHAR *recvJ0Msg)
{
    memcpy(recvJ0Msg, _tx_bytes, sizeof(_tx_bytes));
    return RV_SUCCESS;
}

RetVal CsdhRsEntity::getExpJ0Trace(App_dev::UCHAR *recvJ0Msg)
{
    memcpy(recvJ0Msg, _exp_bytes, sizeof(_exp_bytes));
    return RV_SUCCESS;
}


}//App_dev

