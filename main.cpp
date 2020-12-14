#include <iostream>
#include <memory>
#include "unique_ptr.h"
#include "thread"

int main() {
    unique_ptr *ptr1= new unique_ptr(10);
    unique_ptr *ptr2= new unique_ptr(20);
//    unique_ptr *ptr = ptr2->release(ptr2);
    ptr1->swap(ptr2);
    ptr1->reset(ptr2);
//    ptr1->move(ptr);
    std::cout << ptr1<<"\t"<<ptr1->getdata()<<std::endl;
    unique_ptr *p=ptr1->get();
    std::cout << p<<"\t"<<p->getdata()<<std::endl;


    /** this will not work because now ownership of ptr is given to ptr1*/
    //std::cout <<ptr << "\t"<< ptr->getdata()<<std::endl;
    /**This will work*/
    //std::cout << ptr1<<"\t"<<ptr1->getdata()<<std::endl;

}