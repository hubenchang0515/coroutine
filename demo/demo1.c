#include <stdio.h>
#include <coroutine.h>

void coroutine(Scheduler_t* scheduler,CoHandle_t handle,void* param)
{
	for(int i = 0; i < 10; i++)
	{
		// print i
		printf("coroutine : %s %d\n",(char*)param,i);
		CoYield(scheduler,handle,1);//yield
	}
}

int main()
{
	Scheduler_t* scheduler = CoCreateScheduler(10);
	CoHandle_t handle = CoCreate(scheduler,coroutine,"hello");
	while(CoResume(scheduler,handle,1));//resume coroutine until CoResume return 0
	
	return 0;
}

