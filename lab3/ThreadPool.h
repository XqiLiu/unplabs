#include <vector>
#include <queue>
#include <pthread.h>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "TcpServer.h"
class ThreadPool {
public:
    ThreadPool(size_t numThreads, TcpServer& tcpserver);
    ~ThreadPool();

    void enqueue(int fd);
    void setCallBack(std::function<void(TcpServer&, int)> callback)
    {callback_=callback;}

private:
    TcpServer& tcpserver_;
    static void* threadEntry(void *arg);
    std::vector<pthread_t> threads;
    std::queue<std::function<void(TcpServer&, int)>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::function<void(TcpServer&, int)> callback_;
    
    bool stop;
};
