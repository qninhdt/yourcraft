#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <iostream>
namespace yc {

class ThreadPool {

public:

    ThreadPool() {};

    void start() {
        const int maxThreads = 1;

        for (int i=0;i<maxThreads;++i) {
            this->threads.push_back(std::thread([this] { loop(); }));
        }
    }

    void loop() {
        while (true) {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(this->queueLock);

                this->flag.wait(lock, [this](){
                    return !jobs.empty() || shouldTerminate;
                });

                if (shouldTerminate && jobs.empty()) return;

                job = jobs.front();
                jobs.pop();
            }
            job();
        }
    }

    void queueJob(const std::function<void()>& job) {
        {
            std::unique_lock<std::mutex> lock(this->queueLock);
            jobs.push(job);
        }
        this->flag.notify_one();
    }

    void stop() {
        {
            std::unique_lock<std::mutex> lock(this->queueLock);
            this->shouldTerminate = true;
        }
        this->flag.notify_all();
        for (std::thread& active_thread : this->threads) {
            active_thread.join();
        }
        this->threads.clear();
    }
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
    std::mutex queueLock;
    std::condition_variable flag;
    bool shouldTerminate = false;

};

}