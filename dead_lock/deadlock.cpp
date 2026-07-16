#include<mutex>
#include<iostream>
#include<thread>
#include<unistd.h>
std::mutex mux1;
std::mutex mux2;

void func1(){
	std::lock_guard<std::mutex> lk1(mux1);
	sleep(1);
	std::lock_guard<std::mutex> lk2(mux2);

}

void func2(){
	std::lock_guard<std::mutex> lk2(mux2);
	sleep(1);
	std::lock_guard<std::mutex> lk1(mux1);
}

int main(){
	std::thread t1(func1);
	std::thread t2(func2);
	t1.join();
	t2.join();
	return 0;
}
