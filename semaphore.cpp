//
// Created by chenzhi on 2023/4/21.
//

#include <iostream>
#include <pthread.h>
#include <thread>
#include <string>
#include <semaphore.h>
#include <chrono>

// 全局变量作为共享资源
int g_counter = 0;

// POSIX 信号量
sem_t g_semaphore;

struct params{
    params(int r, std::string nm): range(r), name(nm) {}
    int range;
    std::string name;
};

// 线程函数
void* increment(void* arg) {
    params* parg = static_cast<params*>(arg);
    for (int i = 0; i < parg->range; ++i) {
        // 等待信号量，获取资源访问权限
        sem_wait(&g_semaphore);
        std::cout << "this round by " << parg->name << std::endl;
        // 休眠1秒

        std::this_thread::sleep_for(std::chrono::seconds (1));
        ++g_counter; // 访问全局变量
        // 释放信号量，释放资源访问权限
        sem_post(&g_semaphore);
    }
    return nullptr;
}

int main() {
    // 初始化信号量
    sem_init(&g_semaphore, 0, 1);

    // 创建两个线程
    pthread_t t1, t2;
    params p1(50, "thread1");
    params p2(50, "thread2");
    pthread_create(&t1, nullptr, increment, &p1);
    pthread_create(&t2, nullptr, increment, &p2);

    // 等待线程执行完毕
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    // 输出最终的全局变量值
    std::cout << "Final counter value: " << g_counter << std::endl;

    // 销毁信号量
    sem_destroy(&g_semaphore);

    return 0;
}
