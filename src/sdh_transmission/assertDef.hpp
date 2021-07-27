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
#ifndef ASSERT_DEF_HPP
#define ASSERT_DEF_HPP
#define ASSERT_PTR(pointer, retVal) \
    if(pointer == NULL){            \
        std::cout << "This is a NULL pointer" << std::endl; \
        return(retVal); \
    }else{  \
        (void) 0;   \
    }


#endif
