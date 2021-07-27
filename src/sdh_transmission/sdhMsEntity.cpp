//////////////////////////////////////////////////////////////////
// sdhMsEntity.cpp
//
// Date: 2021/03/29
// Author: yoxia
// Change history:
//      2021/03/29: This is the initialization version.
//
// Discription:
// Implematation class(CsdhMsEntity)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#include<string.h>
#include "sdhMsEntity.hpp"
#include "sdhDef.hpp"
#include "sdhApiFactory.hpp"
#include "assertDef.hpp"

namespace App_dev{

CsdhMsEntity::CsdhMsEntity() :
	CsdhEntityBase()
{
    _S1_value = 0xff;
}

CsdhMsEntity::CsdhMsEntity(const CobjSdhId &myObj) :
	CsdhEntityBase(myObj)
{
    _S1_value = 0xff;
}

CsdhMsEntity::~CsdhMsEntity()
{
}

void CsdhMsEntity::getInstance()
{
	_impl = CsdhApiFactory::getInstance(_myObj);
}

RetVal CsdhMsEntity::set_s1_value(const App_dev::UCHAR &value)
{
    if(!_impl){
		std::cout << "impl is null" << std::endl;
		return RV_FAILURE;
	}

	_S1_value = value;
	_impl->set_S1_value(_myObj, value);

    return RV_SUCCESS;
}

}//App_dev

