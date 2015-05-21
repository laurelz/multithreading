#include<iostream>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

using namespace std;


void cleanup(void* args) 
{
    cout<<"cleanup:"<<(char*)args<<endl;
}

void *thr_fn1(void *args)
{
    cout<<"thread 1 start "<<endl;

	pthread_cleanup_push(cleanup,(void*)"thread 1 first handler");
	pthread_cleanup_push(cleanup,(void*)"thread 1 second handler");
	cout<<"thread 1 push complete"<<endl;
    //sleep(1);
	if (args)
		return((void*)1);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
		return((void*)1);
}

void *thr_fn2(void * args) 
{
    cout<<"thread 2 start "<<endl;

	pthread_cleanup_push(cleanup,(void*)"thread 2 first handler");
	pthread_cleanup_push(cleanup,(void*)"thread 2 second handler");
	cout<<"thread 2 push complete"<<endl;

	if (args)
	    pthread_exit((void*)2);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_exit((void*)2);
}

// Confusion:
// 书上说，把pthread_cleanup_pop()的参数设置为0就不会调用清理函数
// 因此本程序应该只调用第二个线程的清理函数
// 但是实际执行时，两个线程的清理函数都被调用了
// 说明第一个线程也没有从他的启动例程中返回而终止,
// 但是第一个线程确实使用了return语句
// why？
int main () 
{
    int err;
    pthread_t tid1,tid2;
    void *tret;

    err = pthread_create(&tid1,NULL,thr_fn1,(void*)1);
    if (err != 0)
        cout<<"can't create thread 1: "<< err;

    err = pthread_create(&tid2,NULL,thr_fn2,(void*)2);
    if (err != 0)
        cout<<"can't create thread 2: "<< err;

	//err = pthread_cancel(tid1);

    err = pthread_join(tid1,&tret);
    if (err != 0)
        cout<<"can't join thread with thread 1: "<< err;
    cout<<"thread 1 exit code is "<<tret<<endl;

    err = pthread_join(tid2,&tret);
    if (err != 0)
        cout<<"can't join thread with thread 2: " << err;
    cout<<"thread 2 exit code is "<<tret<<endl;

    exit(0);
}
