#pragma once

#include <memory.h>
#include <mutex>
#include <queue>

template<class T>
class ThreadSafeQueue{

private:
    std::queue<T> data_;
    mutable std::mutex mutex_;


public:
    ThreadSafeQueue(){

    }
    ThreadSafeQueue(const ThreadSafeQueue& other){

    }

    void push(const T& value){

        std::lock_guard<std::mutex> lock(mutex_);
        data_.push(value);

    }

    void pop(){

        if(!empty()){
            std::lock_guard<std::mutex> lock(mutex_);
            data_.pop();
        }

    }

    const T& front()const {
        if(!empty()){
            std::lock_guard<std::mutex> lock(mutex_);// ? need use
            return data_.front();
        }
        return T();
    }

    bool try_to_pop()const{

        return !empty();
    }

    bool empty()const{

        std::lock_guard<std::mutex> lock(mutex_);
        return data_.empty();
    }





};
