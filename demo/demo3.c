#include <stdio.h>
#include <coroutine.h>


void coroutine(Scheduler_t* scheduler,CoHandle_t handle)
{
	for(int i = 0; i < 10; i++)
	{
		// print yield return value
		printf("yield return : %d\n",CoYield(scheduler,handle,i));
	}
}

int main()
{
	Scheduler_t* scheduler = CoCreateScheduler(10);
	CoHandle_t handle = CoCreate(scheduler,coroutine);

	CoResume(scheduler,handle,1); // first time 3rd parameter will be ignored
	CoResume(scheduler,handle,2);
	CoResume(scheduler,handle,3);
	printf("resume return : %d\n",CoResume(scheduler,handle,4));
	printf("resume return : %d\n",CoResume(scheduler,handle,5));
	
	return 0;
}

