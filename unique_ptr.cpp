//
// Created by pc on 14/12/20.
//

#include "unique_ptr.h"

/**releases pointer after assigning control to another pointer*/
unique_ptr *unique_ptr::release(unique_ptr *&obj) {
    unique_ptr *temp;
    temp=obj;
    obj=0;

    return temp;
}

/** reset the pointer */
unique_ptr *unique_ptr::reset(unique_ptr *obj) {
    this->ptr=obj;
    return obj;
}

/** swap the pointers */
void unique_ptr::swap(unique_ptr *ptr1) {
    unique_ptr temp= *this;
    *this=*ptr1;
    *ptr1=temp;
}

/**move the pointer from one to another*/
void unique_ptr::move(unique_ptr *&ptrobj) {
    unique_ptr *temp;
    *this=*ptrobj;
    ptrobj=0;

    return;
}

/**returns the reference of pointer*/
unique_ptr *unique_ptr::get() {
    return this;
}
