//
// Created by 18716 on 2019/8/6.
//

#ifndef PBFTV2_MYVECTOR_H
#define PBFTV2_MYVECTOR_H

#include <exception>
#include <iostream>
#include <vector>
#include <mutex>
using namespace std;

struct empty_vector:exception
{
    const char* what() const throw();
};

template<typename T>
class MyVector {
private:
    vector<T> data;
    mutable mutex m;
public:
    MyVector();

    MyVector(const MyVector& other){
        lock_guard<mutex> lock(other.m);
        data=other.data;
    }

//    MyVector& oprator=(const MyVector&)=delete;

    void push(T new_value){
        lock_guard<mutex> lock(m);
        data.push_back(move(new_value));
    }

//    shared_ptr<T>


};


#endif //PBFTV2_MYVECTOR_H
