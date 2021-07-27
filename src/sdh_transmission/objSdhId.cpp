//////////////////////////////////////////////////////////////////
// objSdhId.cpp
//
// Date: 2021/02/24
// Author: yoxia
// hange history:
//      2021/02/24: This is the initialization version.
//
// Discription:
// Implematation class(CobjSdhId)
//
//
// opyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#include "objSdhId.hpp"

namespace App_dev{
//clasee CobjSdhId
CobjSdhId::CobjSdhId()
{
	CobjModuId();
}

CobjSdhId::CobjSdhId(const CobjSdhId &oid)
	: CobjModuId(oid)
{
	_type = getMyType();
	_length = getMyLength();
}

CobjSdhId::CobjSdhId(const App_dev::objEqmIdType &typeEqm,
              const App_dev::objSdhIdType &typeSdh, const App_dev::UCHAR &rack,
              const App_dev::UCHAR &shelf, const App_dev::UCHAR &slot,
              const App_dev::UCHAR &port, App_dev::UCHAR vc4Num,
              App_dev::UCHAR tug3Num, App_dev::UCHAR tug2Num,
              App_dev::UCHAR vc12Num)
	: CobjModuId(typeEqm, rack, shelf, slot, port)
{
	_data[OBJID_MAXMODULEN - OBJID_HEADLEN] = typeSdh; 
	_data[OBJID_MAXMODULEN - OBJID_HEADLEN + 1] = vc4Num; 
	_data[OBJID_MAXMODULEN - OBJID_HEADLEN + 2] = tug3Num; 
	_data[OBJID_MAXMODULEN - OBJID_HEADLEN + 3] = tug2Num; 
	_data[OBJID_MAXMODULEN - OBJID_HEADLEN + 4] = vc12Num; 

	_type = getMyType();
	_length = getMyLength();
}


#if 0
bool CobjSdhId::operator>(CobjSdhId &temp) const
{
	int i = 0;

	for(i = 0; i < MAX_OBJID_LENGTH; i++)
	{

		if((_data[i]) > (temp._data[i]))
		{
			return true;
		}
		else if((_data[i]) == (temp._data[i]))
		{
			continue;
		}
		else
			return false;
	}
}
#endif
bool CobjSdhId::operator<(const CobjSdhId &temp) const
{
	int i = 0;

	for(i = 0; i < MAX_OBJID_LENGTH; i++)
	{

		if((_data[i]) < (temp._data[i]))
		{
			return true;
		}
		else if((_data[i]) == (temp._data[i]))
		{
			continue;
		}
		else
			return false;
	}

	return false;
}
#if 0
bool CobjSdhId::operator==(CobjSdhId &temp) const
{
	int i = 0;

	for(i = 0; i < MAX_OBJID_LENGTH; i++)
	{

		if((_data[i]) == (temp._data[i]))
		{
			continue;
		}
		else
			return false;
	};

	return true;
}
#endif

App_dev::objSdhIdType CobjSdhId::getSdhIdType() const
{
	return(App_dev::objSdhIdType(_data[OBJID_MAXMODULEN - OBJID_HEADLEN]));
}

App_dev::UCHAR CobjSdhId::getVc4Num() const
{
	return(_data[OBJID_MAXMODULEN - OBJID_HEADLEN + 1]);
}

App_dev::UCHAR CobjSdhId::getTug3Num() const
{
	return(_data[OBJID_MAXMODULEN - OBJID_HEADLEN + 2]);
}

App_dev::UCHAR CobjSdhId::getTug2Num() const
{
	return(_data[OBJID_MAXMODULEN - OBJID_HEADLEN + 3]);
}

App_dev::UCHAR CobjSdhId::getVc12Num() const
{
	return(_data[OBJID_MAXMODULEN - OBJID_HEADLEN + 4]);
}

}
