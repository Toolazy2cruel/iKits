//
// Created by chenzhi on 2023/4/20.
//

#include <chrono>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <semaphore.h>
#include <iostream>

using namespace std;

class FizzBuzz {
private:
    int n;
    int critical_val;
    std::mutex mtx_;
    std::condition_variable cv_;

public:
    FizzBuzz(int n) {
        this->n = n;
        critical_val = 1;
    }

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() {return critical_val % 3 == 0 && critical_val % 5 != 0;});
        printFizz(critical_val);
        critical_val++;
        cv_.notify_all();

    }

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() {return critical_val % 5 == 0 && critical_val % 3 != 0;});
        printBuzz(critical_val);
        critical_val++;
        cv_.notify_all();

    }

    // printFizzBuzz() outputs "fizzbuzz".
    void fizzbuzz(function<void()> printFizzBuzz) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() {return critical_val % 3 == 0 && critical_val % 5 == 0;});
        printFizzBuzz(critical_val);
        critical_val++;
        cv_.notify_all();

    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() {return critical_val % 3 != 0 && critical_val % 5 != 0;});
        printNumber(critical_val);
        critical_val++;
        cv_.notify_all();

    }
};


class BoundedBlockingQueue {
private:
    int capacity_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::deque<int> block_queue_;
public:
    BoundedBlockingQueue(int capacity) {
        capacity_ = capacity;
    }

    void enqueue(int element) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() {return block_queue_.size() < capacity_;});
        block_queue_.push_back(element);
        cv_.notify_all();
    }

    int dequeue() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() {return !block_queue_.empty();});
        int elem = block_queue_.front();
        block_queue_.pop_front();
        cv_.notify_all();
        return elem
    }

    int size() {
        std::unique_lock<std::mutex> lock(mtx);
        return block_queue_.size();
    }
};

int main() {


    cout << std::chrono::microseconds(1) << endl;
    return 0;

}