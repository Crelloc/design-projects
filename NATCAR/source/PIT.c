/*Andrew Lejano and Reginald lai */
/* Lab 3: Simulated Linescan Camera Interface */
/* PIT.c*/

#include "Derivative.h"
#include "PIT.h"
#include "adc16.h"
#include "arm_cm0.h"
#include "tpm.h"


int value;

void InitPIT() {
        //Enable clock to the PIT
    SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
        //enable PIT0 and its interrupt
    PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;      
	    // Determine LDVAL for specific sampling rate
    PIT_LDVAL0 = BUS_CLOCK/PIT_FREQUENCY;
	 // stop the pit when in debug mode
//  PIT_MCR |= PIT_MCR_FRZ_MASK;
        //Enable the PIT module
    PIT_MCR &= ~PIT_MCR_MDIS_MASK;                      
}

void PIT_IRQHandler() {
        //assert SI signal
	pinSI_ON;
		// set GPIO signal high
	pinCONV_ON;
		//assert CLK signal
	pinCLK_ON;
		//Turn off the Pit 0 Irq flag
	PIT_TFLG0 = PIT_TFLG_TIF_MASK;
		//deassert SI signal
	pinSI_OFF;
		//deassert CLK
	pinCLK_OFF;
	
	if(MUX_B){
		ADC0_CFG2 |= MUXSEL_ADCB;
			// Start A/D conversion by writing to ADC0_SC1A 
		ADC0_SC1A=AIEN_ON|7; //0x80 
	}
	else
		put("\r\nPIT is too short!!!");
	
	if(MIDDLEF){SLOWDOWN_COUNTER++;}
	else{
		SLOWDOWN=0;
		SLOWDOWN_COUNTER =0;
	}
	if(SLOWDOWN_COUNTER == 250)	{
			SLOWDOWN = 1;
			
	}
	if(SLOWDOWN_COUNTER > 300){
		SLOWDOWN=0;
		SLOWDOWN_COUNTER=0;
	}
	
	//Stopping at the finish line 
	if (STOP_FLAG){
		STOP_COUNTER++;
		if (STOP_COUNTER>40){
			STOP=1;
		}
	
	}
	
	
}
