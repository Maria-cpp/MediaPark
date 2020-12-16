//
// Created by pc on 14/12/20.
//

#include "unique_ptr.h"

unique_ptr::unique_ptr(const unique_ptr& p) throw() :ptr(p.ptr)// never throws
{
    const_cast<unique_ptr&>(p).ptr = NULL; // const-cast to force ownership transfer!
}

unique_ptr::unique_ptr(unique_ptr &p) {
    this->ptr=p.ptr;
    data=p.data;
    destroy(&p);
}

/// @brief the destructor releases its ownership and destroy the object


/// @brief this reset releases its ownership and destroy the object
void unique_ptr::reset() throw()
{
    unique_ptr::destroy();
}
/// @brief this reset release its ownership and re-acquire another one
void unique_ptr::reset(unique_ptr* p) throw()
{
    SHARED_ASSERT((NULL == p) || (ptr != p) || (data==0));
    data=p->data;
    destroy(p);
}


/// @brief release the ownership of the px pointer without destroying the object!
void unique_ptr::release() throw()
{
    this->data=0;
    this->ptr = NULL;
}

// underlying pointer operations :
unique_ptr& unique_ptr::operator*()  const throw()
{
    SHARED_ASSERT(NULL != ptr);
    return *ptr;
}


unique_ptr* unique_ptr::operator->() const throw()
{
    return ptr;
}
unique_ptr* unique_ptr::get()  const throw()
{
    return ptr;
}

/** swap the pointers */
void unique_ptr::swap(unique_ptr *ptr1) throw() {
    std::swap(unique_ptr::ptr, ptr1);
}


void unique_ptr::move(unique_ptr *&ptrobj) {
    int i=ptrobj->data;
    this->data=i;
    unique_ptr* ptr1=ptrobj->ptr;
    this->ptr=ptr1;
    ptrobj->release();

}

/// comparaison operators
bool operator==(const unique_ptr& l, const unique_ptr& r)
{
    return (l.get() == r.get());
}

bool operator!=(const unique_ptr& l, const unique_ptr& r)
{
    return (l.get() != r.get());
}

bool operator<=(const unique_ptr& l, const unique_ptr& r)
{
    return (l.get() <= r.get());
}

bool operator<(const unique_ptr& l, const unique_ptr& r)
{
    return (l.get() < r.get());
}
bool operator>=(const unique_ptr& l, const unique_ptr& r)
{
    return (l.get() >= r.get());
}

bool operator>(const unique_ptr& l, const unique_ptr& r)
{
    return (l.get() > r.get());
}