//
// Created by pc on 14/12/20.
//
#include "iostream"
#ifndef SMART_POINTERS_UNIQUE_PTR_H
#define SMART_POINTERS_UNIQUE_PTR_H


class unique_ptr {
    int data;
    unique_ptr *ptr;
public:
    explicit unique_ptr(int x){data=x; ptr=this->ptr;}
    int getdata(){ return data;}
    unique_ptr* release(unique_ptr *&obj);
    void reset();
    void swap(unique_ptr *ptr1);
    void move(unique_ptr *&ptr);
    unique_ptr* get();

    ~unique_ptr()
    {
        delete ptr;
    }
};


#endif //SMART_POINTERS_UNIQUE_PTR_H
