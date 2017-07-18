//// ***** 0. Documentation Section *****
//// SwitchLEDInterface.c for Lab 8
//// Runs on LM4F120/TM4C123
//// Use simple programming structures in C to toggle an LED
//// while a button is pressed and turn the LED on when the
//// button is released.  This lab requires external hardware
//// to be wired to the LaunchPad using the prototyping board.
//// January 15, 2016
////      Jon Valvano and Ramesh Yerraballi

//// ***** 1. Pre-processor Directives Section *****
//#include "TExaS.h"
//#include "tm4c123gh6pm.h"
///*
//#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
//#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
//#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
//#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
//#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
//#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
//#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
//*/
//#define PE1   (*((volatile unsigned long *)0x40024008))

//// ***** 2. Global Declarations Section *****

//// FUNCTION PROTOTYPES: Each subroutine defined
//void DisableInterrupts(void); // Disable interrupts
//void EnableInterrupts(void);  // Enable interrupts
//void PortE_Init(void);
//void Delay100ms(unsigned long time);

//// ***** 3. Subroutines Section *****

//// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor.
//// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor.

//// To avoid damaging your hardware, ensure that your circuits match the schematic
//// shown in Lab8_artist.sch (PCB Artist schematic file) or 
//// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).

//int main(void)
//{ 
//	//**********************************************************************
//	// The following version tests input on PE0 and output on PE1
//	//**********************************************************************
//  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
//  
//	// initialization goes here
//	PortE_Init();
//	
//  EnableInterrupts();           // enable interrupts for the grader
//	
//	//LED ON PE1
//	
//  while(1)
//	{
//    // Delay about 100 ms
//		Delay100ms(1);
//		// Read the switch and while the switch is pressed
//		if(GPIO_PORTE_DATA_R & 0x01)
//		{
//			PE1 = PE1^(1<<1);
//		}
//		else
//		{
//			// LED ON again
//			PE1 = (1<<1);
//		}
//  }  
//}

//// Subroutine to initialize port E pins for input and output
//// PE0 is input SW1 and PE1 is output LED
//// Inputs: None
//// Outputs: None
//// Notes: ...
//void PortE_Init(void)
//{
//	volatile unsigned long delay;
//	SYSCTL_RCGC2_R |= 0x00000010;     // 1) activate clock for Port E  // 0001.0000  <=>  HGFE.DCBA
//	delay = SYSCTL_RCGC2_R;           // 2) allow time for clock to start // any statement as a delay
//	GPIO_PORTE_AMSEL_R &= ~0x17;      // 3) disable analog on PF
//  GPIO_PORTE_PCTL_R &= ~0x000000FF; // 4) PCTL GPIO on PE4-0 bits for PE4,PE2,PE1,PE0
//  GPIO_PORTE_DIR_R |= (1<<1);       // 5) PE1 output
//  GPIO_PORTE_DIR_R &= ~(1<<0);       // 5) PE0 input	
//  GPIO_PORTE_AFSEL_R &= ~0x17;      // 6) not alternative..disable alt funct on PF7-0
//  GPIO_PORTE_DEN_R |= 0x17;         // 7) enable digital I/O on PE4,PE2,PE1,PE0 enable 
//}

//void Delay100ms(unsigned long time)
//{
//  unsigned long i;
//  while(time > 0){
//    i = 1333333;  // this number means 100ms
//    while(i > 0)
//		{
//      i = i - 1;
//    }
//    time = time - 1; // decrements every 100 ms
//  }
//}


// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to:
// 1. Flash LED at 5Hz if switch pressed
// 2. Turn LED on (no flashing) if switch released
// This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 11, 2014

// Lab 8
//      Jon Valvano and Ramesh Yerraballi
//      November 21, 2013

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

/* ***** 3. Subroutines Section *****
 * PE0: +ve logic switch
 * PE1: +ve logic LED
 *************************************/

// Define port E addresses
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R 				(*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define PE0											(*((volatile unsigned long *)0x40024004))
#define PE1											(*((volatile unsigned long *)0x40024008))

void port_init(void){
	// Initialise port e
	// written by Billy.Ljm
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x10;           // Port E clock
  delay = SYSCTL_RCGC2_R;           // wait 3-5 bus cycles
  GPIO_PORTE_DIR_R |= 0x02;         // PE1 output
  GPIO_PORTE_DIR_R &= ~0x01;        // PE0 input 
  GPIO_PORTE_AFSEL_R &= ~0x03;      // not alternative
  GPIO_PORTE_AMSEL_R &= ~0x03;      // no analog
  GPIO_PORTE_PCTL_R &= 0x00000000; 	// clear PCTL
	GPIO_PORTE_PUR_R &= 0x00;					// no pull-up
  GPIO_PORTE_DEN_R |= 0x03;         // enable PE0, PE1
}

void Delay100ms(void){
	// function to delay for 100ms
  unsigned long i;
    i = 1333333; 
    while(i > 0)
      i = i - 1;
}

int main(void){ 
	// written by Billy.Ljm
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  EnableInterrupts(); // enable interrupts for the grader
  port_init(); // initialise porrt E
	PE1 = 0x02;	// turn LED on
	
	while(1){
		if (PE0 == 0x01) // switch pressd, toggle LED
			PE1 ^= 0x02;
		else // switch released, turn on LED
			PE1 = 0x02;
		Delay100ms(); // delay to read
  }
}

