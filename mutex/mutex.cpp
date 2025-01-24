#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int shared_data = 0;
mutex mtx;

// Function to increment the shared data
void increment(void) {
    for (int cnt = 0; cnt < 100000; cnt++) {
        mtx.lock();
        shared_data++; 
        mtx.unlock();
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
