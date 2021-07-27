//////////////////////////////////////////////////////////////////
// objId.cpp
//
// Date: 2021/02/23
// Author: yoxia
// hange history:
//      2021/02/23: This is the initialization version.
//
// Discription:
// Implematation class(objId)
//
//
// opyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#include "objId.hpp"

namespace App_dev{

//class CobjId
CobjId::CobjId()
{
	_length = getMyLength();
	_type = getMyType();
	for(auto &elem : _data){
		elem = '\0';
	}
}

CobjId::CobjId(const CobjId &oid)
{
	if(getMyType() != OBJ_NULL){
		if(oid.getLength() < getMyLength()){
			// force OBJ_NULL
			_type = OBJ_NULL;
			_length = OBJID_HEADLEN;
		}else{
			_type = getMyType();
            _length = getMyLength();
		}
    }
    else{
        _type = oid.getType();
        _length = static_cast <App_dev::UCHAR> (oid.getLength());
    }

    for(int i=0; i<MAX_OBJID_LENGTH; i++){
       _data[i] = oid._data[i];
    }
}

CobjId::~CobjId()
{
}

CobjId &CobjId::operator=(const CobjId &oid)
{
    if(oid.getLength() < getMyLength()){
        _type = OBJ_NULL;
        _length = OBJID_HEADLEN;
        return(*this);
    }

    _type = getMyType();
    _length = getMyLength();

    for(int i=0; i<MAX_OBJID_LENGTH; i++){
       _data[i] = oid._data[i];
    }
    return(*this);
}


bool CobjId::operator==(const CobjId &oid) const
{
    if(_type != oid._type){
		return(false);
    }

    if(_length != oid._length){
		return(true);
    }

    int i;
    const App_dev::UCHAR *sp, *dp;
    for(dp=_data, sp=oid._data, i=0; i<(_length-OBJID_HEADLEN); i++){
		if(*(dp+i) != *(sp+i)){
			return(false);
		}
    }
    return(true);
}

App_dev::UCHAR CobjId::getLength() const
{
	return(_length);
}

App_dev::objIdType CobjId::getType() const
{
	return(_type);
}


}//namespace App_dev
