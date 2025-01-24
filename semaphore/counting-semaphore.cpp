#include <iostream>
#include <thread>
#include <semaphore>  // For counting_semaphore
#include <mutex>      // For mutex
#include <unistd.h>

using namespace std;

counting_semaphore<3> server_semaphore(3);  // Allow 3 servers to handle requests concurrently
mutex print_mutex;  // Mutex for synchronizing output

// Function simulating a server handling a request
void handle_request(int client_id) {
    {
        // Synchronize access to cout to prevent mixed output
        print_mutex.lock();  // Lock the mutex to safely print
        cout << "Client " << client_id << " is requesting access to a server.\n";
        print_mutex.unlock(); 
    }
    server_semaphore.acquire();  // Try to acquire a server
    {
        // Synchronize access to cout to prevent mixed output
        print_mutex.lock();  // Lock the mutex to safely print
        cout << "Client " << client_id << " is being processed by a server.\n";
        print_mutex.unlock();  // Lock the mutex to safely print
    }
    // Simulate server processing
    sleep(2);
    {
        // Synchronize access to cout to prevent mixed output
        print_mutex.lock();  // Lock the mutex to safely print
        cout << "Client " << client_id << " has been processed and is leaving.\n";
        print_mutex.unlock();  // Lock the mutex to safely print
    }
    server_semaphore.release();  // Release the server back to the pool
}

int main() {
    // Simulate incoming client requests
    thread t1(handle_request, 1);  // Thread for Client 1
    thread t2(handle_request, 2);  // Thread for Client 2
    thread t3(handle_request, 3);  // Thread for Client 3
    thread t4(handle_request, 4);  // Thread for Client 4
    thread t5(handle_request, 5);  // Thread for Client 5

    // Wait for all threads to finish
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}
