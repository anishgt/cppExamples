#include<iostream>
#include<thread>
#include <mutex>
#include <timer.h>

namespace {
    class PrintDone {
        private:
            std::string d_name;
        public:
            PrintDone(std::string name): d_name(name) {
            }
            void operator() () const {
                std::cout << "[" << std::this_thread::get_id() << "] Done with " << d_name << "\n";
            }
    };

}
int main() {
    timer::Timer<PrintDone> myTimer(PrintDone("A"), "A");
    std::cout << "[" << std::this_thread::get_id() << "] Scheduling task A\n";
    myTimer.scheduleTask(std::chrono::duration<double, std::milli>(10000));
    //std::cout << "[" << std::this_thread::get_id() << "] Scheduled task A\n";
    myTimer.cancelTask();
    std::cout << "[" << std::this_thread::get_id() << "] Canceled task A\n";
    return 0;
}        
