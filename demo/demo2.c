#include <stdio.h>
#include <coroutine.h>

void coroutine(Scheduler_t* scheduler,CoHandle_t handle,void* param)
{
	for(int i = 0; i < 10; i++)
	{
		// print handle and i
		printf("coroutine %lu : %d\n",handle,i);
		CoYield(scheduler,handle,1);//yield
	}
}

int main()
{
	Scheduler_t* scheduler = CoCreateScheduler(10);

	// create 2 coroutine by 1 function
	CoHandle_t handle0 = CoCreate(scheduler,coroutine,NULL);
	CoHandle_t handle1 = CoCreate(scheduler,coroutine,NULL);

	//resume coroutine until CoResume return 0
	while(CoResume(scheduler,handle0,1) && CoResume(scheduler,handle1,1));
	
	return 0;
}

