#include <iostream>
#include <thread>
#include <vector>

using namespace std;

long g_count = 0;

void func_count(){
    for (int i = 0; i < 100000; ++i) {
        g_count++;
    }
}



int main() {
    vector<thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.push_back(thread(func_count));
    }
    for (int j = 0; j < 10; ++j) {
        threads[j].join();
    }
    std::cout << g_count << std::endl;
}
