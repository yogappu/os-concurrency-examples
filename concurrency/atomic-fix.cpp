#include <iostream>
#include <thread>
#include <atomic>  // For std::atomic

using namespace std;

atomic<int> shared_data(0);  // Atomic shared resource

// Function to increment the shared data
void increment(void) {
    for (int cnt = 0; cnt < 100000; cnt++) {
        shared_data++;  // Atomically increment the shared data
    }
}

int main(void) {
    thread t1(increment);   // Start t1 thread
    thread t2(increment);   // Start t2 thread

    t1.join();
    t2.join();

    cout << "value of shared_data is " << shared_data << endl;
    return 0;
}
