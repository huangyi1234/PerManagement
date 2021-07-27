//////////////////////////////////////////////////////////////////
// sdhApiFactory.cpp 
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
#include "sdhApiFactory.hpp"
#include "sdhCardO1S64Api.hpp"

namespace App_dev{

CsdhApiFactory::CsdhApiFactory()
{
}

std::shared_ptr<CsdhAbstrApi> CsdhApiFactory::getInstance(const CobjSdhId &obj)
{
	App_dev::objEqmIdType eqmType = obj.getEqmIdType();

	/* For different card we need to difine the class */
	/* which is derived from CsdhAbstrApi             */
	if(eqmType == OBJ_EQMID_O1S64){
		return(CsdhCardO1S64Api::getInstance());
	}else{
		return(NULL);
	}
}

CsdhApiFactory::~CsdhApiFactory()	
{
}

}//App_dev

