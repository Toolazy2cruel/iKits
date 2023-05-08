//
// Created by chenzhi on 2023/4/18.
//

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

// 任务类
class Task {
public:
    virtual void execute() = 0;
};

// 具体任务类
class ConcreteTask : public Task {
public:
    ConcreteTask(int id) : m_id(id) {}

    void execute() override {
        std::cout << "Task " << m_id << " is being executed in thread " << std::this_thread::get_id() << std::endl;
    }

private:
    int m_id;
};

// 线程池类
class ThreadPool {
public:
    ThreadPool(int numThreads) : m_numThreads(numThreads), m_stop(false) {
        // 创建指定数量的工作线程
        for (int i = 0; i < m_numThreads; ++i) {
            m_threads.emplace_back(std::thread(&ThreadPool::workerThread, this));
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_stop = true;
            m_condVar.notify_all();
        }

        // 等待所有线程退出
        for (std::thread& thread : m_threads) {
            thread.join();
        }
    }

    // 提交任务到线程池
    void submitTask(Task* task) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_taskQueue.push(task);
        m_condVar.notify_one();
    }

private:
    void workerThread() {
        while (true) {
            //这里应该能看到1个问题，submitTask也会放任务进去，这个冲突是通过m_condVar实现的
            std::unique_lock<std::mutex> lock(m_mutex);

            // 线程池停止时，工作线程退出
            if (m_stop && m_taskQueue.empty()) {
                return;
            }
//            m_condVar.wait(lock, [this]() { return m_stop || !m_taskQueue.empty(); }); 中的 wait() 函数会使当前线程阻塞，并释放之前持有的互斥锁 lock，允许其他线程访问该互斥锁保护的共享资源。
//            等待期间，会通过 lambda 表达式 [this]() { return m_stop || !m_taskQueue.empty(); } 来判断是否满足等待的条件。这里的 lambda 表达式定义了一个条件，即当 m_stop 为真（表示线程池停止运行）或者 m_taskQueue 非空（表示有任务待处理）时，等待条件变量的通知。
//            当满足等待条件时，线程会被唤醒，重新获取互斥锁 lock，继续执行后续的代码。如果等待期间发生了虚假唤醒（spurious wakeup），即条件变量没有被显式通知但线程被唤醒，通过条件判断可以避免线程误认为满足了等待条件而错误地继续执行。
//            这种使用方式允许线程在等待期间自动释放互斥锁，并在条件满足时被唤醒，从而实现了有效的线程协调和同步

            // 等待任务队列非空
            m_condVar.wait(lock, [this]() { return m_stop || !m_taskQueue.empty(); });

            // 获取任务并执行
            if (!m_taskQueue.empty()) {
                Task* task = m_taskQueue.front();
                m_taskQueue.pop();
                lock.unlock();
                task->execute();
                delete task; // 注意：需要手动释放任务资源
            }
        }
    }

    int m_numThreads;                      // 线程池中的线程数量
    std::vector<std::thread> m_threads;    // 工作线程数组
    std::queue<Task*> m_taskQueue;         // 任务队列
    std::mutex m_mutex;                    // 互斥锁
    std::condition_variable m_condVar;      // 条件变量
    bool m_stop;                          // 线程池停止标志
};

//int main() {
//    ThreadPool threadPool(4); // 创建线程池，包含4个线程
//
//    // 提交任务到线程池
//    for (int i = 0; i < 10; ++i) {
//        Task* task = new ConcreteTask(i);
//        threadPool.submitTask(task);
//    }
//
//    // 主线程等待一段时间，观察工作线程的执行情况
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//
//    return 0;
//}
