//////////////////////////////////////////////////////////////////
// sdhPortEntity.hpp
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
#ifndef SDH_PORT_ENTITY_HPP
#define SDH_PORT_ENTITY_HPP
#include<memory>
#include "sdhDef.hpp"
#include "typeDef.hpp"
#include "objSdhId.hpp"
#include "sdhEntityBase.hpp"
#include "sdhAbstractApi.hpp"


namespace App_dev{

class CsdhPortEntity : public CsdhEntityBase
{
public:
	CsdhPortEntity();
	CsdhPortEntity(const CobjSdhId &myObj);
	virtual ~CsdhPortEntity();
	virtual void getInstance();
	//////////////////////////////////////////////
	//Port type process
	//////////////////////////////////////////////
	RetVal setPortType(const Port_Transmission_Type &type);
	Port_Transmission_Type getPortType() const;
    std::map<unsigned int, ALARM_INFO> &get_alarm_map();
protected:

private:
	Port_Transmission_Type	_port_type;
	//define implementation
	std::shared_ptr<CsdhAbstrApi> _impl;
};

}//App_dev

#endif
