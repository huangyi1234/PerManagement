//////////////////////////////////////////////////////////////////
// sdhPortEntity.cpp
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
#include "sdhPortEntity.hpp"
#include "sdhDef.hpp"
#include "sdhApiFactory.hpp"
#include "assertDef.hpp"

namespace App_dev{

CsdhPortEntity::CsdhPortEntity() :
	CsdhEntityBase()
{
	_port_type = (UNKNOWN);
}

CsdhPortEntity::CsdhPortEntity(const CobjSdhId &myObj) :
	CsdhEntityBase(myObj)
{
	_port_type = (UNKNOWN);
}

CsdhPortEntity::~CsdhPortEntity()
{
}

//////////////////////////////////////////////
//JO process
//////////////////////////////////////////////
void CsdhPortEntity::getInstance()
{
	_impl = CsdhApiFactory::getInstance(_myObj);
}

RetVal CsdhPortEntity::setPortType(const Port_Transmission_Type &type)
{
	if(!_impl){
		std::cout << "impl is null" << std::endl;
		return RV_FAILURE;
	}

	_port_type = type;
	_impl->setPortType(_myObj, type);
	return RV_SUCCESS;
}

Port_Transmission_Type CsdhPortEntity::getPortType() const
{
	return _port_type;
}

std::map<unsigned int, ALARM_INFO> & CsdhPortEntity::get_alarm_map()
{
    if(!_impl)
    {
        std::cout << "impl is null" << std::endl;
    }
    else
    {
        _impl->get_alarm_check_map(_myObj, _alarm_map);
    }
 

    return _alarm_map;
}


}//App_dev

