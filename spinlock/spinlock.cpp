#include<iostream>
#include<unistd.h>
#include<mutex>
#include<atomic>
#include<thread>
#include<vector>
#include<chrono>

class spin_lock{
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
	void lock(){
		while(flag.test_and_set()){
		
		}
	}
	void unlock(){
		flag.clear();
	}
};

int count=0;
spin_lock splk;
std::mutex mux;
const int PER_THREAD=1000000;
const int THREAD_NUM=4;

void test_no_lock(){
	std::vector<std::thread> threads;
	for(int i=0;i<THREAD_NUM;++i){
		threads.emplace_back([&]{
			for(int j=0;j<PER_THREAD;++j){
				count++;
			}});
	}
	for(auto &t:threads)
		t.join();
}
void test_spin_lock(){
	std::vector<std::thread> threads;
	for(int i=0;i<THREAD_NUM;++i){
		threads.emplace_back([&]{
			for(int j=0;j<PER_THREAD;++j){
				splk.lock();
				count++;
				splk.unlock();
			}
		});
	}
	for(auto &t:threads)
		t.join();
}
void test_mutex(){
	std::vector<std::thread>threads;
	for(int i=0;i<THREAD_NUM;++i){
		threads.emplace_back([&]{
			for(int j=0;j<PER_THREAD;++j){
				mux.lock();
				count++;
				mux.unlock();
			}
		});
	}

	for(auto &t:threads)
		t.join();
}

template<typename F>
	long long time_ms(F f){
        auto start = std::chrono::steady_clock::now();
        f();
        auto end = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  }

void test_long_critical(){
      std::vector<std::thread> threads;
      for(int i=0;i<THREAD_NUM;++i){
          threads.emplace_back([&](){
              for(int j=0;j<1000;++j){  // 临界区长，少跑几次
                  splk.lock();
		  volatile int x=0;
		  for(int k=0;k<1000000;++k) x+=k;
                  count++;
                  splk.unlock();
              }
          });
      }
      for(auto &t : threads) t.join();
  }

void test_long_critical_mutex(){
      std::vector<std::thread> threads;
      for(int i=0;i<THREAD_NUM;++i){
          threads.emplace_back([&](){
              for(int j=0;j<1000;++j){  
                  mux.lock();
		  volatile int x=0;
                  for(int k=0;k<1000000;++k) x+=k;
                  count++;
                  mux.unlock();
              }
          });
      }
      for(auto &t : threads) t.join();
  }

int main(){
      // 清空count，跑无锁版
      count = 0;
      auto t1 = time_ms(test_no_lock);
      std::cout << "无锁版  → count=" << count << " 耗时" << t1 << "ms\n";

      // 清空count，跑自旋锁版
      count = 0;
      auto t2 = time_ms(test_spin_lock);
      std::cout << "自旋锁版 → count=" << count << " 耗时" << t2 << "ms\n";

      // 清空count，跑mutex版
      count = 0;
      auto t3 = time_ms(test_mutex);
      std::cout << "mutex版  → count=" << count << " 耗时" << t3 << "ms\n";

      count = 0;
      auto t4 = time_ms(test_long_critical);
      std::cout << "长临界区(自旋锁) → count=" << count << " 耗时" << t4 << "ms\n";

      count = 0;
      auto t5 = time_ms(test_long_critical_mutex);
      std::cout << "长临界区(mutex)  → count=" << count << " 耗时" << t5 << "ms\n";
      return 0;
  }
