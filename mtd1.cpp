#include<iostream>
#include<pthread.h>
#include<unistd.h>

using namespace std;

// #threads
#define NUM_THREAD 5 

int sum = 0;
pthread_mutex_t sum_mutex;

struct foo {
	int a,b,c,d;
};

void printinfo(const char* s, const struct foo*fp) 
{
	cout<<s;
	cout<<"sturcture at "<<fp<<endl;
	cout<<"foo a is "<<fp->a<<endl;
	cout<<"foo b is "<<fp->b<<endl;
	cout<<"foo c is "<<fp->c<<endl;
	cout<<"foo d is "<<fp->d<<endl;
}

void * startup_1(void * args)
{
	//創建一個結構，並通過pthread_exit()返回
	//此時是創建了一個自動變量，存儲在棧上，返回時被覆蓋
	//使用動態內存分配可以使其分配在堆中，從而正確訪問
	struct foo * f = new foo();
	f->a = 1;
	f->b = 2;
	f->c = 3;
	f->d = 4;
    printinfo("new thread1: \n",f);
	pthread_exit((void*)f);
}

void * startup_2(void*args)
{
    cout<<"new thread2: ID is "<<pthread_self()<<endl;
	pthread_exit((void*)0);
}
void * start_up(void* args) 
{
	int *tid = (int*)args;
    cout<<"hello I am a thread: "<<*tid<<" !"<<endl;
	
	pthread_mutex_lock(&sum_mutex);
	cout<<"before sum is "<<sum<<" in thread "<<*tid<<endl;
    sum += *tid;
	cout<<"after sum is "<<sum<<" in thread "<<*tid<<endl;
	pthread_mutex_unlock(&sum_mutex);

	//int status = 10 + *((int *)args);
	//pthread_exit((void*)status);
	pthread_exit(0);
}

int main () 
{
    // thread id 
    pthread_t tids[NUM_THREAD];    

	int num[NUM_THREAD];

	sched_param param;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    pthread_attr_getschedparam(&attr, &param);

	struct foo * fp;

	int ret = pthread_create(&tids[0],NULL,startup_1, NULL);
	
	ret = pthread_join(tids[0], (void**)&fp);
	sleep(1);
	cout<<"parent thread starting second thread"<<endl;
	
	ret = pthread_create(&tids[1],NULL,startup_2,NULL);
	sleep(1);
	printinfo("parent: \n",fp);
/*	for (int i = 0; i < NUM_THREAD; ++i) 
    {
		num[i] = i;
		// thread id, thread arguments, 
		// address of the startup function, arguments of the function
		param.sched_priority = i+2;
		pthread_attr_setschedparam(&attr, &param);
		int ret = pthread_create(&tids[i],&attr, start_up,(void*)&num[i]);

	    cout<< "current tid is "<<i<<" and priority is "<<i+2 << endl;
		
		// if created succeed, return 0
		if (ret != 0)
		{
		    cout<<"pthread_create error, error_code = "<<ret<<endl;
		}
	}
	pthread_attr_destroy(&attr);

	void *status;

	for (int i = 0; i <NUM_THREAD;++i)
	{
	    int ret = pthread_join(tids[i], &status);
	
		if (ret != 0)
			cout<<"pthread_join error:error_code="<<ret<<endl;
		else
			cout<<"pthread_join get status:"<<(long)status<<endl;
	}
	cout<<"in the end, sum is "<<sum<<endl;
	pthread_mutex_destroy(&sum_mutex);*/

	// wait for all threads exiting
	pthread_exit(NULL);
}
