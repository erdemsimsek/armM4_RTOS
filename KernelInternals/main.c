#include <stdint.h>
#include "msp.h"

enum LEDs{
	LED_RED	= 0,
	LED_BLUE,
	
	LED_MAX
};

void DelayS(uint32_t seconds);

void gpio_set_output(enum LEDs which_led);
void gpio_set_toggle(enum LEDs which_led);

volatile uint32_t tick;
volatile uint32_t _tick;

uint32_t red_stack[40];
uint32_t blue_stack[40];

uint32_t * sp_red = &red_stack[40];
uint32_t * sp_blue = &blue_stack[40];

int main_red_led(void)
{
	gpio_set_output(LED_RED);
	
	while(1){
		gpio_set_toggle(LED_RED);
		DelayS(1u);
	}
}

int main_blue_led(void)
{
	gpio_set_output(LED_BLUE);
	
	while(1){
		gpio_set_toggle(LED_BLUE);
		DelayS(1u);
	}
}

int main(void)
{
	/* Stop watchdog timer */
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;           
	
	/* Update system clock */
	SystemCoreClockUpdate();
	
	/* Systick interrupt each 1 second */
	SysTick_Config(SystemCoreClock / 100U); 

	/* Enable interrupts */
	__enable_interrupt();

	/* Initilization of Stack Frame for RED LED Task */
	/* xPSR for red LED task */
	*(--sp_red) = (1U << 24);
	/* Setting PC */
	*(--sp_red) = (uint32_t)&main_red_led;
	/* LR */
	*(--sp_red) = 0x0000000DU;
	/* R12 */
	*(--sp_red) = 0x0000000EU;
	/* R3 */
	*(--sp_red) = 0x0000000AU;
	/* R2 */
	*(--sp_red) = 0x0000000EU;
	/* R1 */
	*(--sp_red) = 0x0000000AU;
	/* R0 */
	*(--sp_red) = 0x0000000DU;
	
	/* Initilization of Stack Frame for BLUE LED Task */
	/* xPSR for blue LED task */
	*(--sp_blue) = (1U << 24);
	/* Setting PC */
	*(--sp_blue) = (uint32_t)&main_blue_led;
	/* LR */
	*(--sp_blue) = 0x0000000BU;
	/* R12 */
	*(--sp_blue) = 0x0000000EU;
	/* R3 */
	*(--sp_blue) = 0x0000000EU;
	/* R2 */
	*(--sp_blue) = 0x0000000EU;
	/* R1 */
	*(--sp_blue) = 0x0000000EU;
	/* R0 */
	*(--sp_blue) = 0x0000000FU;
	
	while(1)
	{
	}
}

void gpio_set_output(enum LEDs which_led)
{
	if(LED_RED == which_led){
		/* Configure P1.0 as output  LED Port 1 - RED*/
		P1->DIR |= BIT0;                      
	}
	
	if(LED_BLUE == which_led){
		/* Configure P2.2 as output  LED Port 2 - BLUE*/
		P2->DIR |= BIT2;
	}
}

void gpio_set_toggle(enum LEDs which_led)
{
	if(LED_RED == which_led){
		/* Configure P1.0 as output  LED Port 1 - RED*/
		P1->OUT ^= BIT0;                      
	}
	
	if(LED_BLUE == which_led){
		/* Configure P2.2 as output  LED Port 2 - BLUE*/
		P2->OUT ^= BIT2;                      
	}

}

void SysTick_Handler(void)
{
	++tick;
}

uint32_t getTick(void)
{
	__disable_interrupt();
	_tick = tick;
	__enable_interrupt();
	
	return _tick;
}

void DelayS(uint32_t seconds)
{
	seconds *= 100;
	uint32_t temp = getTick();
	while((getTick() - temp) < seconds);
}