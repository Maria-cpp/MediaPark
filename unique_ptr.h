//
// Created by pc on 14/12/20.
//
#include "iostream"
#ifndef SMART_POINTERS_UNIQUE_PTR_H
#define SMART_POINTERS_UNIQUE_PTR_H
#include <cassert>
#define SHARED_ASSERT(x)    assert(x)


class unique_ptr {

public:
    /// @brief Default constructor
    explicit unique_ptr(){data=0; ptr=0;};

    /// @brief Constructor with the provided pointer to manage
    explicit unique_ptr(unique_ptr& p) ;

    /// @brief Copy constructor (used by the copy-and-swap idiom)
    unique_ptr(const unique_ptr& p) throw();

    /// @brief the destructor releases its ownership and destroy the object
    unique_ptr& operator=(unique_ptr &p) throw();

    /// @brief this reset releases its ownership and destroy the object
    void reset() throw();

    /// @brief this reset release its ownership and re-acquire another one
    void reset(unique_ptr* p) throw();

    /// @brief release the ownership of the ptr pointer without destroying the object!
    void release() throw();


    /// underlying pointer operations :
    inline unique_ptr& operator*()  const throw();

    inline unique_ptr* operator->() const throw();

    unique_ptr* get() const throw();

    explicit unique_ptr(int x){data=x;}

    int getdata(){ return data;}

    void swap(unique_ptr *ptr1)throw();

    unique_ptr* move(unique_ptr *&ptrobj);

    inline ~unique_ptr() throw(){destroy();}
private:

    int data;
    unique_ptr *ptr;

    inline void destroy() throw() // never throws
    {
        data=0;
        delete ptr;
        ptr = NULL;
    }

    inline void destroy(unique_ptr* p) throw() // never throws
    {
        delete p;
    }

};


#endif //SMART_POINTERS_UNIQUE_PTR_H
