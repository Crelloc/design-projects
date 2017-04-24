//REMOTE CODE 1046
#include "inc/lm3s8962.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "driverlib/systick.c"
#include "driverlib/uart.h"

//#include "uart_echo.c"


#define total_bits 25
#define bit_rate    1175            
//1175

/*These are the precalculated integer values of each button press from the IR remote. Commented out are the commpressed bit stream for each button. 
*/
const int VOLup = 8050771;  // USED to ignore text written on screen if sending text from sparkcore to stellaris    //{3,3,1,1,3,1,3,1,1,1,1,3,3,1,3,3,1,3,1,3,3,3,3,1,1}
const int CHup = 24823891;  //delimiter to send text to spark                           //{3,3,1,1,3,1,3,1,1,1,1,3,1,1,3,3,1,3,1,3,3,3,3,1,3}
const int zero = 11719891;                          //{3,3,1,1,3,1,3,3,1,1,3,1,3,1,3,3,1,3,1,1,3,3,1,3,1};
const int one  = 16961491;                            //{3,3,1,1,3,1,3,3,3,3,3,3,1,1,3,3,1,3,1,1,1,1,1,1,3};
const int two  = 15913171;                      //{3,3,1,1,3,1,3,3,1,1,1,1,3,1,3,3,1,3,1,1,3,3,3,3,1};
const int three= 15389011;                          //{3,3,1,1,3,1,3,1,3,1,1,1,3,1,3,3,1,3,1,3,1,3,3,3,1};
const int four = 14864851;                      //{3,3,1,1,3,1,3,3,3,1,1,1,3,1,3,3,1,3,1,1,1,3,3,3,1};
const int five = 14340691;                        //{3,3,1,1,3,1,3,1,1,3,1,1,3,1,3,3,1,3,1,3,3,1,3,3,1};
const int six  = 13816531;                              //{3,3,1,1,3,1,3,3,1,3,1,1,3,1,3,3,1,3,1,1,3,1,3,3,1};
const int seven= 13292371;                              //{3,3,1,1,3,1,3,1,3,3,1,1,3,1,3,3,1,3,1,3,1,1,3,3,1};
const int eight= 12768211;                              //{3,3,1,1,3,1,3,3,3,3,1,1,3,1,3,3,1,3,1,1,1,1,3,3,1};
const int nine = 12244051;                              //{3,3,1,1,3,1,3,1,1,1,3,1,3,1,3,3,1,3,1,3,3,3,1,3,1};
const int del  = 19058131;                              //{3,3,1,1,3,1,3,3,3,1,3,3,1,1,3,3,1,3,1,1,1,3,1,1,3};

/* The SysTick_Handler is called periodically to sample incomming data from the IR remote. pc5 is the bit being read into the microprocessor. The handler continuously checks for the input bit and once it has enough information to decipher which button was pressed, it sends a done flag to main().*/

void SysTick_Handler(void);

/*init_GPIO initializes several GPIO pins that are commmonly used on the microprocessor board. */

void init_GPIO(void);

int button_sum;
int array_count; //tells what element in the array we are inputting
int assert_count;
int GENERATE = 0;
int pc5;
int done;
int z,y;
int counter;
int hold,next;
int row=0;
int column=0;
int good;
int num_char;
int print_num_char;
unsigned char character[100];
unsigned char checksum;
unsigned char endlength;
char incoming_text[100];
int printc;
int char_index=0;
int incoming_size=0;
char xxx;
char *xx;

/* draw_num takes in an integer, and prints out the corresponding character based on the duration of the button press. row and column decide where on the OLED the character is printed */

void draw_num(int number, int row, int column );

void init_uart();

volatile int flag=0;

int main(){
    z=0;
    y=0;
    counter =0;
    init_GPIO();
    RIT128x96x4Init(1000000);
        RIT128x96x4DisplayOn();
    RIT128x96x4Clear();
        
    //set clock to 50MHz
        SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
            
      SysTickPeriodSet(50000000/(2*bit_rate) );
        SysTickEnable();
    SysTickIntEnable();
    
        init_uart();

        
        num_char=0; //used to num_char through character array when sending to xbee
        RIT128x96x4StringDraw("Send text:", 8*(column), 8*row, 15);
            row++;
            column=0;
            button_sum = 0;
            array_count = 0;
            done = 0;
            hold=0;
            next =0;
            good=0;
        while (1){
            if(done){
                if (button_sum == del){
                    good=1;
                    num_char = num_char-2;
                    RIT128x96x4StringDraw(" ", 8*(column-1), 8*row, 15);
                    if (column==0){
                        column=14;
                    }
                    else
                        column = column-2;
                }
                
                else if(button_sum==VOLup){
                    
                    num_char=0;
                    RIT128x96x4Clear();                
                }
                else if (button_sum == CHup){//when delimiter is pressed send transmit packet to UART1 1 byte at a time

                    UARTCharPut(UART1_BASE, 0x7E); //delimiter
                    UARTCharPut(UART1_BASE, 0x00); //length start
                    
                    endlength = (num_char+5) & 0xFF; //num_char contains total number of character to send inside message.  you add 6 because we are taking into account total number of bytes-1 (API identifier, API Frame ID, MSB des, LSB des, Option, checksum. subtract 1 because the first value is index 0 (length start))
                    //endlength is masked with 0xFF to get byte value
                    
                    UARTCharPut(UART1_BASE, endlength); //length end
                    UARTCharPut(UART1_BASE, 0x01); //API identifier
                    UARTCharPut(UART1_BASE, 0x00); //API Frame ID
                    UARTCharPut(UART1_BASE, 0x00); //Destination Address to xbee connected to sparkcore
                    UARTCharPut(UART1_BASE, 0x01);
                    UARTCharPut(UART1_BASE, 0x01); //Disable ACK, Option
                    
                    checksum = 0x03;  //initialize checksum (add API indentifier, API Frame ID, MSByte of Des. address, LSByte of Des. address, Option)

                    for(print_num_char=0; print_num_char<num_char; print_num_char++){
                        
                        
                        UARTCharPut(UART1_BASE, character[print_num_char]); //transmit character of message to UART1
                        
                        checksum = checksum + character[print_num_char];  //this will add hex values. the end result will be truncated  to a 8 bit (byte) value automatically
                        
                    }
                    
                    checksum = 0xFF - checksum; //compute checksum
                    UARTCharPut(UART1_BASE, checksum); //send checksum through UART1 to XBee
                    
                    num_char = 0; //reset character count
                    
                    //RIT128x96x4Clear();//reset LED
                    row++;
                    column=0;
                    RIT128x96x4StringDraw("Send text:", 8*(column), 8*row, 15);
                    row++;
                    column=0;
                }
                else if (button_sum == one){
                    good=1;
                    RIT128x96x4StringDraw("1", 8*column, 8*row, 15);
                    character[num_char] = '1';
                }
        
                if (good){
                                    num_char++;
                    column++;
                            }
                if (column > 15){
                    row++;
                    column=0;
                }
                
                
            button_sum = 0;
            array_count = 0;
            done = 0;
            hold=0;
            next =0;
            good=0;
            }
            
            
            if(flag){
                        
                    RIT128x96x4Clear();
                    column=0; row=0;
                    RIT128x96x4StringDraw("Incoming text:", 8*column, 8*row , 15);
                    row++;
                    column=0;
                
                xx = &incoming_text[printc+8];
                RIT128x96x4StringDraw(xx, 8*column, 8*row , 15);
                    char_index =0; 
                    flag=0;
                    incoming_size=0;
                    row++;
                    row++;
                    column=0;
                    RIT128x96x4StringDraw("Send text:", 8*(column), 8*row, 15);
                    row++;
                    column=0;
            
            
            }
        }   
        
        return 0;
}

void SysTick_Handler(){
        pc5 = GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5);
    
        if (GENERATE && pc5){
            assert_count++;
            if (assert_count > 100){
                GENERATE=0;
                done=1;
            }
        }           
        
        if( (GENERATE) && (array_count < total_bits) && (assert_count>0) && (!pc5) ){
                if(assert_count < 4){
                    array_count++;
                }
                
                else if(assert_count >= 4){
                    button_sum = button_sum + (1 << array_count);
                    array_count++;
                }
                y++;
            }
        if (y>15){
                y=0;
                z++;
        }
        if (array_count >= total_bits){
                hold++;
                if ( !(hold%1500) ){
                                        
                    draw_num(button_sum, column,row);
                    next++;
                }
        }
        if (!pc5){
            GENERATE=1;
            assert_count=0;
        }      
}

void init_GPIO(){
    
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIOF ports
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); //enable GPIOE ports
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); //enable GPIOC ports
    
    //INPUT pin PC5
    GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_5,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
    GPIODirModeSet(GPIO_PORTC_BASE,GPIO_PIN_5,GPIO_DIR_MODE_IN);    
    
     //OUTPUT pin PC7
  GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
    GPIODirModeSet(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_DIR_MODE_OUT);
    
    //switch select  PRESS = 0
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_DIR_MODE_IN);     
    
    //switch up     PRESS = 0
        GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTE_BASE,GPIO_PIN_0,GPIO_DIR_MODE_IN);     
    
    //"" down    PRESS = 0
        GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_1,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTE_BASE,GPIO_PIN_1,GPIO_DIR_MODE_IN);     
    
    //"" left    PRESS = 0
        GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_DIR_MODE_IN);     
    
    //"" right  PRESS = 0
        GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_DIR_MODE_IN);
}


void draw_num(int number, int column, int row ){
    switch(number){
        case zero: 
            RIT128x96x4StringDraw(" ", 8*column, 8*row , 15);
            good=1;
            character[num_char] = ' ';
            break;

        case two: 
            if ((next%4) ==0){
                RIT128x96x4StringDraw("A", 8*column, 8*row, 15);
                character[num_char] = 'A';
            }
            else if ((next%4) == 1){
                RIT128x96x4StringDraw("B", 8*column, 8*row, 15);
                character[num_char] = 'B';
            }
            else if ((next%4) == 2){
                RIT128x96x4StringDraw("C", 8*column, 8*row, 15);
                character[num_char] = 'C';
            }
            else if ((next%4) == 3){
                RIT128x96x4StringDraw("2", 8*column, 8*row, 15);
                character[num_char] = '2';
            }
            
            good=1;
            break;
        case three: 
            if ((next%4) ==0){
                RIT128x96x4StringDraw("D", 8*column, 8*row, 15);
                character[num_char] = 'D';
            }
            else if ((next%4) == 1){
                RIT128x96x4StringDraw("E", 8*column, 8*row, 15);
                character[num_char] = 'E';
            }
            else if ((next%4) == 2){
                RIT128x96x4StringDraw("F", 8*column, 8*row, 15);
                character[num_char] = 'F';
            }
            else if ((next%4) == 3){
                RIT128x96x4StringDraw("3", 8*column, 8*row, 15);
                character[num_char] = '3';
            }
            good=1;
            break;
        case four: 
            if ((next%4) ==0){
                RIT128x96x4StringDraw("G", 8*column, 8*row, 15);
                character[num_char] = 'G';
            }
            else if ((next%4) == 1){
                RIT128x96x4StringDraw("H", 8*column, 8*row, 15);
                character[num_char] = 'H';
            }
            else if ((next%4) == 2){
                RIT128x96x4StringDraw("I", 8*column, 8*row, 15);
                character[num_char] = 'I';
            }
            else if ((next%4) == 3){
                RIT128x96x4StringDraw("4", 8*column, 8*row, 15);
                character[num_char] = '4';
            }
            good=1;
            break;
        case five: 
            if ((next%4) ==0){
                RIT128x96x4StringDraw("J", 8*column, 8*row, 15);
                character[num_char] = 'J';
            }
            else if ((next%4) == 1){
                RIT128x96x4StringDraw("K", 8*column, 8*row, 15);
                character[num_char] = 'K';
            }
            else if ((next%4) == 2){
                RIT128x96x4StringDraw("L", 8*column, 8*row, 15);
                character[num_char] = 'L';
            }
            else if ((next%4) == 3){
                RIT128x96x4StringDraw("5", 8*column, 8*row, 15);
                character[num_char] = '5';
            }
            good=1;
            break;
        case six: 
            if ((next%4) ==0){
                RIT128x96x4StringDraw("M", 8*column, 8*row, 15);
                character[num_char] = 'M';
            }
            else if ((next%4) == 1){
                RIT128x96x4StringDraw("N", 8*column, 8*row, 15);
                character[num_char] = 'N';
            }
            else if ((next%4) == 2){
                RIT128x96x4StringDraw("O", 8*column, 8*row, 15);
                character[num_char] = 'O';
            }
            else if ((next%4) == 3){
                RIT128x96x4StringDraw("6", 8*column, 8*row, 15);
                character[num_char] = '6';
            }
            good=1;
            break;
        case seven: 
            if ((next%5) ==0){
                RIT128x96x4StringDraw("P", 8*column, 8*row, 15);
                character[num_char] = 'P';
            }
            else if ((next%5) == 1){
                RIT128x96x4StringDraw("Q", 8*column, 8*row, 15);
                character[num_char] = 'Q';
            }
            else if ((next%5) == 2){
                RIT128x96x4StringDraw("R", 8*column, 8*row, 15);
                character[num_char] = 'R';
            }
            else if ((next%5) == 3){
                RIT128x96x4StringDraw("S", 8*column, 8*row, 15);
                character[num_char] = 'S';
            }
            else if ((next%5) == 4){
                RIT128x96x4StringDraw("7", 8*column, 8*row, 15);
                character[num_char] = '7';
            }
            good=1;
            break;
        case eight: 
            if ((next%4) ==0){
                RIT128x96x4StringDraw("T", 8*column, 8*row, 15);
                character[num_char] = 'T';
            }
            else if ((next%4) == 1){
                RIT128x96x4StringDraw("U", 8*column, 8*row, 15);
                character[num_char] = 'U';
            }
            else if ((next%4) == 2){
                RIT128x96x4StringDraw("V", 8*column, 8*row, 15);
                character[num_char] = 'V';
            }
            else if ((next%4) == 3){
                RIT128x96x4StringDraw("8", 8*column, 8*row, 15);
                character[num_char] = '8';
            }
            good=1;
            break;
        case nine: 
            if ((next%5) ==0){
                RIT128x96x4StringDraw("W", 8*column, 8*row, 15);
                character[num_char] = 'W';
            }
            else if ((next%5) == 1){
                RIT128x96x4StringDraw("X", 8*column, 8*row, 15);
                character[num_char] = 'X';
            }
            else if ((next%5) == 2){
                RIT128x96x4StringDraw("Y", 8*column, 8*row, 15);
                character[num_char] = 'Y';
            }
            else if ((next%5) == 3){
                RIT128x96x4StringDraw("Z", 8*column, 8*row, 15);
                character[num_char] = 'Z';
            }
            else if ((next%5) == 4){
                RIT128x96x4StringDraw("9", 8*column, 8*row, 15);
                character[num_char] = '9';
            }
            good=1;
            break;
//      default:
//          RIT128x96x4StringDraw("ERROR", 8*column, 8*row, 15);
        
    }

}
void
UART0IntHandler(void)
{
    unsigned long ulStatus;

    ulStatus = UARTIntStatus(UART0_BASE, true);    // Get the interrrupt status.

    UARTIntClear(UART0_BASE, ulStatus);    // Clear the asserted interrupts.

    while(UARTCharsAvail(UART0_BASE))    // Loop while there are characters in the receive FIFO.

    {
        // Read the next character from the UART and write it back to the UART.
        // UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
                UARTCharPut(UART1_BASE, UARTCharGet(UART0_BASE));
    }
}


void
UART1IntHandler(void){
     unsigned long ulStatus;

    ulStatus = UARTIntStatus(UART1_BASE, true);    // Get the interrrupt status.

    UARTIntClear(UART1_BASE, ulStatus);    // Clear the asserted interrupts.

    while(UARTCharsAvail(UART1_BASE))    // Loop while there are characters in the receive FIFO.
    {
     incoming_text[char_index] = UARTCharGet(UART1_BASE); 
         char_index++;
         if(char_index==3){
                incoming_size = incoming_text[2];
         }
     
         if (char_index == (incoming_size+4)){
             flag =1;
             incoming_text[char_index-1] = '\0'; 
       }
    }
}



void
init_uart(void)
{

    // Enable the peripherals used
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

   IntMasterEnable();    // Enable processor interrupts.

    // Set GPIO A0 and A1 as UART0 pins.
    //D2 and D3 for UART1
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    // Configure the UART for 9600, 8-N-1 operation.

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

    // Enable the UART interrupt.
    UARTEnable(UART0_BASE);   
    UARTEnable(UART1_BASE);
    
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); 
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);

}

