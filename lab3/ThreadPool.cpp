#include "ThreadPool.h"
void* ThreadPool::threadEntry(void *arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    while(true) {
        std::function<void(TcpServer&, int)> task;
        {
            std::unique_lock<std::mutex> lock(pool->queueMutex);
            while(!pool->stop && pool->taskQueue.empty()) {
                pool->condition.wait(lock);
            }
            if(pool->stop && pool->taskQueue.empty()) {
                return NULL;
            }
            task = pool->taskQueue.front();
            pool->taskQueue.pop();
        }
        task(pool->tcpserver_, 0);
    }
    return NULL;
}
ThreadPool::ThreadPool(size_t numThreads, TcpServer& tcpserver) : stop(false), tcpserver_(tcpserver) {
    for(size_t i = 0; i < numThreads; ++i) {
        pthread_t thread;
        pthread_create(&thread, NULL, &ThreadPool::threadEntry, this);
        threads.push_back(thread);
    }
}
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for(pthread_t &thread : threads) {
        pthread_join(thread, NULL);
    }
}
void ThreadPool::enqueue(int fd) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskQueue.push(std::bind(callback_, std::placeholders::_1, fd));
    }
    condition.notify_one();
}