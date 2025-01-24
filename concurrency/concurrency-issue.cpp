#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;
int shared_data = 0;       // Shared resource

void increment(void) {
    for(int cnt=0; cnt < 100000; cnt++) {
        shared_data++;    //increment data
    }
}

int main(void) {
    thread t1(increment);   // start t1 thread
    thread t2(increment);   // start t2 thread

    t1.join();
    t2.join();

    cout << "value of shared_data is " << shared_data << endl;
    return 0;
}