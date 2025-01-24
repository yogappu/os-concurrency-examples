#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <unistd.h> 

using namespace std;

mutex mtx;
int turn = 0;
int cnt = 1;
condition_variable cv;

void zero(int n) {
    while(cnt<=n) {
        unique_lock<mutex> lock(mtx);
        while(turn != 0 && cnt<=n) cv.wait(lock);
        if(cnt>n) break;
        cout << 0;
        turn = (cnt%2)?1:2;
        cv.notify_all();
    }
}

void odd(int n) {
    while(cnt<=n) {
        unique_lock<mutex> lock(mtx);
        while(turn != 1 && cnt<=n) cv.wait(lock);
        if(cnt>n) break;
        cout << cnt;
        cnt++;
        turn = 0;
        cv.notify_all();
    }
}

void even(int n) {
    while(cnt<=n) {
        unique_lock<mutex> lock(mtx);
        while(turn != 2 && cnt<=n) cv.wait(lock);
        if(cnt>n) break;
        cout << cnt;
        cnt++;
        turn = 0;
        cv.notify_all();
    }
}

int main() {
    // Create and start threads
    int n;
    cout << "enter the number till count is needed : ";
    cin >> n;
    thread t1(zero, n);
    thread t2(odd, n);
    thread t3(even, n);

    // Join all threads
    t1.join();
    t2.join();
    t3.join();

    cout<<endl;
    return 0;
}