/*
 * tpm.c
 *
 *  Created on: Jul 16, 2013
 *      Author: B34443
 */
#define RED_LED_TOGGLE  	GPIOB_PTOR = RED_LED_LOC
#define RED_LED_LOC		(1<<18)
#define TPM_MODULE  3750 //change this value in order to change PWM output frequency
#define DUTY_MIN	195 //originally 105
#define DUTY_MAX	362 //originally 425
#include "main.h"
#include "adc16.h"
volatile int counter = DUTY_MIN;
volatile int counter2 = 0;
volatile int goUp=1;
void TPM_init_PWM(TPM_MemMapPtr TPMx, int  clock_source, int module, int clock_mode, int ps, int counting_mode)
{
	if(TPMx == TPM0_BASE_PTR)
		SIM_SCGC6	|= SIM_SCGC6_TPM0_MASK;
	else if(TPMx == TPM1_BASE_PTR)
		SIM_SCGC6	|= SIM_SCGC6_TPM1_MASK;
	else
		SIM_SCGC6	|= SIM_SCGC6_TPM2_MASK;
	
	SIM_SOPT2	|= SIM_SOPT2_TPMSRC(clock_source);
	
	TPM_MOD_REG(TPMx)  	=  module;
	
	TPM_SC_REG(TPMx)	|= TPM_SC_CMOD(clock_mode) | TPM_SC_PS(ps);
	
	if(counting_mode)
		TPM_SC_REG(TPMx) |= TPM_SC_CPWMS_MASK;
}

void TPM_CH_init(TPM_MemMapPtr TPMx, int channel, int mode)
{
	TPM_CnSC_REG(TPMx, channel) |= mode;
}

void set_TPM_CnV(TPM_MemMapPtr TPMx, int channel, int value)
{
	TPM_CnV_REG(TPMx, channel) = value;
}

void FTM0_IRQHandler()
{
	put("In FTM0 IRQ handler!\r\n");
}

void FTM2_IRQHandler(){
	//++TOFCount;
	if(TPM2_SC & 0x80u){
		//put("\r\nIn FTM2 IRQ Handler!\r\n");
		//TPM2_STATUS |= 0b100000000;
		TPM2_SC|=0b10000000;
		++TOFCount;
	}
	else if(TPM2_STATUS & 0x1u){
		TPM2_CnSC(0)|=0b10000000; //clears the interrupt requestion flag;
		TPM2_CNT=0x0000; //Writing to TPM2_CNT clears the CNT register 
		input_capture[avg] = (TOFCount * (1+ModNum) ) + TPM2_C0V;
		avg++;
		if (avg == 4)
			avg=0;
		input_capture_avg = input_capture[0]+input_capture[1]+input_capture[2]+input_capture[3];
		input_capture_avg = input_capture_avg >> 14;
		TOFCount=0;
		avg_flag=1;
	}

}


