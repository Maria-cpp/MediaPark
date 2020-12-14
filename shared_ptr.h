//
// Created by pc on 14/12/20.
//

#ifndef SMART_POINTERS_SHARED_PTR_H
#define SMART_POINTERS_SHARED_PTR_H


class shared_ptr {

    int data;
    shared_ptr *ptr;
protected: int count=0;
public:
    explicit shared_ptr(int x){data=x; count++; }
    int getdata(){ return data;}
    shared_ptr* release(shared_ptr *&obj);
    void reset(shared_ptr *obj);
    void swap(shared_ptr *ptr1);
    void move(shared_ptr *&ptr);
    shared_ptr* get();
    int getcout(){ return count;}

    ~shared_ptr()
    {
        delete ptr;
    }

};


#endif //SMART_POINTERS_SHARED_PTR_H
