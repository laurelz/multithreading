#include<iostream>
#include<pthread.h>
#include<unistd.h>

using namespace std;

// #threads
#define NUM_THREAD 5 

int sum = 0;
pthread_mutex_t sum_mutex;

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

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	for (int i = 0; i < NUM_THREAD; ++i) 
    {
		num[i] = i;
		// thread id, thread arguments, 
		// address of the startup function, arguments of the function
	    int ret = pthread_create(&tids[i],&attr, start_up,(void*)&num[i]);
	    //cout<< "current tid is "<<tids[i] << endl;
		
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
	/*
		if (ret != 0)
			cout<<"pthread_join error:error_code="<<ret<<endl;
		else
			cout<<"pthread_join get status:"<<(long)status<<endl;*/
	}
	cout<<"in the end, sum is "<<sum<<endl;
	pthread_mutex_destroy(&sum_mutex);
	// wait for all threads exiting
	//pthread_exit(NULL);
}
