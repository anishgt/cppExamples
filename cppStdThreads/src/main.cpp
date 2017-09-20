#include<iostream>
#include<thread>
#include <mutex>

namespace {
    void printUtil(int input) {
        static std::mutex m;
        std::lock_guard<std::mutex> lock(m);
        std::cout << "[" << input << "]" << std::endl;
    }

}
void printNumbers(int start, int stop) {
    if (start > stop) {
        std::swap(start, stop);
    }
    for(int i=start; i<=stop; ++i) {
        printUtil(i);
         std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
int main() {
    std::cout << "Hello World\n";
    std::thread t2(printNumbers, 1, 50);
    std::thread t3(printNumbers, 51, 100);
    t2.join();
    t3.join();
    // printNumbers(100, 10);
    return 0;
}
