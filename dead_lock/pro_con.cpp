#include<queue>
#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

std::queue<int>  q;
std::mutex mux;
std::condition_variable not_empty;
std::condition_variable not_full;
const int MAX_SIZE=5;
const int PRODUCE_NUM=10;

void producer(int id){
	for(int i=0;i<PRODUCE_NUM;++i){
		std::unique_lock<std::mutex> lk(mux);
		while(q.size()==MAX_SIZE){	
			not_full.wait(lk);
 		}
		q.push(i);
		std::cout<<"[P"<<id<<"] produce "<<i<<std::endl;
		not_empty.notify_one();
		
	}
}

void consumer(int id){
	for(int i=0;i<PRODUCE_NUM;++i){
		std::unique_lock<std::mutex> lk(mux);
		not_empty.wait(lk,[]{return !q.empty();});
		int val=q.front();
		q.pop();
		std::cout<<"[C"<<id<<"] consume "<<val<<std::endl;
		not_full.notify_one();
	}

}
int main(){
	std::thread p1(producer,1);
	std::thread p2(producer,2);
	std::thread c1(consumer,1);
	std::thread c2(consumer,2);
	p1.join();p2.join();c1.join();c2.join();
	std::cout<<"ok"<<std::endl;
	return 0;
}
