#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace timer {

template <class Functor>
class Timer {
    private:
        Functor                   d_executable;
        std::string               d_execName;
        std::thread              *d_threadScheduler;
        std::thread              *d_threadRunner;
        std::mutex                d_mutexForRunner;
        std::condition_variable   d_cv;
        bool                      d_canceled;

        // PRIVATE method scheduler method
        void schedule(const std::chrono::duration<double, std::milli>& timeInterval) {
            std::cout << "["
                      << std::this_thread::get_id()
                      << "] Sleeping for: "
                      << timeInterval.count()
                      << std::endl;
            // adding scope layer for unique_lock
            {
                std::unique_lock<std::mutex> lk(d_mutexForRunner);
                if(d_cv.wait_for(lk, timeInterval, [&]{return d_canceled;})) {
                    std::cout << "["
                        << std::this_thread::get_id()
                        << "] Task was canceled\n";
                    return;
                } else {
                    std::cout << "["
                              << std::this_thread::get_id()
                              << "] Task not canceled. Creating thread\n";
                    // It is possible to reach this code block on CV notify or timeout.
                    // We control the CV, so ideally should reach here only on timeout
                    // (CV notify is only called when the d_canceled is set to
                    // true)
                    
                    // wait_for has acquired the lock for us
                    d_threadRunner = new std::thread (d_executable);
                }
            }
            if (d_threadRunner)
                d_threadRunner->join();
        }
    public:
        Timer(Functor f, std::string name): d_executable(f),
            d_execName(name),
            d_threadScheduler(NULL),
            d_threadRunner(NULL),
            d_canceled(false){
            // EMPTY
        } 

        ~Timer() {
            std::cout << "[" << std::this_thread::get_id() << "] In destructor of Timer. Calling join\n";
            d_threadScheduler->join();
            std::cout << "[" << std::this_thread::get_id() << "] After join\n";
            delete d_threadScheduler;
        }

        void scheduleTask(const std::chrono::duration<double,
                std::milli>& timeInterval) {

            d_threadScheduler = new std::thread(&Timer::schedule, this, timeInterval);
            // FIXME does std::thread constructor fail??
        }

        void cancelTask() {
            {
            
                std::lock_guard<std::mutex> lock(d_mutexForRunner);
                if (d_threadRunner) {
                    std::cout << "[" << std::this_thread::get_id() << "] Thread has already started\n.";
                    return;
                }
                // Thread has not been scheduled yet..
                d_canceled = true;
                d_cv.notify_all();
            }
        }
};
}
