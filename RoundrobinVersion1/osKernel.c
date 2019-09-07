#include "osKernel.h"

#define SYSPRI3		(*(volatile uint32_t *)(0xE000ED20))

#define NUMBER_OF_THREADS	3u
#define STACK_SIZE				100u

#define BUS_FREQ					16000000u

static uint32_t MILLIS_PRESCALER;

/* Declearation of task control block */
struct tcb{
	int32_t *stackPointer;
	struct tcb * nextPt;
};

typedef struct tcb tcbType;

/* Task control blocks have been created */
tcbType tcbs[NUMBER_OF_THREADS];
tcbType *currentPt;

/* Stack has been created to support 3 tasks */
int32_t TCB_STACK[NUMBER_OF_THREADS][STACK_SIZE]; 

void osSchedulerLaunch(void);

/* Assigns all stacks to the all task control blocks */
static void osKernelStackInit(int i){
	/* The reason of minus 16 while assigning stack is that excluding processor core registers */
	/* Like, [R0:R12], SP(R13), LR(R14), PC(R15) */
	/* I think, it is decleared in this way because we would like to point top of the stack. */
	tcbs[i].stackPointer = &TCB_STACK[i][STACK_SIZE - 16];
	
	/* Initiliazing XPSRS register and processor will run in thumb mode */
	TCB_STACK[i][STACK_SIZE-1] = 0x01000000;
}

uint8_t osKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void)){

	/* Entering Critical Section. Because of that, interrupts have been disabled. */
	__disable_irq();
	
	tcbs[0].nextPt = &tcbs[1];
	tcbs[1].nextPt = &tcbs[2];
	tcbs[2].nextPt = &tcbs[0];
	
	/* Init stack for task0 */
	osKernelStackInit(0);
	/* Location of program counter */
	TCB_STACK[0][STACK_SIZE - 2] = (int32_t)(task0);
	
	/* Init stack for task1 */
	osKernelStackInit(1);
	/* Location of program counter */
	TCB_STACK[1][STACK_SIZE - 2] = (int32_t)(task1);
	
	/* Init stack for task2 */
	osKernelStackInit(2);
	/* Location of program counter */
	TCB_STACK[2][STACK_SIZE - 2] = (int32_t)(task2);
	
	currentPt = &tcbs[0];
	
	__enable_irq();
	return 1;
}

void osKernelInit(void){
	/* Disable interrupts critical sections */
	__disable_irq();
	
	MILLIS_PRESCALER = BUS_FREQ / 1000;
	
	__enable_irq();
}

void osKernelLaunch(uint32_t quanta){

	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	/* Setting SysTick's priority to level 7 */
	SYSPRI3 = (SYSPRI3 & 0x00FFFFFF) | 0xE0000000;
	/* Set tick period */
	SysTick->LOAD = (quanta * MILLIS_PRESCALER) - 1;
	
	/* Start SysTick */
	SysTick->CTRL = 0x00000007;
	
	osSchedulerLaunch();
	
}

