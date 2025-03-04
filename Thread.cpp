#include <iostream>
#include <future>
#include <chrono>

int print_numbers() {
    for (int i = 0; i < 5; i++) {
        std::cout << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 5;
}

int main() {
    auto future1 = std::async(std::launch::async, print_numbers);
    auto future2 = std::async(std::launch::async, print_numbers);

    future1.wait();
    future2.wait();

    std::cout << "Main thread finished" << std::endl;
    return 0;
}