#pragma once

#include <common.hpp>
#include <queue>
#include <mutex>

EMOLIB_BEGIN_NAMESPACE

template<typename T>
class ThreadSafeQueue
{
private:
    std::mutex m;
    std::queue<T> q;
    
public:
    ThreadSafeQueue()
    {
        
    }
    
    ~ThreadSafeQueue()
    {
        
    }
    
    void push(T e)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push(e);
    }
    
    bool pop(T& e)
    {
        std::lock_guard<std::mutex> lock(m);
        if (q.empty()) {
            return false;
        }
        e = q.front();
        q.pop();
        return true;
    }
};

EMOLIB_END_NAMESPACE
