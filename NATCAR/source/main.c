/**************** Update ****************/
/* Reginald Lai, Andrew Trinh, Thomas Turner */
/* Including Breaking, TFC shield */
/* main.c */

#include "mcg.h"
#include "main.h"
#include "derivative.h"
#include "PIT.h"
#include "adc16.h"
#include "arm_cm0.h"
#include "uart.h"
#include "sysinit.h"
#include "tpm.h"

#define TPM_MODULE 		3750/20 //change this value in order to change PWM output frequency
#define DUTY_MIN		0 // 0% Duty Cycle
#define V_THRESH_1 	    64//near cam  //voltage threshold use to be 47 and 50
#define V_THRESH_2 	    55
#define FB_THRESH		0x17	//current-feedback-that-makes-it-go-faster threshold
#define DUTY_MAX		TPM_MODULE //100% Duty Cycle
#define SLOW			0
#define FAST			1
#define LEFT			0
#define RIGHT			1
//#define DB_LEFT		42
//#define DB_RIGHT		77
#define DB_Near_LEFT	45
#define DB_Near_RIGHT	86
#define DB_Far_LEFT		55
#define DB_Far_RIGHT	77
#define SPEED_THRESH	13
#define RUG_MAX			8 //largest track thickness, old 23
#define RUG_MIN			4 //minimum to detect , old 10

void set_pos(int angle);
void set_speed(int speed, int turn);

void bit_to_hex(int number){
		//Mask 4-LSB
	uint8_t bits = 0b00001111;
		//mask given 8 bit number
	int	mask2 = number & bits;
		//shift 8 bit number to left 4 times
	number = (number >> 4);
		//mask those last 4 numbers
	int mask1 = number & bits;

		//Convert MSB 4 bits to character Hex
	switch(mask1)	{
	case 0:
		put("0");
	break;
	case 1:
		put("1");
	break;
	case 2:
		put("2");
	break;
	case 3:
		put("3");
	break;
	case 4:
		put("4");
	break;
	case 5:
		put("5");
	break;
	case 6:
		put("6");
	break;
	case 7:
		put("7");
	break;
	case 8:
		put("8");
	break;
	case 9:
		put("9");
	break;
	case 10:
		put("A");
	break;
	case 11:
		put("B");
	break;
	case 12:
		put("C");
	break;
	case 13:
		put("D");
	break;
	case 14:
		put("E");
	break;
	case 15:
		put("F");
	break;
	}
		//Convert LSB 4 bits to character Hex
	switch(mask2){
	case 0:
		put("0");
	break;
	case 1:
		put("1");
	break;
	case 2:
		put("2");
	break;
	case 3:
		put("3");
	break;
	case 4:
		put("4");
	break;
	case 5:
		put("5");
	break;
	case 6:
		put("6");
	break;
	case 7:
		put("7");
	break;
	case 8:
		put("8");
	break;
	case 9:
		put("9");
	break;
	case 10:
		put("A");
	break;
	case 11:
		put("B");
	break;
	case 12:
		put("C");
	break;
	case 13:
		put("D");
	break;
	case 14:
		put("E");
	break;
	case 15:
		put("F");
	break;
	}
	put(" ");
}


int main(void){
	counter1=0;
	/*unsigned*/ int i;
	int j;
	int middle2 = 0;
	int steer;
	int ServoCounter = 48;
	int wheel2, m;
	int LR_flag=0; 	// Left/Right flag, tells what to do when camera sees white&white. 0=left, 1=right
	int old_servo=0;
	int CONT_FLAG = 0;
	int steep_count=0;
	
	/*slope method variables*/
	
	int slope_pmin2=0;
	int slope_pmax2=0;
	int slope_nmin2=0;
	int slope_nmax2=0;
	int first_p2=0;
	int first_n2=0;
	
	
	SLOWDOWN = 0;
	SLOWDOWN_COUNTER =0;
	STOP_COUNTER=0;
	//initialize all global variables
	//BRAKE =0;
	//BRAKE_COUNTER=0;
	d_control_init = 0;
	d_control_done = 0;
	white_check = 0;		//initialize motor values in PIT
	PIT_COUNTER = 0;
	PIT_RESET=0;
	STOP=0;
	STOP_FLAG=0;
	flag=0;
	MUX_B = 1;
	TOFCount=0;
	input_capture_avg=0;
	avg=0;
	avg_flag=0;
	speed_sensor_motor=0;
	ModNum = 0xFFFF;
	sysinit();
        
		//First order of business is to enable the Clocks to the ports!
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK |SIM_SCGC5_PORTE_MASK|SIM_SCGC5_PORTC_MASK ;

	    /* Set the initial SI state to low */
	GPIOD_PCOR = pinSI;
	    /* Set the initial Clock state to low */
	GPIOE_PCOR = pinCLK;
	    /* Set the initial Conversion state to low */
	GPIOB_PCOR = pinCONV;
	                       
	    //Now,  setup the port mux for GPIO! See Page 163 and 183 of KL25 Sub-Family Reference Manual
	PORTD_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTE_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTB_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTD_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTD_PCR5 = PORT_PCR_MUX(0) | PORT_PCR_DSE_MASK;  	//camera1 AO pin
	PORTE_PCR21= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;	//H_Bridge Enabler
	PORTC_PCR13 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;	//SW1
	PORTC_PCR17 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;	//SW2
	PORTD_PCR6 = PORT_PCR_MUX(0) | PORT_PCR_DSE_MASK;  	//camera2 AO pin 

	
	PORTA_PCR1 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK; //Speed sensor using TPM2_CH0
	
	    //Set the GPIO to outputs in the data direction register
	    //See Page 778 of KL25 Sub-Family Reference Manual, Rev. 3, September 2012
	GPIOB_PDDR = pinCONV;
	GPIOD_PDDR = pinSI;
	GPIOE_PDDR = pinCLK;
    
		// Initialize ADC, PIT and enable interrupts           
    Init_ADC0();
    InitPIT();
    
    
    	//add ports for servo and LEDs
	PORTB_PCR0 =  PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
	
	PORTB_PCR8 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; 
	PORTB_PCR9 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTB_PCR10 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTB_PCR11 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	
	GPIOB_PCOR |= pinPTB0 | LED0 | LED1 | LED2 | LED3;
	GPIOB_PDDR |= pinPTB0 | LED0 | LED1 | LED2 | LED3;

	H_Bridge_OFF;
	
		//set TPM values like in part 1B
	TPM_init_PWM(TPM1_BASE_PTR, TPM_PLLFLL, 3750, TPM_CLK, PS_128, EDGE_PWM);
 
	TPM_CH_init(TPM1_BASE_PTR, 0, TPM_PWM_H);	//servo

	enable_irq(INT_TPM2 -16);	//enable TPM2 interrupts
    EnableInterrupts;
	
	//Speed Sensor Config
	TPM_init_PWM(TPM2_BASE_PTR, TPM_PLLFLL, ModNum, TPM_CLK, PS_4, EDGE_PWM);
	TPM_CH_init(TPM2_BASE_PTR, 0, 0b01001000); //CHIE and ELSB are set to 1
	TPM_SC_REG(TPM2_BASE_PTR) |=  0b11000000;	 //TOF set to 1, set TOIE
	TPM_SC_REG(TPM2_BASE_PTR) &=  0b11011111;	//CPWMS set to 0

	
	set_TPM_CnV(TPM1_BASE_PTR, 0, 278);	//setting Servo TPM
	

	
	//	size = sizeof(i); //i is unsigned
	//bit_to_hex(size);
	put("\r\n Turn on Power and Press SW2!\r\n");
	while( !SW2_P ){
		//waiting for SW2 to be pressed
	}
	//***ENABLE H-BRIDGES******//
	PORTC_PCR3 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	//H-BridgeA IN1
	PORTC_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;	//H-BridgeA IN2
	
	PORTC_PCR1 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	//H-BridgeB IN1
	PORTC_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;	//H-BridgeB IN2	
	
	TPM_init_PWM(TPM0_BASE_PTR, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
	TPM_CH_init(TPM0_BASE_PTR, 2, TPM_PWM_H);	//H-bridgeA IN1
	TPM_CH_init(TPM0_BASE_PTR, 0, TPM_PWM_H);	//H-bridgeB IN1
	HBB2_OFF;	//Setting H-BridgeA and H-BridgeB IN2 as GPIO low
	HBA2_OFF;   
	GPIOC_PDDR |= (1 <<4) | (1 <<2);	//setting GPIO signals to outputs

	GPIOE_PDDR |= (1 << 21);			//H-Bridge enabler set as output
	
//	PORTE_PCR22= PORT_PCR_MUX(0) | PORT_PCR_DSE_MASK;	//H_Bridge B IFB
//	PORTE_PCR23= PORT_PCR_MUX(0) | PORT_PCR_DSE_MASK;	//H_Bridge A IFB
	
	put("SET DUTY CYCLE!!\r\n");
		//Set H-Bridge
	H_Bridge_ON;
	
	//unsigned int INIT_ADC0_CFG2 = ADC0_CFG2; 
	ADC0_CFG2 &= !MUXSEL_ADCB;
		//If SW1 is not pressed yet

	while(!SW1_P){
		
			//Read ADC values from POT1
		//switch over to MUX A to read other feedback values
		
		ADC0_SC1A = 13;
//		put("POT_NUM: ");
//		bit_to_hex(POT_NUM);
//		put("\r\n");
		while ( !(ADC0_SC1A & 0x80u) ){/*Do nothing until coco*/}
		
		POT_NUM = ADC0_RA;		
	

			//Display last 4 bits on LEDs
		if (POT_NUM & 0b10000000){
			LED0_ON;
		}
		else 
			LED0_OFF;
		if (POT_NUM & 0b01000000){
					LED1_ON;
		}
		else 
			LED1_OFF;
		if (POT_NUM & 0b00100000){
					LED2_ON;
		}
		else 
			LED2_OFF;
		if (POT_NUM & 0b00010000){
					LED3_ON;
		}
		else 
			LED3_OFF;
			//convert and display Bits
			//Set motors to run at POT value
		set_TPM_CnV(TPM0_BASE_PTR, 0, POT_NUM);
		set_TPM_CnV(TPM0_BASE_PTR, 2, POT_NUM);
		
//		put("\r\n");
//		put("Capture Avg: ");
//		bit_to_hex(input_capture_avg);
//		put("		Current Speed: ");
//		bit_to_hex(input_capture[avg]);
		
	}
		//Press SW1 and switches to Camera/ Servo
	put("SW1 WAS PRESSED!!");
	
//	ADC0_CFG2 = INIT_ADC0_CFG2;
	
	
    enable_irq(INT_ADC0 -16);
    enable_irq(INT_PIT -16);
    EnableInterrupts;
	LED0_OFF;
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	
    while(TRUE && !STOP) {
    	if (done && pp_toggle == 0){
			//Check if done collecting data in PING buffer
            	for (i = 0 ; i <= 127; i++){
            		if (i > 19 && i <= 107){
            			slope2[i] = ((ping2[i+1]- ping2[i-1]) >> 1);
            		}
					else{
						slope2[i] = 0;
					}
					
					if (slope2[i] > SLOPE_THRESHOLD2){
						if ( first_p2 == 0){
							first_p2=1;
							slope_pmin2=i;
						}
						slope_pmax2 = i;
						steep_count++;
					}
					
					else if(first_p2){
						if ( (slope2[i]) < ~SLOPE_THRESHOLD2 ){
						  if (first_n2==0){
							  slope_nmin2 = i;
							  first_n2 = 1;
						  }
						  slope_nmax2=i;
						  steep_count++;

						}
					}
		}
            	
		//calculate middle of track using slope method

		if (slope_pmin2 == 0 && slope_nmin2 == 0)
			middle2 =0;
		else if(slope_pmin2 == 0 || slope_nmin2 == 0)
			middle2 = (slope_pmin2 + slope_pmax2 + slope_nmin2 + slope_nmax2)>>1;
		else
			middle2 = (slope_pmin2 + slope_pmax2 + slope_nmin2 + slope_nmax2)>>2;
		
				
//		put("\r\nbeginning of array!\n\r");
//			for (j=19; j<=107; j++){
//					bit_to_hex(j);
////					put("|");
////					bit_to_hex(slope1[j]);
//					put("|");
//					bit_to_hex(slope2[j]);
//					put("  ");
//					if ( (j%8) ==0 )
//					  put("\r\n");
//		}
 
		
//		put("\r\n");
////		put("lowcam:  ");
////		bit_to_hex(middle);
//		put("   middle:  ");
//		bit_to_hex(middle2);
//		
////		put("\r\n");
//		put("   pmin:  ");
//		bit_to_hex(slope_pmin2);
//		put("   pmax:  ");
//		bit_to_hex(slope_pmax2);
//		put("nmin:  ");
//		bit_to_hex(slope_nmin2);
//		put("   nmax:  ");
//		bit_to_hex(slope_nmax2);
//		put("   steep_count: ");
//		bit_to_hex(steep_count);
		
		if (CONT_FLAG){
			if (LR_flag){
				if (middle2)
					CONT_FLAG=0;
				else {
					middle2 = 110;
				}
				//hard right turn
			}
			else{
				if (middle2)
					CONT_FLAG=0;
				else {
					middle2 = 20;
				}
				//hard left turn
			}
		}
	

		if (middle2 ==0)
			wheel2 = 0b00;
		else if (middle2 <DB_Far_LEFT)
			wheel2= 0b10;
		else if (middle2 > DB_Far_RIGHT)
			wheel2 = 0b01;
		else if (middle2 >= DB_Far_LEFT && middle2 <= DB_Far_RIGHT)
			wheel2 = 0b11;	

		steer = wheel2;
		m=middle2;
	
		if (steer == 0b00){		//seeing white, stopping
			CONT_FLAG = 1;
			if (LR_flag){		//on a right turn
				if (input_capture_avg < SPEED_THRESH){
					set_pos(old_servo-15);
					set_speed(SLOW,RIGHT);
				}
				else 
					set_speed(FAST,RIGHT);
			}
			else if (! LR_flag){
				if (input_capture_avg < SPEED_THRESH){
					set_pos(old_servo+15);
					set_speed(SLOW,LEFT);
				}
				else 
					set_speed(FAST,LEFT);
			}
		}
		else if (steer == 0b01){	//going right
			flag=0;
			white_check=0;
			MIDDLEF=0;
			//ServoCounter = (48 + DB_Far_RIGHT)-(m*(1+(m-DB_Far_RIGHT)/40));
			ServoCounter = 48 + (1.0)*(DB_Far_RIGHT-m);
			set_pos(ServoCounter);
			if (input_capture_avg < SPEED_THRESH)	//causes breaking on turns
				set_speed(SLOW,RIGHT);
			else
				set_speed(FAST,RIGHT);
			if ( !CONT_FLAG )
				LR_flag=1;
			old_servo=ServoCounter;
		}
		else if (steer == 0b10){		//going left
			flag=0;
			white_check=0;
			MIDDLEF=0;
			//ServoCounter = DB_Far_LEFT*(m-DB_Far_LEFT)/(DB_Far_LEFT - DB_Far_RIGHT)+48;
			ServoCounter = 48+(1.0)*(DB_Far_LEFT-m);
			set_pos(ServoCounter);
			if (input_capture_avg < SPEED_THRESH)	//causes breaking on turns
				set_speed(SLOW,LEFT);
			else
				set_speed(FAST,LEFT);
			if ( !CONT_FLAG )
				LR_flag=0;
			old_servo=ServoCounter;
		}
		else if (steer == 0b11){	//going middle
			MIDDLEF=1;
			flag=0;
			white_check=0;
			ServoCounter = ((62-m)>>1)+48;
			set_pos(ServoCounter);
			HBB2_OFF;	//Setting H-BridgeA and H-BridgeB IN2 as GPIO low
			HBA2_OFF;
			if(SLOWDOWN){
//				set_speed(SLOW,3);
			}
			else
			{
				set_TPM_CnV(TPM0_BASE_PTR, 0, POT_NUM);
				set_TPM_CnV(TPM0_BASE_PTR, 2, POT_NUM);
			}
		}
		else{
			put("\r\nSkipping!");
		}
			
		steep_count=0;
		max = 0;
		min = 0xFF;

		slope_pmin2=0;
		slope_pmax2=0;
		slope_nmin2=0;
		slope_nmax2=0;
		first_p2=0;
		first_n2=0;

					
    
    	}
			//Same functions as lines 273-406 but with PONG buffer
    	else if (done && pp_toggle == 1){
//    		put("\r\nbeginning of array!\n\r");
//			for (j=20; j<=107; j++){
//					bit_to_hex(j);
//					put("|");
//					bit_to_hex(slope1[j]);
//					put("|");
//					bit_to_hex(slope2[j]);
//					put("  ");
//				if ( (j%8) ==0 )
//					 put("\r\n");
//			}
			
    		for (i = 0 ; i <= 127; i++){
            		//convert data values from ping to 0s and 1s
            		//ignore first and last 20 values in buffer
            		if (i > 19 && i <= 107){
            			slope2[i] = ((pong2[i+1]- pong2[i-1]) >> 1);
            		}
			else{
				slope2[i] = 0;
			}
			
			if (slope2[i] > SLOPE_THRESHOLD2){
				if ( first_p2 == 0){
					first_p2=1;
					slope_pmin2=i;
				}
				slope_pmax2 = i;
				steep_count++;
			}
			
			else if(first_p2){
				if ( (slope2[i]) < ~SLOPE_THRESHOLD2 ){
				  if (first_n2==0){
					  slope_nmin2 = i;
					  first_n2 = 1;
				  }
				  slope_nmax2=i;
				  steep_count++;

				}
			  
			}

		}
		//calculate middle of track using slope method
		if (slope_pmin2 == 0 && slope_nmin2 == 0)
			middle2 =0;
		else if(slope_pmin2 == 0 || slope_nmin2 == 0)
			middle2 = (slope_pmin2 + slope_pmax2 + slope_nmin2 + slope_nmax2)>>1;
		else
			middle2 = (slope_pmin2 + slope_pmax2 + slope_nmin2 + slope_nmax2)>>2;
		
//		put("\r\nbeginning of array!\n\r");
//			for (j=19; j<=107; j++){
//					bit_to_hex(j);
//					put("|");
//					bit_to_hex(slope2[j]);
//					put("  ");
//					if ( (j%8) ==0 )
//					  put("\r\n");
//		}
//		put("\r\n");
////		put("lowcam:  ");
////		bit_to_hex(middle);
//		put("   middle:  ");
//		bit_to_hex(middle2);
		
////		put("\r\n");
//		put("   pmin:  ");
//		bit_to_hex(slope_pmin2);
//		put("   pmax:  ");
//		bit_to_hex(slope_pmax2);
//		put("nmin:  ");
//		bit_to_hex(slope_nmin2);
//		put("   nmax:  ");
//		bit_to_hex(slope_nmax2);
//		put("   steep_count: ");
//		bit_to_hex(steep_count);
		
		if (CONT_FLAG){
			if (LR_flag){
				if (middle2)
					CONT_FLAG=0;
				else {
					middle2 = 110;
				}
				//hard right turn
			}
			else{
				if (middle2)
					CONT_FLAG=0;
				else {
					middle2 = 20;
				}
				//hard left turn
			}
		}

		if (middle2 ==0)
			wheel2 = 0b00;
		else if (middle2 <DB_Far_LEFT)
			wheel2= 0b10;
		else if (middle2 > DB_Far_RIGHT)
			wheel2 = 0b01;
		else if (middle2 >= DB_Far_LEFT && middle2 <= DB_Far_RIGHT)
			wheel2 = 0b11;	
		
		steer = wheel2;
		m = middle2;
		
		if (steer == 0b00){		//seeing white, stopping
			CONT_FLAG = 1;
			if (LR_flag){		//on a right turn
				if (input_capture_avg < SPEED_THRESH){
					set_pos(old_servo-15);
					set_speed(SLOW,RIGHT);
				}
				else 
					set_speed(FAST,RIGHT);
			}
			else if (! LR_flag){
				if (input_capture_avg < SPEED_THRESH){
					set_pos(old_servo+15);
					set_speed(SLOW,LEFT);
				}
				else 
					set_speed(FAST,LEFT);
			}
		}
		else if (steer == 0b01){	//going right
			flag=0;
			white_check=0;
			MIDDLEF=0;
			//ServoCounter = (48 + DB_Far_RIGHT)-(m*(1+(m-DB_Far_RIGHT)/40));
			ServoCounter = 48+ (1.0)*(DB_Far_RIGHT-m);
			set_pos(ServoCounter);
			if (input_capture_avg < SPEED_THRESH)	//causes breaking on turns
				set_speed(SLOW,RIGHT);
			else
				set_speed(FAST,RIGHT);
			if ( !CONT_FLAG )			
				LR_flag=1;
			old_servo=ServoCounter;
		}
		else if (steer == 0b10){		//going left
			flag=0;
			white_check=0;
			MIDDLEF=0;
			//ServoCounter = DB_Far_LEFT*(m-DB_Far_LEFT)/(DB_Far_LEFT - DB_Far_RIGHT)+48;
			ServoCounter = 48 +(1.0)*(DB_Far_LEFT-m);
			set_pos(ServoCounter);
			if (input_capture_avg < SPEED_THRESH)	//causes breaking on turns
				set_speed(SLOW,LEFT);
			else
				set_speed(FAST,LEFT);
			if ( !CONT_FLAG )
				LR_flag=0;
			old_servo=ServoCounter;
			
		}
		else if (steer == 0b11){	//going middle
			MIDDLEF=1;
			flag=0;
			white_check=0;
			ServoCounter = ((62-m)>>1)+48;
			set_pos(ServoCounter);
			HBB2_OFF;	//Setting H-BridgeA and H-BridgeB IN2 as GPIO low
			HBA2_OFF;
			if(SLOWDOWN){
//				set_speed(SLOW,3);
			}
			else
			{
				set_TPM_CnV(TPM0_BASE_PTR, 0, POT_NUM);
				set_TPM_CnV(TPM0_BASE_PTR, 2, POT_NUM);
			}
		}
		else{
			put("\r\nSkipping!");
		}

		steep_count=0;
		max = 0;
		min = 0xFF;
		
		slope_pmin2=0;
		slope_pmax2=0;
		slope_nmin2=0;
		slope_nmax2=0;
		first_p2=0;
		first_n2=0;
	}
    }
    
    for ( i=0 ; i <100000; i++){
    	set_speed(FAST,3);	//stopping after finish line	
    }
	H_Bridge_OFF;

	while(1){}
    return 0;
}

void set_pos(int angle)
{
	int duty = 0;
	if (angle < 12)
		angle=12;
	else if (angle > 85)
		angle = 85;
		//To get rid of the floating points
	duty = (angle*177)/100 + 195;
	set_TPM_CnV(TPM1_BASE_PTR, 0, duty);
}

void set_speed(int speed, int turn){
	if (turn == 1){
		if (speed){		//RIGHT TURN
			HBB2_OFF;	//Setting Hp-BridgeA and H-BridgeB IN2 as GPIO low
			HBA2_OFF;
			set_TPM_CnV(TPM0_BASE_PTR, 0, POT_NUM * 7 / 10);
			set_TPM_CnV(TPM0_BASE_PTR, 2, POT_NUM * 9 / 10);
		}
		else {
			//slower breaking

			set_TPM_CnV(TPM0_BASE_PTR, 0, POT_NUM/3);
			set_TPM_CnV(TPM0_BASE_PTR, 2, POT_NUM/2);
		}
		
			
	}
	else if(turn ==0) {		//LEFT TURN
		if (speed){
			HBB2_OFF;	
			HBA2_OFF;
			set_TPM_CnV(TPM0_BASE_PTR, 0, POT_NUM * 9 / 10);
			set_TPM_CnV(TPM0_BASE_PTR, 2, POT_NUM * 7 / 10);
		}
		else {
			//slower breaking

			set_TPM_CnV(TPM0_BASE_PTR, 0, POT_NUM/2);
			set_TPM_CnV(TPM0_BASE_PTR, 2, POT_NUM/3);
		}
	
	}
	else{
			set_TPM_CnV(TPM0_BASE_PTR, 0, 0);
			set_TPM_CnV(TPM0_BASE_PTR, 2, 0);
			LED0_ON;
			LED1_ON;
			LED2_ON;
			LED3_ON;
	}
	

}
