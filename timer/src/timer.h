#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

namespace timer {

template <class Functor>
class Timer {
    private:
        Functor executable;
        std::string execName;
        // Timer_Thread *thread_p;
        std::thread * d_threadScheduler;
        std::thread * d_threadRunner;
        std::mutex d_mutexForRunner;
        bool d_canceled;
        // std::chrono::time_point<std::chrono::high_resolution_clock> d_threadStart;
        void sleep(const std::chrono::duration<double, std::milli>& timeInterval) {
            std::cout << "[" << std::this_thread::get_id() << "] Sleeping for: " << timeInterval.count() << std::endl;
            std::this_thread::sleep_for(timeInterval);
            {
                std::lock_guard<std::mutex> lock(d_mutexForRunner);
                if (!d_canceled)
                    d_threadRunner = new std::thread (executable);
            }
            if (d_threadRunner)
                d_threadRunner->join();
        }
    public:
        Timer(Functor f, std::string name): executable(f),
            execName(name),
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

            // thread_p = new Timer_Thread(execName, timeInterval);
            // d_threadStart = std::chrono::high_resolution_clock::now();
            d_threadScheduler = new std::thread(&Timer::sleep, this, timeInterval);
            // FIXME does std::thread constructor fail??
        }

        void cancelTask() {
            {
            
                std::lock_guard<std::mutex> lock(d_mutexForRunner);
                if (d_threadRunner) {
                    std::cout << "[" << std::this_thread::get_id() << "] Thread has already started\n.";
                    return;
                }
                d_canceled = true;
            }
        }
};
}
