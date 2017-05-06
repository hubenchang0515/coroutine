/*
	Author : PlanC
	E-mail : hubenchang0515@outlook.com
	Blog   : www.kurukurumi.com
	File   : coroutine.c
	Date   : 2017-5-4
*/




#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <ucontext.h>
#include <signal.h>
#include <stdlib.h>

#include "coroutine.h"


/* function 	: CoCreateScheduler
 *
 * description 	: Create a scheduler
 *
 * parameters	: listLength - max quantity of coroutinr in this scheduler
 *
 * return		: a pointer of Scheduler_t , NULL means failed
 */
Scheduler_t* CoCreateScheduler(size_t listLength)
{
	if(listLength > COROUTINE_MAX_QUANTITY)
	{
		return NULL;
	}

	Scheduler_t* sp = (Scheduler_t*) malloc(sizeof(Scheduler_t));
	sp->list = (CoInfo_t*) malloc(listLength * sizeof(CoInfo_t));

	sp->length = 0;
	sp->maxlength = listLength;

	return sp;
}


/* function 	: CoDeleteScheduler
 *
 * description 	: Delete a scheduler
 *
 * parameters	: scheduler - scheduler to delete
 *
 * return 		: void
 */
void CoDeleteScheduler(Scheduler_t* scheduler)
{
	free(scheduler->list);
	free(scheduler);
}


/* function 	: CoResume
 *
 * description 	: resume a coroutine
 *
 * parameters	: scheduler 	- scheduler of coroutine
 *				  handle 		- handle of coroutine
 *				  yield_rvalue	- will be the return value of previous CoYield invocation
 *                              - if yield_rvalue is 0 ,  CoYield will return 1
 *
 * return 		: 3rd parameter of next CoYield invocation , 0 means coroutine has terminated , -1 means exception
 */
int CoResume(Scheduler_t* scheduler,CoHandle_t handle,int yield_rvalue)
{
	// if yield_rvalue is 0 ,  CoYield will return 1
	scheduler->list[handle].rvalue = yield_rvalue ? yield_rvalue : 1;
	CoFunc func = scheduler->list[handle].func;
	void* param = scheduler->list[handle].param;
	
	switch(scheduler->list[handle].state)
	{
		case CoState_NotStart: // start the coroutine
				// change state of coroutine to be CoState_Running
				scheduler->list[handle].state = CoState_Running;
				// make a context to invoke starter , go back to &scheduler->context after starter return
				getcontext(&scheduler->list[handle].context);
				scheduler->list[handle].context.uc_stack.ss_sp = scheduler->list[handle].stack;
				scheduler->list[handle].context.uc_stack.ss_size = COROUTINE_STACK_SZIE;
				scheduler->list[handle].context.uc_link = &scheduler->context;
				makecontext(&scheduler->list[handle].context , (void(*)(void))func , 3 ,scheduler, handle,param);
				// invoke starter by scheduler and handle to start the coroutine and save current context
				swapcontext(&scheduler->context,&scheduler->list[handle].context);
				// if coroutine come back without CoYield , that means coroutine has terminated
				if(scheduler->list[handle].state != CoState_Yielded)
				{
					scheduler->list[handle].state = CoState_Terminated;
				}
				
				return scheduler->list[handle].rvalue;

		case CoState_Yielded: // resume the coroutine
				// change state of coroutine to be CoState_Running
				scheduler->list[handle].state = CoState_Running;
				// if coroutine come back without CoYield , that means coroutine has terminated
				swapcontext(&scheduler->context,&scheduler->list[handle].context);
				if(scheduler->list[handle].state != CoState_Yielded)
				{
					scheduler->list[handle].state = CoState_Terminated;
				}
				
				return scheduler->list[handle].rvalue;

		case CoState_Running:
				return 0;

		case CoState_Terminated:
				return 0;

		default:
				return -1; // return -1 means exceptional
	}
}



/* function 	: CoYield
 *
 * description 	: yield a coroutine
 *
 * parameters	: scheduler 	- scheduler of coroutine
 *				  handle 		- handle of coroutine
 *				  resume_rvalue	- will be the return value of previous CoResume invocation
 *                              - if resume_rvalue is 0 , CoResume  will return 1
 *
 * return 		: 3rd parameter of next CoResume invocation
 */
int CoYield(Scheduler_t* scheduler,CoHandle_t handle,int resume_rvalue)
{
	// if resume_rvalue is 0 , CoResume  will return 1
	scheduler->list[handle].rvalue = resume_rvalue ? resume_rvalue : 1;
	// change state of coroutine to be CoState_Yielded
	scheduler->list[handle].state = CoState_Yielded;
	// go back to CoResume
	swapcontext(&scheduler->list[handle].context,&scheduler->context);

	return scheduler->list[handle].rvalue;
}



/* function 	: CoCreate
 *
 * description 	: Create a coroutine
 *
 * parameters	: scheduler - scheduler of coroutine
 *				  func 		- function of coroutine
 *              : param     - parameter of coroutine function
 *
 * return		: a handle of coroutine , -1 means failed
 */
CoHandle_t CoCreate(Scheduler_t* scheduler,CoFunc func,void* param)
{
	if(scheduler->length <= scheduler->maxlength)
	{
		scheduler->list[scheduler->length].func = func;
		scheduler->list[scheduler->length].param = param;
		scheduler->list[scheduler->length].state = CoState_NotStart;
		scheduler->length++;
		return scheduler->length-1;
	}
	else
		return -1;
}
