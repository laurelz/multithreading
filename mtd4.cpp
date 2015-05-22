#include<iostream>
#include<pthread.h>
#include<unistd.h>

using namespace std;

pthread_mutex_t count_lock;
pthread_cond_t count_cond;

// 两个线程对同一个全局变量进行访问，使用条件变量同步
// 全局变量的初值为0
unsigned int count = 0;

// 对变量进行减一操作的线程工作，thread1
// 通过while循环来检查条件变量是否满足条件
// 此处不能使用if，需要循环判断当前变量是否满足条件(count !=0)
// 若count == 0, 则一直处于等待状态，阻塞本线程
// 当在其他线程中通过signal唤醒本线程时，继续执行阻塞后的语句
// 即进入while循环，继续判断条件是否满足
// 满足跳出循环的条件，则对count-1
void *decrement_count(void* args) 
{
	for (int i = 0;  ; i++) {
		cout<<"this is thread 1 : "<< pthread_self()<<endl;
    pthread_mutex_lock(&count_lock);
	cout<<"thread 1: "<<pthread_self()<<" get the lock"<<endl;
    while(count == 0) {
		cout<<"thread 1 is wait for cond"<<endl;
	    pthread_cond_wait(&count_cond, &count_lock);
	}

		cout<<"thread 1 is wake up and count is "<<count<<endl;
	count = count - 1;
	pthread_mutex_unlock(&count_lock);
	sleep(1);
    }
}

// 对变量进行加一操作的线程工作，thread2
// 每次执行时，检查当前全局变量值count是否已经满足thread1的条件
// 即，若count ！= 0,则通过signal通知thread1,将其唤醒
// 由于此时thread2仍拥有mutex的锁，所以继续执行语句
void *increment_count(void* args) 
{
	for (int i = 0; ; ++i) {
	sleep(1);
		cout<<"this is thread 2: "<< pthread_self()<<endl;

    pthread_mutex_lock(&count_lock);
	cout<<"thread 2: "<<pthread_self()<<" get the lock"<<endl;
	if (count != 0) {
		pthread_cond_signal(&count_cond);
		cout<<"thread 2 signal thread 1"<<endl;
    }
		cout<<"thread 2 is wake up and count is "<<count<<endl;
	count = count + 1;
	pthread_mutex_unlock(&count_lock);
	}
}

int main() 
{
    pthread_t tid1, tid2;
		cout<<"this is main thread "<< pthread_self()<<endl;
	pthread_create(&tid1, NULL, decrement_count, NULL);
	pthread_create(&tid2, NULL, increment_count, NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_exit(0);

}
