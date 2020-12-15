//
// Created by pc on 14/12/20.
//

#include "shared_ptr.h"

#include<iostream>

//using namespace std;

template <typename T>
class CuShPtr {
    T* ptr;
    int *count;	//shared. shallow copied

    inline void dec_count() {
        (*count)--;
    }

    inline void inc_count() {
        (*count)++;
    }
public:
    CuShPtr(T* _ptr) {
        ptr = _ptr;
        count = new int(1);
    }

    ~CuShPtr() {

        if(count != nullptr) {
            dec_count();
            if(*count == 0) {
                delete ptr;
                delete count;
            }
        }
    }

    CuShPtr(const CuShPtr<T>& other) {
        ptr = other.ptr;
        count = other.count;
        inc_count();
    }

    CuShPtr<T>& operator=(const CuShPtr<T>& other) {
        if(count != nullptr) {
            dec_count();
            if(*count == 0) {
                delete ptr;
            }
        }
        ptr = other.ptr;
        count = other.count;
        inc_count();
        return *this;
    }

    int use_count() const {
        if(count != nullptr)
            return *count;
        else
            return 0;
    }

    void reset() {
        if(count != nullptr) {
            dec_count();
            if(*count == 0) {
                delete ptr;
                delete count;
            }
            count = nullptr;
            ptr = nullptr;
        }
    }

    T* operator*() {
        //cout << "In operator *" << endl;
        return ptr;
    }

    T* operator ->() {
        //cout << "In operator ->" << endl;
        return ptr;
    }
};
