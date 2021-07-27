//////////////////////////////////////////////////////////////////
// sdhMsEntity.hpp
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
#ifndef SDH_MS_ENTITY_HPP
#define SDH_MS_ENTITY_HPP
#include<memory>
#include "sdhDef.hpp"
#include "typeDef.hpp"
#include "objSdhId.hpp"
#include "sdhEntityBase.hpp"
#include "sdhAbstractApi.hpp"

namespace App_dev{

class CsdhMsEntity : public CsdhEntityBase
{
public:
	CsdhMsEntity();
	CsdhMsEntity(const CobjSdhId &myObj);
	virtual ~CsdhMsEntity();
	virtual void getInstance();


    virtual RetVal set_s1_value(const App_dev::UCHAR &value);
    virtual App_dev::UCHAR get_S1_value(){return _S1_value;}

protected:

private:
    App_dev::UCHAR   _S1_value;
	//define implementation
	std::shared_ptr<CsdhAbstrApi> _impl;
};

}//App_dev

#endif
