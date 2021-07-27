//////////////////////////////////////////////////////////////////
// sdhApiFactory.hpp 
//
// Date: 2021/03/29
// Author: yoxia
// Change history:
//      2021/03/29: This is the initialization version.
//
// Discription:
// Implematation class(CsdhApiFactory)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef SDH_API_FACTORY_HPP
#define SDH_API_FACTORY_HPP
#include<memory>
#include "sdhAbstractApi.hpp"
#include "objSdhId.hpp"

namespace App_dev{

class CsdhApiFactory
{
public:
	CsdhApiFactory();
	~CsdhApiFactory();	
	static std::shared_ptr<CsdhAbstrApi> getInstance(const CobjSdhId &obj);
};

}//App_dev

#endif
