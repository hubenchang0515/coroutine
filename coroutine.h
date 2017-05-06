/*
	Author : PlanC
	E-mail : hubenchang0515@outlook.com
	Blog   : www.kurukurumi.com
	File   : coroutine.h
	Date   : 2017-5-4
*/


#ifndef COROUTINE_H
#define COROUTINE_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <ucontext.h>
#include <signal.h>
#include <stdlib.h>

// max quantity of coroutine of one scheduler
#define COROUTINE_MAX_QUANTITY (SIZE_MAX - 1)

// the stack size of every coroutine
#define COROUTINE_STACK_SZIE (10240)

// type of scheduler
typedef struct Scheduler Scheduler_t;

// handle of coroutine
typedef size_t CoHandle_t;

// coroutine funcrions pointer
typedef void (*CoFunc)(Scheduler_t*,CoHandle_t,void*);


/* states of coroutine
 * CoState_NotStart		: coroutine has been created but hasn't bigun to run
 * CoState_Running		: coroutine is running
 * CoState_Yielded		: coroutine has yielded
 * CoState_Terminated	: coroutine has terminated
 */
typedef enum CoStateEnum
{
	CoState_NotStart,
	CoState_Running,
	CoState_Yielded,
	CoState_Terminated,
}CoStateEnum_t;


/* informaion of every coroutine
 * func 	: function of the coroutine
 * param	: parameter of coroutine function
 * context 	: context of the coroutine , to resume coroutine
 * state 	: state of the coroutrine
 * stack 	: stack of the coroutine
 * rvalue 	: return value of CoResume and CoYield
 */
typedef struct CoInfo
{
	CoFunc func;
	void* param;
	ucontext_t context;
	CoStateEnum_t state;
	unsigned char stack[COROUTINE_STACK_SZIE];
	int rvalue;
}CoInfo_t;

/* type of scheduler 
 * length 		: quantity of coroutine in this scheduler
 * maxlength 	: max quantity of coroutine in this scheduler
 * list 		: coroutines' list
 * context		: context of scheduler , to back to CoResume and return
 */
typedef struct Scheduler
{
	size_t length;
	size_t maxlength;
	CoInfo_t* list;
	ucontext_t context;
}Scheduler_t;

#ifdef __cplusplus
	extern "C" {
#endif 




/* function 	: CoCreateScheduler
 *
 * description 	: Create a scheduler
 *
 * parameters	: listLength - max quantity of coroutinr in this scheduler
 *
 * return		: a pointer of Scheduler_t , NULL means failed
 */
Scheduler_t* CoCreateScheduler(size_t listLength);





/* function 	: CoDeleteScheduler
 *
 * description 	: Delete a scheduler
 *
 * parameters	: scheduler - scheduler to delete
 *
 * return 		: void
 */
void CoDeleteScheduler(Scheduler_t* scheduler);




/* function 	: CoResume
 *
 * description 	: resume a coroutine
 *
 * parameters	: scheduler 	- scheduler of coroutine
 *				  handle 		- handle of coroutine
 *				  yield_rvalue	- will be the return value of previous CoYield invocation
 *                              - if yield_rvalue is 0 ,  CoYield will return 1
 *
 * return 		: 3rd parameter of next CoYield invocation , 0 means failed
 */
int CoResume(Scheduler_t* scheduler,CoHandle_t handle,int yield_rvalue);




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
int CoYield(Scheduler_t* scheduler,CoHandle_t handle,int resume_rvalue);




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
CoHandle_t CoCreate(Scheduler_t* scheduler,CoFunc func,void* param);

#ifdef __cplusplus
	}
#endif 

#endif //COROUTINE_H
