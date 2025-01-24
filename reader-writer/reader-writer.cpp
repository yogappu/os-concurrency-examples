#include <iostream>
#include <thread>
#include <semaphore>
#include <unistd.h> 

using namespace std;

binary_semaphore read_cnt_mtx(1);    // Mutex for readers to modify read_count safely
binary_semaphore write_mutex(1);    // Mutex for writers to access shared resource

int read_count(0);    // Keeps track of the number of readers currently reading
int shared_resource = 0;      // Shared resource (to be modified by writers and read by readers)
mutex print_mutex;  // Mutex for synchronizing output

// Reader function with exit condition
void reader(int id) {
    do {
        // Wait to read
        read_cnt_mtx.acquire();   // Acquire the mutex for modifying read_count
        read_count++;            // Increment the count of readers
        if (read_count == 1) {   // First reader, lock the resource for exclusive read access
            write_mutex.acquire();
        }
        read_cnt_mtx.release();        // Release the mutex for modifying read_count
        {
            // Synchronize access to cout to prevent mixed output
            print_mutex.lock();  // Lock the mutex to safely print
            // Reading the shared resource (simulating read)
            cout << "Reader " << id << " is reading shared_resource: " << shared_resource << endl;
            print_mutex.unlock();
        }
        usleep(1000);
        // Done reading
        read_cnt_mtx.acquire();        // Acquire the mutex for modifying read_count
        read_count--;         // Decrement the count of readers
        if (read_count == 0) {  // Last reader, release the resource mutex for writers
            write_mutex.release();
        }
        read_cnt_mtx.release();        // Release the mutex for modifying read_count
        usleep(1000);
    } while(true);
}

// Writer function with exit condition
void writer(int id) {
    do{
        // Writer waits for the resource mutex and no readers to be active
        write_mutex.acquire();    // Ensure mutual exclusion for writing

        // Writing to the shared resource
        shared_resource++;
        {
            // Synchronize access to cout to prevent mixed output
            print_mutex.lock();  // Lock the mutex to safely print
            // Reading the shared resource (simulating read)
            cout << "Writer " << id << " is writing shared_resource: " << shared_resource << endl;
            print_mutex.unlock();
        }
        usleep(1000);
        // Done writing
        write_mutex.release();  // Release the resource mutex for other writers
        usleep(1000);
    }while(true);
}

int main() {
    // Create and start reader and writer threads
    thread t1(reader, 1);
    thread t2(reader, 2);
    thread t3(reader, 3);
    thread t4(writer, 1);

    // Join all threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}