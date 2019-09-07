#include "osKernel.h"

#define USE_LED 1

uint32_t counter1 = 0;
uint32_t counter2 = 0;
uint32_t counter3 = 0;

int32_t sem = 0;

enum LEDs{
	LED_RED	= 0,
	LED_BLUE,
	
	LED_MAX
};

void gpio_set_toggle(enum LEDs which_led);

void task0(void);
void task1(void);
void task2(void);

int main(void)
{

	/* Stop watchdog timer */
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     
	
	/* Initilization of kernel */
	osKernelInit();
	
	/* Adding task to the kernel */
	osKernelAddThreads(&task0, &task1, &task2);
	
	/* Initilize semaphore */
	osSemaphoreInit(&sem, 0);
	
	/* Launching kernel */
	osKernelLaunch(10);
}
void task0(void){
	while(1){
#if USE_LED
		/* Wait for sem */
		osSignalWait(&sem);
		
		gpio_set_toggle(LED_RED);
#else
		counter1++;
#endif
	}
	
}
void task1(void){
	static uint16_t counter = 0;
	
	while(1){
#if USE_LED
		
		counter++;
		
		if(counter == 5){
			counter = 0;
			osSignalSet(&sem);
		}
		
		gpio_set_toggle(LED_BLUE);
#else
		counter2++;
#endif
	}
}
void task2(void){
	while(1){
#if USE_LED
		//gpio_set_toggle(LED_RED);
		//gpio_set_toggle(LED_BLUE);
#else
		counter3++;
#endif	
	}
}


void gpio_set_toggle(enum LEDs which_led)
{
	P1->DIR |= BIT0;
	P2->DIR |= BIT2;
	
	if(LED_RED == which_led){
		/* Configure P1.0 as output  LED Port 1 - RED*/
		P1->OUT ^= BIT0;                      
	}
	
	if(LED_BLUE == which_led){
		/* Configure P2.2 as output  LED Port 2 - BLUE*/
		P2->OUT ^= BIT2;                      
	}

}