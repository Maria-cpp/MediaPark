#include<iostream>

using namespace std;

template <typename T>
class shared_ptr {
    T* ptr;
    int *count;	//shared. shallow copied

    inline void dec_count() {
        (*count)--;
    }

    inline void inc_count() {
        (*count)++;
    }
public:
    shared_ptr(T* _ptr) {
        ptr = _ptr;
        count = new int(1);
    }

    ~shared_ptr() {
        //cout << "In ~CuShPtr" << endl;
        if(count != nullptr) {
            dec_count();
            if(*count == 0) {
                delete ptr;
                delete count;
            }
        }
    }

    shared_ptr(const shared_ptr<T>& other) {
        ptr = other.ptr;
        count = other.count;
        inc_count();
    }
    shared_ptr<T>& operator=(const shared_ptr<T>& other) {
        //TODO: only if count != nullptr?
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