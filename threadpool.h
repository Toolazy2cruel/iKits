//
// Created by chenzhi on 2023/1/17.
//

#ifndef IKITS_THREADPOOL_H
#define IKITS_THREADPOOL_H

#include <condition_variable>  // NOLINT
#include <functional>
#include <list>
#include <mutex>  // NOLINT
#include <string>
#include <thread>  // NOLINT
#include <vector>

namespace operations_research {
    class ThreadPool {
    public:
        ThreadPool(const std::string& prefix, int num_threads);
        ~ThreadPool();

        void StartWorkers();
        void Schedule(std::function<void()> closure);
        std::function<void()> GetNextTask();
        void SetQueueCapacity(int capacity);

    private:
        const int num_workers_;
        std::list<std::function<void()>> tasks_;
        std::mutex mutex_;
        std::condition_variable condition_;
        std::condition_variable capacity_condition_;
        bool waiting_to_finish_ = false;
        bool waiting_for_capacity_ = false;
        bool started_ = false;
        int queue_capacity_ = 2e9;
        std::vector<std::thread> all_workers_;
    };
}  // namespace operations_research

#endif //IKITS_THREADPOOL_H
