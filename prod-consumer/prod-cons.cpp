#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <uinstd.h> 

using namespace std;

const int BUFFER_SIZE = 5;      // Size of the buffer
vector<int> buffer(BUFFER_SIZE); // The shared buffer
int in = 0;  // Points to the next slot where a producer will insert an item
int out = 0; // Points to the next item the consumer will consume
mutex mtx;   // Mutex to protect buffer access

// Semaphores to track the buffer state
counting_semaphore<BUFFER_SIZE> empty_slots(BUFFER_SIZE); // Initially, the buffer is empty
counting_semaphore<0> full_slots(0);                      // Initially, no items are in the buffer

// Producer function
void producer(int id) {
    for (int i = 0; i < 10; ++i) {  // Each producer produces 10 items
        empty_slots.acquire();  // Decrement the empty slots
        mtx.lock();       // Lock the critical section (buffer)
        
        buffer[in] = i;   // Produce an item (just put the value of i)
        cout << "Producer " << id << " produced item " << i << " at position " << in << endl;
        
        in = (in + 1) % BUFFER_SIZE; // Move the 'in' pointer circularly
        
        mtx.unlock();     // Unlock the critical section
        full_slots.release();   // Increment the full slots
        usleep(1000);
    }
}

// Consumer function
void consumer(int id) {
    for (int i = 0; i < 10; ++i) {  // Each consumer consumes 10 items
        full_slots.acquire();  // Decrement the full slots
        mtx.lock();      // Lock the critical section (buffer)
        
        int item = buffer[out];  // Consume an item
        cout << "Consumer " << id << " consumed item " << item << " from position " << out << endl;
        out = (out + 1) % BUFFER_SIZE; // Move the 'out' pointer circularly
        
        mtx.unlock();     // Unlock the critical section
        empty_slots.release();  // Increment the empty slots
        usleep(1000);
    }
}

int main() {
    // Create producer and consumer threads
    thread p1(producer, 1);
    thread p2(producer, 2);
    thread c1(consumer, 1);
    thread c2(consumer, 2);

    // Join all threads
    p1.join();
    p2.join();
    c1.join();
    c2.join();

    return 0;
}