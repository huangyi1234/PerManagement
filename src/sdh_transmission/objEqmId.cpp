//////////////////////////////////////////////////////////////////
// objEqmId.cpp
//
// Date: 2021/02/24
// Author: yoxia
// hange history:
//      2021/02/24: This is the initialization version.
//
// Discription:
// Implematation class(CobjEqmId)
//
//
// opyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#include "objEqmId.hpp"

namespace App_dev{
//clasee CobjEqmId
CobjEqmId::CobjEqmId()
{
	CobjId();
}

CobjEqmId::CobjEqmId(const CobjEqmId &oid)
	: CobjId(oid)
{
	_type = getMyType();
	_length = getMyLength();
}

CobjEqmId::CobjEqmId(const App_dev::objEqmIdType &type, 
					 const App_dev::UCHAR &rack, 
                     const App_dev::UCHAR &shelf, const App_dev::UCHAR &slot)
{
	_data[0] = (App_dev::UCHAR)type;
	_data[1] = (App_dev::UCHAR)rack;
	_data[2] = (App_dev::UCHAR)shelf;
	_data[3] = (App_dev::UCHAR)slot;

	_type = getMyType();
	_length = getMyLength();
}

App_dev::objEqmIdType CobjEqmId::getEqmIdType() const
{
	return((App_dev::objEqmIdType)(_data[0]));
}

App_dev::UCHAR CobjEqmId::getRack() const
{
	return(_data[1]);
}

App_dev::UCHAR CobjEqmId::getShelf() const
{
	return(_data[2]);
}

App_dev::UCHAR CobjEqmId::getSlot() const
{
	return(_data[3]);
}

}
