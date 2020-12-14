#include <iostream>
#include "unique_ptr.h"
#include "shared_ptr.h"

int main() {
    shared_ptr *obj= new shared_ptr(10);
    shared_ptr *ptr=obj;
    shared_ptr *ptr1=obj;
    std::cout<<ptr1->getcout();

//
//    unique_ptr *ptr1= new unique_ptr(10);
//    unique_ptr *ptr2= new unique_ptr(20);
//    unique_ptr *p=ptr1->get();
//    std::cout << p<<"\t"<<p->getdata()<<std::endl;


    /** this will not work because now ownership of ptr is given to ptr1*/
    //std::cout <<ptr << "\t"<< ptr->getdata()<<std::endl;
    /**This will work*/
    //std::cout << ptr1<<"\t"<<ptr1->getdata()<<std::endl;

}