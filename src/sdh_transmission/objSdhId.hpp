//////////////////////////////////////////////////////////////////
// objSdhId.hpp
//
// Date: 2021/02/24
// Author: yoxia
// Change history:
//      2021/02/24: This is the initialization version.
//
// Discription:
// Implematation class(CobjSdhId)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef OBJ_SDHID_HPP
#define OBJ_SDHID_HPP
#include "objModuId.hpp"
namespace App_dev{
/////////////////////////////////////////////////////////////////
const int OBJID_MAXSDHDATALEN = 5;
const int OBJID_MAXSDHLEN = OBJID_MAXMODULEN + OBJID_MAXSDHDATALEN;
/////////////////////////////////////////////////////////////////

class CobjSdhId : public CobjModuId
{
public:
	//define construction functions
	CobjSdhId();
	CobjSdhId(const CobjSdhId & obj);
	CobjSdhId(const App_dev::objEqmIdType &typeEqm,
			  const App_dev::objSdhIdType &typeSdh, const App_dev::UCHAR &rack, 
              const App_dev::UCHAR &shelf, const App_dev::UCHAR &slot,
			  const App_dev::UCHAR &port, App_dev::UCHAR vc4Num = 0xff,
			  App_dev::UCHAR tug3Num = 0xff, App_dev::UCHAR tug2Num = 0xff,
			  App_dev::UCHAR vc12Num = 0xff);

    //////////////////////////////////////////////////////////
    // extraction functions
	App_dev::objSdhIdType getSdhIdType() const;
    App_dev::UCHAR getVc4Num() const;
    App_dev::UCHAR getTug3Num() const;
    App_dev::UCHAR getTug2Num() const;
    App_dev::UCHAR getVc12Num() const;
    //void getData(App_dev::UCHAR *buffer);

    //bool operator>(CobjSdhId &temp) const;
    //bool operator<(CobjSdhId &temp) const;
    //bool operator==(CobjSdhId &temp) const;
    bool operator<(const CobjSdhId &temp) const; //注意这里的两个const

private:
    //Derived class need to overwite these two function
    virtual inline App_dev::objIdType getMyType() { return(OBJ_SDHID); }
    virtual inline App_dev::UCHAR getMyLength()  { return(OBJID_MAXSDHLEN); }
};

}
#endif
