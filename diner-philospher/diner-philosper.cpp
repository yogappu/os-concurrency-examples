#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <unistd.h>

using namespace std;

const int NUM_PHILOSOPHERS = 5;
mutex forks[NUM_PHILOSOPHERS];  // Mutex for each fork
mutex prnt;
// Function for the philosopher's behavior
void philosopher(int id) {
    while (true) {
        prnt.lock();
        cout << "Philosopher " << id << " is thinking.\n";
        prnt.unlock();

        // Simulate thinking time
        usleep(100000);

        // Try to pick up the left fork
        int left = id;
        int right = (id + 1) % NUM_PHILOSOPHERS;

        // Lock both forks (first the left fork, then the right fork)
        forks[left].lock();
        forks[right].lock();

        prnt.lock();
        cout << "Philosopher " << id << " is eating.\n";
        prnt.unlock();

        // Simulate eating time
        usleep(100000);

        prnt.lock();
        cout << "Philosopher " << id << " has finished eating.\n";
        prnt.unlock();

        // Unlock both forks
        forks[left].unlock();
        forks[right].unlock();
    }
}

int main() {
    vector<thread> philosophers;
    // Start philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers.push_back(thread(philosopher, i));
    }
    // Join all threads (they will run indefinitely in this example)
    for (auto& t : philosophers) {
        t.join();
    }
    return 0;
}