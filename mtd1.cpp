#include<iostream>
#include<pthread.h>
#include<unistd.h>

using namespace std;

// #threads
#define NUM_THREAD 5 
void * start_up(void* args) 
{
	int *tid = (int*)args;
    cout<<"hello I am a thread: "<<*tid<<" !"<<endl;
}

int main () 
{
    // thread id 
    pthread_t tids[NUM_THREAD];    
	for (int i = 0; i < NUM_THREAD; ++i) 
	{
		// thread id, thread arguments, 
		// address of the startup function, arguments of the function
	    int ret = pthread_create(&tids[i], NULL, start_up,(void*)&i);
	    cout<< " current tid is "<<tids[i] << endl;
		
		// if created succeed, return 0
		if (ret != 0)
		{
		    cout<<"pthread_create error, error_code = "<<ret<<endl;
		}
	}
	for (int i = 0; i <NUM_THREAD;++i)
	{
	    pthread_join(tids[i], NULL);
	}
	// wait for all threads exiting
	//pthread_exit(NULL);
}
