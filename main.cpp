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

/** Unique_pointers */

    unique_ptr  obj(10);
    unique_ptr obj1(100);

    //obj=obj1; //not allowed
    ///parameterized constructor call
    unique_ptr *px=new unique_ptr(1000);

    ///copy constructor called
    unique_ptr* ptr1=&obj;


    std::cout << "The value should be : " << px->getdata() << std::endl;
    std::cout << "The value should be : " << ptr1->getdata() << std::endl;

    unique_ptr* ptr2=&obj1;

    ptr2->move(px);

    std::cout << "The value should be : " << ptr2->getdata() << std::endl;


    /** Shared_pointers*/

   /* shared_ptr<A> a = make_A();
    shared_ptr<A> b = a;
    shared_ptr<A> c = a;

    cout << a.use_count() << " " << b.use_count() << " " << c.use_count() << endl;

    c.reset();
    cout << a.use_count() << " " << b.use_count() << " " << c.use_count() << endl;
    cout << a->i << endl;*/
}
