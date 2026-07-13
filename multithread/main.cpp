#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
#include<atomic>

int count=0;
std::atomic<int> count1={0};
std::mutex mux;

void test_no_lock(){
	count=0;
	std::thread t1([&](){for(int i=0;i<100000;++i) count++;});
	std::thread t2([&](){for(int i=0;i<100000;++i) count++;});
	std::thread t3([&](){for(int i=0;i<100000;++i) count++;});
	std::thread t4([&](){for(int i=0;i<100000;++i) count++;});

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	std::cout<<"no_lock 结果="<<count<<" (预期400000)"<<std::endl; 
}

void test_mutex(){
	count=0;
	std::thread t1([&](){
		for(int i=0;i<100000;++i){	
			std::lock_guard<std::mutex> guard(mux);
			count++;
		}
 	});
	std::thread t2([&](){
                for(int i=0;i<100000;++i){      
                        std::lock_guard<std::mutex> guard(mux);
                        count++;
                }
        });
	std::thread t3([&](){
                for(int i=0;i<100000;++i){      
                        std::lock_guard<std::mutex> guard(mux);
                        count++;
                }
        });
	std::thread t4([&](){
                for(int i=0;i<100000;++i){      
                        std::lock_guard<std::mutex> guard(mux);
                        count++;
                }
        });
	
	t1.join();t2.join();t3.join();t4.join();
	std::cout<<"mutex 结果="<<count<<" (预期400000)"<<std::endl; 


}

void test_atomic(){
	count1=0;
	std::thread t1([&](){
                for(int i=0;i<100000;++i){      
                        count1.fetch_add(1);
                }
        });
        std::thread t2([&](){
                for(int i=0;i<100000;++i){      
                        count1.fetch_add(1);
                }
        });
        std::thread t3([&](){
                for(int i=0;i<100000;++i){      
                        count1.fetch_add(1);
                }
        });
        std::thread t4([&](){
                for(int i=0;i<100000;++i){      
                        count1.fetch_add(1);
                }
        });

        t1.join();t2.join();t3.join();t4.join();
        std::cout<<"atomic 结果="<<count1<<" (预期400000)"<<std::endl; 

}
int main(){
	test_no_lock();
	test_mutex();
	test_atomic();
	return 0;
}

