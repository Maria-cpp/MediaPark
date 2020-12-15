#include <iostream>
#include "unique_ptr.h"
#include "shared_ptr.h"

using namespace std;

class A {
public:
    int i;
    explicit A(int _i) : i(_i) {
    }
    ~A() {
        cout << "deleting A" << endl;
    }
};

shared_ptr<A> make_A() {
    shared_ptr<A> a(new A(5));
    return a;
}

int main() {
    /** Shared_pointers*/

    shared_ptr<A> a = make_A();
    shared_ptr<A> b = a;
    shared_ptr<A> c = a;

    cout << a.use_count() << " " << b.use_count() << " " << c.use_count() << endl;

    c.reset();

    cout << a.use_count() << " " << b.use_count() << " " << c.use_count() << endl;
    cout << a->i << endl;



/** Unique_pointers */

    ///Default constructor
    unique_ptr  obj;

    ///parameterized constructor call
    unique_ptr *px=new unique_ptr(10);

    ///copy constructor called
    unique_ptr* ptr1=new unique_ptr(*px);


    std::cout << "The value should be : " << px->getdata() << std::endl;
    std::cout << "The value should be : " << ptr1->getdata() << std::endl;

    unique_ptr* ptr2=new unique_ptr(20);
    unique_ptr* ptr3;
    ptr3=ptr3->move(ptr2);

    std::cout << "The value should be : " << ptr3->getdata() << std::endl;

    //This will not print
    //std::cout << "The value should be : " << ptr2->getdata() << std::endl;
    ptr2= new unique_ptr(100);
    if(ptr1!=ptr2)
        std::cout<<"Not equal"<<std::endl;
    else
        std::cout<<"equal"<<std::endl;

    if(ptr1>ptr2)
        std::cout<<"ptr1 is greater"<<std::endl;
    else
        std::cout<<"ptr2 is greater"<<std::endl;

    if(ptr2>=ptr3)
        std::cout<<"ptr2 is greater than equals to ptr3"<<std::endl;
    else
        std::cout<<"ptr2 is greater than equals to ptr3"<<std::endl;

    ptr1->reset(ptr3);
    std::cout << "The value should be : " << ptr1->getdata() << std::endl;

    ptr2->release();
    std::cout << "The value should be : " << ptr3->getdata() << std::endl;
    std::cout << "The value should be : " << ptr2->getdata() << std::endl;

}
