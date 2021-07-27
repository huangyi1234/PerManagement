//////////////////////////////////////////////////////////////////
// objEqmId.hpp
//
// Date: 2021/02/24
// Author: yoxia
// Change history:
//      2021/02/24: This is the initialization version.
//
// Discription:
// Implematation class(CobjEqmId)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef OBJ_EQMID_HPP
#define OBJ_EQMID_HPP
#include "objId.hpp"
namespace App_dev{
/////////////////////////////////////////////////////////////////
const int OBJID_MAXEQMDATALEN = 4;
const int OBJID_MAXEQMLEN = OBJID_HEADLEN + OBJID_MAXEQMDATALEN;
/////////////////////////////////////////////////////////////////

class CobjEqmId : public CobjId
{
public:
	//define construction functions
	CobjEqmId();
	CobjEqmId(const CobjEqmId & obj);
	CobjEqmId(const App_dev::objEqmIdType &type, const App_dev::UCHAR &rack, 
              const App_dev::UCHAR &shelf, const App_dev::UCHAR &slot);

	virtual ~CobjEqmId(){};
    //////////////////////////////////////////////////////////
    // extraction functions
    App_dev::objEqmIdType getEqmIdType() const;
    App_dev::UCHAR getRack() const;
    App_dev::UCHAR getShelf() const;
    App_dev::UCHAR getSlot() const;
    //void getData(App_dev::UCHAR *buffer);

private:
    //Derived class need to overwite these two function
    virtual App_dev::objIdType getMyType() { return(OBJ_EQMID); }
    virtual App_dev::UCHAR getMyLength()  { return(OBJID_MAXEQMLEN); }
};

}

#endif
