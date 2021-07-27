//////////////////////////////////////////////////////////////////
// objModuId.hpp
//
// Date: 2021/02/24
// Author: yoxia
// Change history:
//      2021/02/24: This is the initialization version.
//
// Discription:
// Implematation class(CobjModuId)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef OBJ_MODUID_HPP
#define OBJ_MODUID_HPP
#include "objEqmId.hpp"
namespace App_dev{
/////////////////////////////////////////////////////////////////
const int OBJID_MAXMODUDATALEN = 1;
const int OBJID_MAXMODULEN = OBJID_MAXEQMLEN + OBJID_MAXMODUDATALEN;
/////////////////////////////////////////////////////////////////

class CobjModuId : public CobjEqmId
{
public:
	//define construction functions
	CobjModuId();
	CobjModuId(const CobjModuId & obj);
	CobjModuId(const App_dev::objEqmIdType &type, const App_dev::UCHAR &rack, 
              const App_dev::UCHAR &shelf, const App_dev::UCHAR &slot,
			  const App_dev::UCHAR &port);

	virtual ~CobjModuId(){};
    //////////////////////////////////////////////////////////
    // extraction functions
    App_dev::UCHAR getPort() const;
    //void getData(App_dev::UCHAR *buffer);

private:
    //Derived class need to overwite these two function
    virtual inline App_dev::objIdType getMyType() { return(OBJ_MODULEID); }
    virtual inline App_dev::UCHAR getMyLength()  { return(OBJID_MAXMODULEN); }
};

}
#endif
