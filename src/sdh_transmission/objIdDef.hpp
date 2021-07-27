//////////////////////////////////////////////////////////////////
// objIdDef.hpp
//
// Date: 2021/02/22
// Author: yoxia
// Change history:
//      2021/02/22: This is the initialization version.
//
// Discription:
// Implematation for object type define 
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef OBJ_ID_DEF_HPP
#define OBJ_ID_DEF_HPP
namespace App_dev{
enum objIdType{
	OBJ_NULL = 1,
	OBJ_EQMID = 2,  //Identify rack/shelf/card object
	OBJ_DXCID = 3,    //Identify DXC object
	OBJ_MODULEID = 4, //Identify port object
	OBJ_SDHID = 5,    //Identify SDH boject
	OBJ_NODEFID = 6 
};

enum objEqmIdType{
	OBJ_EQMID_NULL = 0,
	OBJ_EQMID_RACK = 1,
	OBJ_EQMID_SHELF = 2,
	OBJ_EQMID_EC = 3,
	OBJ_EQMID_DXC = 4,
	OBJ_EQMID_FAN = 5,
	OBJ_EQMID_PWR = 6,
	//*****************CST3012 card definition start********************//
	//60G DXC capacity and include matrix, clock and controller
	OBJ_EQMID_S60MCC = 7, 
	//120G DXC capacity and include matrix, clock and controller
	OBJ_EQMID_S120MCC = 8, 
	//1XSTM64 IO card
	OBJ_EQMID_O1S64 = 9,
	//2XSTM64 IO card
	OBJ_EQMID_O2S64 = 10,
	//2XSTM16 IO card
	OBJ_EQMID_O2S16 = 11,
	//4XSTM16 IO card
	OBJ_EQMID_O4S16 = 12,
	//4XSTM4 IO card
	OBJ_EQMID_O4S4 = 13,
	//4XSTM1 IO card
	OBJ_EQMID_O4S1 = 14,
	//8XSTM1 IO card
	OBJ_EQMID_O8S1 = 15,
	//8XFE IO card
	OBJ_EQMID_O8FE = 16,
	//2XGE IO card
	OBJ_EQMID_O2GE = 17,
	//4XGE IO card
	OBJ_EQMID_O4GE = 18,
	//virtual 210XE1 IO card,this part is included into SxxMCC card
	OBJ_EQMID_M210E1 = 19,
	//42XE1 electric IO card
	OBJ_EQMID_E42E1 = 20,
	//8XE1 optical IO card
	OBJ_EQMID_O8E1 = 21,
	//memory card
	OBJ_EQMID_SBM3012 = 22,
	//Fan card S4FAN (FAN, 4U, S-SDH)
	OBJ_EQMID_S4FAN = 23,
	//Power	S20ADC(DC, S=SDH, 20A) 
	OBJ_EQMID_S20ADC= 24,
	//AUX SAUX12(12,3012,S=SDH)
	OBJ_EQMID_SAUX12= 25,
	//*****************CST3012 card definition end**********************//
	
	OBJ_EQMID_NODEF = 0xfe 
};


enum objSdhIdType{
	OBJ_SDHID_NULL = 0,
	OBJ_SDHID_STM1 = 1,
	OBJ_SDHID_STM4 = 2,
	OBJ_SDHID_STM16 = 3,
	OBJ_SDHID_STM64 = 4,
	OBJ_SDHID_RS = 5,
	OBJ_SDHID_MS = 6,
	OBJ_SDHID_AU4 = 7,
	OBJ_SDHID_TU3 = 8,
	OBJ_SDHID_TU12 = 9,
	OBJ_SDHID_VC4 = 10,
	OBJ_SDHID_VC3 = 11,
	OBJ_SDHID_VC12 = 12,
	OBJ_SDHID_NODEF = 13,
	OBJ_SDHID_PORT = 14,
};

enum objDxcIdType{
	OBJ_DXCID_VC12 = 0,
	OBJ_DXCID_VC3 = 1,
	OBJ_DXCID_VC4 = 2,
	OBJ_DXCID_ODU0 = 3,
	ODJ_DXCID_NODEF = 4
};
}

#endif
