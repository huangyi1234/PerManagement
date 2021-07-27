//////////////////////////////////////////////////////////////////
// objModuId.cpp
//
// Date: 2021/02/24
// Author: yoxia
// hange history:
//      2021/02/24: This is the initialization version.
//
// Discription:
// Implematation class(CobjModuId)
//
//
// opyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#include "objModuId.hpp"

namespace App_dev{
//clasee CobjModuId
CobjModuId::CobjModuId()
{
	CobjEqmId();
}

CobjModuId::CobjModuId(const CobjModuId &oid)
	: CobjEqmId(oid)
{
	_type = getMyType();
	_length = getMyLength();
}

CobjModuId::CobjModuId(const App_dev::objEqmIdType &type, 
					 const App_dev::UCHAR &rack, 
                     const App_dev::UCHAR &shelf, const App_dev::UCHAR &slot,
					 const App_dev::UCHAR &port)
	: CobjEqmId(type, rack, shelf, slot)
{
	_data[OBJID_MAXEQMDATALEN] = port; 

	_type = getMyType();
	_length = getMyLength();
}

App_dev::UCHAR CobjModuId::getPort() const
{
	return(_data[OBJID_MAXEQMDATALEN]);
}

}
