//////////////////////////////////////////////////////////////////
// objId.hpp
//
// Date: 2021/02/22
// Author: yoxia
// Change history:
//      2021/02/22: This is the initialization version.
//
// Discription:
// Implematation class(CobjId)
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef OBJID_HPP
#define OBJID_HPP
#include "objIdDef.hpp"
#include "typeDef.hpp"
namespace App_dev{

const int MAX_OBJID_LENGTH = 25;
const int OBJID_HEADLEN = 2; //_lengh + _type
const int OBJID_LEN_OFFSET = 0;
const int OBJID_TYPE_OFFSET = 1;

class CobjId
{
public:
	CobjId();
	CobjId(const CobjId &oid);
	virtual ~CobjId();
	

	//////////////////////////////////////////////////////////
	// compare operators(return 0 if == else return -1 if !=)
	bool operator==(const CobjId &oid) const;

	//////////////////////////////////////////////////////////
	// overwrite operators
	virtual CobjId& operator=(const CobjId &oid);

	//////////////////////////////////////////////////////////
	// extraction functions
	App_dev::UCHAR getLength() const;
	App_dev::objIdType getType() const;
	//void getData(App_dev::UCHAR *buffer);
	
protected:
	App_dev::UCHAR _length;
	App_dev::objIdType _type : 8;
	App_dev::UCHAR _data[MAX_OBJID_LENGTH];

private:
	//Derived class need to overwite these two function
	virtual inline App_dev::objIdType getMyType() { return(OBJ_NULL); }
	virtual inline App_dev::UCHAR getMyLength()  { return(OBJID_HEADLEN); }
};

}

#endif
