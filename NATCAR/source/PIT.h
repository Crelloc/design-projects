/*
 * PIT.h
 *
 *  Created on: Sep 4, 2013
 *      Author: halsted
 */

/**************** Update ****************/
/* Reginald Lai, Andrew Trinh, Thomas Turner */
/* PIT.h */
//pins defined here

#ifndef PIT_H_
#define PIT_H_
#define BUS_CLOCK 24000000
#define PIT_FREQUENCY   200 //use to be 150


#define pinSI            (1 << 7)

#define pinSI_ON        (GPIOD_PSOR = pinSI)
#define pinSI_OFF         (GPIOD_PCOR = pinSI)
#define pinSI_TOGGLE     (GPIOD_PTOR = pinSI)

#define pinCLK            (1 << 1)

#define pinCLK_ON        (GPIOE_PSOR = pinCLK)
#define pinCLK_OFF         (GPIOE_PCOR = pinCLK)
#define pinCLK_TOGGLE     (GPIOE_PTOR = pinCLK)

#define pinCONV			(1<<2)

#define pinCONV_ON        (GPIOB_PSOR = pinCONV)
#define pinCONV_OFF         (GPIOB_PCOR = pinCONV)
#define pinCONV_TOGGLE     	(GPIOB_PTOR = pinCONV)

#define pinPTB0            (1 << 0)

#define pinPTB0_ON        (GPIOD_PSOR = pinPTB0)
#define pinPTB0_OFF         (GPIOD_PCOR = pinPTB0)
#define pinPTB0_TOGGLE     (GPIOD_PTOR = pinPTB0)

	//LEDs
#define LED0             (1<<8)
#define LED1		     (1<<9)
#define LED2   			 (1<<10)
#define LED3   			 (1<<11)

#define LED0_ON             GPIOB_PSOR = LED0
#define LED0_OFF              GPIOB_PCOR = LED0
#define LED0_TOGGLE          GPIOB_PTOR = LED0

#define LED1_ON             GPIOB_PSOR = LED1
#define LED1_OFF              GPIOB_PCOR = LED1
#define LED1_TOGGLE          GPIOB_PTOR = LED1

#define LED2_ON             GPIOB_PSOR = LED2
#define LED2_OFF              GPIOB_PCOR = LED2
#define LED2_TOGGLE          GPIOB_PTOR = LED2

#define LED3_ON             GPIOB_PSOR = LED3
#define LED3_OFF              GPIOB_PCOR = LED3
#define LED3_TOGGLE          GPIOB_PTOR = LED3

	//LEFT Motor
#define HBA1_ON 				GPIOC_PSOR = (1 << 3)
#define HBA1_OFF				GPIOC_PCOR = (1 << 3)
#define HBA2_ON 				GPIOC_PSOR = (1 << 4)
#define HBA2_OFF				GPIOC_PCOR = (1 << 4)

	//RIGHT Motor
#define HBB1_ON 				GPIOC_PSOR = (1 << 1)
#define HBB1_OFF				GPIOC_PCOR = (1 << 1)
#define HBB2_ON 				GPIOC_PSOR = (1 << 2)
#define HBB2_OFF				GPIOC_PCOR = (1 << 2)

	//H-Bridge 
#define H_Bridge			(1 << 21);
#define H_Bridge_ON			GPIOE_PSOR = H_Bridge
#define H_Bridge_OFF		GPIOE_PCOR = H_Bridge

	//SW1 and SW2
#define SW1        (1<<13)
#define SW2        (1<<17)
	//set pin active low for switches here
#define SW1_P   ((GPIOC_PDIR & SW1)>0)
#define SW2_P   ((GPIOC_PDIR & SW2)>0)

void InitPIT();
//void PIT_IRQHandler();

#endif /* PIT_H_ */

