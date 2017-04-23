/*
 * gpio.c
 *
 *  Created on: Dec 10, 2012
 *      Author: B34443
 */

#include "gpio.h"
#

void rgb_init(void)
{
	/* Turn on clock to PortB and PortD module */
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTD_MASK;
	
	/* Set the PTB18 pin multiplexer to GPIO mode */
	PORTB_PCR18 = PORT_PCR_MUX(1);
	
	/* Set the initial output state to high */
	GPIOB_PSOR |= RED_SHIFT;
	
	/* Set the pins direction to output */
	GPIOB_PDDR |= RED_SHIFT;
	
	/* Set the PTB19 pin multiplexer to GPIO mode */
	PORTB_PCR19 = PORT_PCR_MUX(1);
	
	/* Set the initial output state to high */
	GPIOB_PSOR |= GREEN_SHIFT;
	
	/* Set the pins direction to output */
	GPIOB_PDDR |= GREEN_SHIFT;
	
	/* Set the PTD1 pin multiplexer to GPIO mode */
	PORTD_PCR1 = PORT_PCR_MUX(1);
	
	/* Set the initial output state to high */
	GPIOD_PSOR = BLUE_SHIFT;
	
	/* Set the pins direction to output */
	GPIOD_PDDR |= BLUE_SHIFT;
}

