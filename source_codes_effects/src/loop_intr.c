// loop_intr.c 

#include "platform.h"
#include "delay.h"
#include "gpio.h"


int main(void)
{

	gpio_set_mode(LED1,Output);
 	
	
  while(1)
	{
	
	gpio_toggle(LED1);
	
	delay_ms(100);
	
	}
}