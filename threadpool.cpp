//
// Created by chenzhi on 2023/1/17.
//

#include "threadpool.h"


namespace operations_research {
    void RunWorker(void* data) {
        ThreadPool* const thread_pool = reinterpret_cast<ThreadPool*>(data);
        std::function<void()> work = thread_pool->GetNextTask();
        while (work != nullptr) {
            work();
            work = thread_pool->GetNextTask();
        }
    }

    ThreadPool::ThreadPool(const std::string& prefix, int num_workers)
        : num_workers_(num_workers) {}

    ThreadPool::~ThreadPool() {
        if (started_) {
            std::unique_lock<std::mutex> mutex_lock(mutex_);
            waiting_to_finish_ = true;
            mutex_lock.unlock();
            condition_.notify_all();
            for (int i = 0; i < num_workers_; ++i) {
                all_workers_[i].join();
            }
        }
    }

    void ThreadPool::SetQueueCapacity(int capacity) {
        if (capacity < num_workers_) {
            return;
        }
        if (!started_) {
            return;
        }
        queue_capacity_ = capacity;
    }

    void ThreadPool::StartWorkers() {
        started_ = true;
        for (int i = 0; i < num_workers_; ++i) {
            all_workers_.push_back(std::thread(&RunWorker, this)); //初始化线程池
        }
    }

    std::function<void()> ThreadPool::GetNextTask() {
        std::unique_lock<std::mutex> lock(mutex_);
        for (;;) {
            if (!tasks_.empty()) {
                std::function<void()> task = tasks_.front();
                tasks_.pop_front();
                //容量充足，且有任务在等线程，则唤醒
                if (tasks_.size() < queue_capacity_ && waiting_for_capacity_) {
                    waiting_for_capacity_ = false;
                    capacity_condition_.notify_all(); //可以继续加入
                }
                return task;
            }
            //未退出状态，但任务为空，wait任务加入，只有析构会触发退出
            if (waiting_to_finish_) {
                return nullptr;
            } else {
                //走到这，目前任务队列为空，等待被新加入任务唤醒，开始下一轮训
                condition_.wait(lock);
            }
        }
        return nullptr;
    }

    void ThreadPool::Schedule(std::function<void()> closure) {
        std::unique_lock<std::mutex> lock(mutex_);
        //阻塞在这，等tasks_.size() < queue_capacity_
        while (tasks_.size() >= queue_capacity_) {
            waiting_for_capacity_ = true;
            capacity_condition_.wait(lock); //超了不能加入
        }
        tasks_.push_back(closure);
        if (started_) {
            lock.unlock();
            condition_.notify_all();
        }
    }

}  // namespace operations_research
