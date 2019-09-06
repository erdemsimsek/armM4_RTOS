#include "osKernel.h"

#define USE_LED 1

uint32_t counter1 = 0;
uint32_t counter2 = 0;
uint32_t counter3 = 0;

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
	
	/* Launching kernel */
	osKernelLaunch(10);
}
void task0(void){
	while(1){
#if USE_LED
		gpio_set_toggle(LED_RED);
#else
		counter1++;
#endif
		
		osKernelYield();
	}
	
}
void task1(void){
	while(1){
#if USE_LED
		gpio_set_toggle(LED_BLUE);
#else
		counter2++;
#endif
		
		osKernelYield();
	}
}
void task2(void){
	while(1){
#if USE_LED
		gpio_set_toggle(LED_RED);
		gpio_set_toggle(LED_BLUE);
#else
		counter3++;
#endif	
		
		osKernelYield();
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