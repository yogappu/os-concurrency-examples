#include <iostream>
#include <thread>
#include <semaphore> 

using namespace std;

int shared_data = 0;
binary_semaphore bin_sem(1);  // Binary semaphore with an initial value of 1 (unlocked)

// Function to increment the shared data
void increment(void) {
    for (int cnt = 0; cnt < 100000; cnt++) {
        bin_sem.acquire(); // Acquire the binary semaphore before accessing critical section
        shared_data++; 
        bin_sem.release(); // Release the binary semaphore after accessing critical section
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