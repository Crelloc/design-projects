/*Andrew Lejano and Reginald lai */
/* Lab 3: Simulated Linescan Camera Interface */
/* adc0.c*/

#include "Derivative.h"
#include "adc16.h"
#include "PIT.h"

void Init_ADC0(void){
	pp_toggle = 0; //ping-pong toggle
    tADC_Config Master_Adc_Config;
  
    SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );    // Enable ADC0 clock
		// setup the initial ADC default configuration      
    Master_Adc_Config.CONFIG1  = ADLPC_NORMAL
        | ADC_CFG1_ADIV(ADIV_2)
        | ADLSMP_LONG
        | ADC_CFG1_MODE(MODE_8)
        | ADC_CFG1_ADICLK(ADICLK_BUS);

    Master_Adc_Config.CONFIG2  = MUXSEL_ADCB
        | ADACKEN_DISABLED
        | ADHSC_HISPEED
        | ADC_CFG2_ADLSTS(ADLSTS_2) ;

    Master_Adc_Config.COMPARE1 = 0x1234u ;  // can be anything
    Master_Adc_Config.COMPARE2 = 0x5678u ;  // can be anything

    Master_Adc_Config.STATUS2  = ADTRG_SW
        | ACFE_DISABLED
        | ACFGT_GREATER
        | ACREN_DISABLED
        | DMAEN_DISABLED
        | ADC_SC2_REFSEL(REFSEL_EXT);
            
    Master_Adc_Config.STATUS3  = CAL_OFF
        | ADCO_SINGLE
        | AVGE_DISABLED
        | ADC_SC3_AVGS(AVGS_32);
            
    Master_Adc_Config.STATUS1A = AIEN_ON | DIFF_SINGLE |ADC_SC1_ADCH(6)| ADC_SC1_ADCH(13);
              
// Configure ADC as it will be used, but because ADC_SC1_ADCH is 31,
// the ADC will be inactive.  Channel 31 is just disable function.
// There really is no channel 31.
            
    ADC_Config_Alt(ADC0_BASE_PTR, &Master_Adc_Config);  // config ADC
                     
// Calibrate the ADC in the configuration in which it will be used:

        ADC_Cal(ADC0_BASE_PTR);                    // do the calibration
            
// The structure still has the desired configuration.  So restore it.
// Why restore it?  The calibration makes some adjustments to the
// configuration of the ADC.  The are now undone:
            
// config the ADC again to desired conditions

    Master_Adc_Config.CONFIG1  = ADLPC_NORMAL
        | ADC_CFG1_ADIV(ADIV_2)
        | ADLSMP_LONG
        | ADC_CFG1_MODE(MODE_8)
        | ADC_CFG1_ADICLK(ADICLK_BUS);

    Master_Adc_Config.STATUS3  = CAL_OFF    // no hardware averaging
        | ADCO_SINGLE;

    ADC_Config_Alt(ADC0_BASE_PTR, &Master_Adc_Config);
}

void ADC0_IRQHandler() {
    	// read ADC value into local (temporary) variable
    analog_val = ADC0_RA;           
    
    if(counter1 < 128){
    	MUX_B = 0;
      	//assert CLK
    	pinCLK_ON;
			//reset done flag
       	done = 0;
			//collect 128 values for PING buffers
		if(pp_toggle == 0){
		      	//deassert CLK
				pinCLK_OFF;
				ping2[counter1] = analog_val;
				counter1++;
				ADC0_SC1A=AIEN_ON |ADC_SC1_ADCH(7);			

        }
			//or collect 128 values for PONG buffers
        else if(pp_toggle == 1){

				pinCLK_OFF;
				pong2[counter1] = analog_val;
				counter1++;
				ADC0_SC1A=AIEN_ON |ADC_SC1_ADCH(7);
        }
    }
    else if (counter1 >= 128){
			//set done flag
    		MUX_B = 1;
    		//reset counter
			counter1=0;  
			done = 1;
			//deassert Conversion time signal
			pinCONV_OFF;
				//switch buffers
			pp_toggle = !pp_toggle;
    }

}
  
