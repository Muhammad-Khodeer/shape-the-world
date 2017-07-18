////// ***** 0. Documentation Section *****
////// TableTrafficLight.c for Lab 10
////// Runs on LM4F120/TM4C123
////// Index implementation of a Moore finite state machine to operate a traffic light.  
////// Daniel Valvano, Jonathan Valvano
////// January 15, 2016

////// east/west red light connected to PB5
////// east/west yellow light connected to PB4
////// east/west green light connected to PB3
////// north/south facing red light connected to PB2
////// north/south facing yellow light connected to PB1
////// north/south facing green light connected to PB0
////// pedestrian detector connected to PE2 (1=pedestrian present)
////// north/south car detector connected to PE1 (1=car present)
////// east/west car detector connected to PE0 (1=car present)
////// "walk" light connected to PF3 (built-in green LED)
////// "don't walk" light connected to PF1 (built-in red LED)

////// ***** 1. Pre-processor Directives Section *****
////#include "TExaS.h"
////#include "tm4c123gh6pm.h"
////#include "SysTick.h"

////// port B >> leds
////// port F >> walk
////// Port E >> sensors

////// east facing red light connected to PB5
////// east facing yellow light connected to PB4
////// east facing green light connected to PB3
////// north facing red light connected to PB2
////// north facing yellow light connected to PB1
////// north facing green light connected to PB0

////// south facing car detector connected to PE1 (1=car present)
////// west facing car detector connected to PE0 (1=car present)

////// port B leds
////#define LIGHT_CARS                   (*((volatile unsigned long *)0x400050FC))
/////*#define GPIO_PORTB_OUT          (*((volatile unsigned long *)0x400050FC)) // bits 5-0
////#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
////#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
////#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
////#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
////#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))*/
////	
////// port E 
//////#define GPIO_PORTE_IN           (*((volatile unsigned long *)0x4002401C)) // bits 1-0
////// 3sensors
////#define SENSOR                  (*((volatile unsigned long *)0x4002401C))
/////*#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
////#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
////#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
////#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
////#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))*/
////	
//////F
////#define LIGHT_PEAPLE                   (*((volatile unsigned long *)0x40025028)) 

////	
////#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
////#define SYSCTL_RCGC2_GPIOE      0x00000010  // port E Clock Gating Control
////#define SYSCTL_RCGC2_GPIOB      0x00000002  // port B Clock Gating Control


////struct State 
////	{
////		unsigned long Car_OutPut;
////		unsigned long Peaple_OutPut;	
////		unsigned long Time;  
////		unsigned long Next[8];
////	}; 
////typedef const struct State STyp;
////	
////	
////#define goW   0        	//2.3 // 0000.1100 // 0x0c	// 0x02
////#define waitW 1        	//2.4 // 0001.0100 // 0x14	// 0x02
////#define goS   2					//0.5	// 0010.0001 // 0x21	// 0x02
////#define waitS 3					//1.5	// 0010.0010 // 0x22	// 0x02
////	
////#define walk   4        //2.5	// 0010.0100 // 0x24	// 0x08
////#define dontWalk 5    	//2.5	// 0010.0100 // 0x24	// 0x02
////#define walkOff   6			//2.5	// 0010.0100 // 0x24	// 0x00
////#define dontWalkT 7			//2.5	// 0010.0100 // 0x24	// 0x02
////#define walkOffT   8		//2.5	// 0010.0100 // 0x24	// 0x00

////STyp FSM[] = 
////{
//// {0x0c, 0x02,	5,{goW,waitW,waitW,waitW,goW,waitW,waitW,goS}},//waitW 
//// {0x14, 0x02, 5,{goS,walk,goS,goS,waitW,walk,goS,goS}},
//// {0x21, 0x02,	5,{goS,waitS,goS,waitS,waitS,waitS,waitS,waitS}},
//// {0x22, 0x02, 5,{goW,walk,waitS,walk,goW,goW,goW,walk}},
//// 
//// {0x24, 0x08,	5,{walk,dontWalk,dontWalk,dontWalk,dontWalk,dontWalk,dontWalk,dontWalk}}, 
//// {0x24, 0x02, 5,{walkOff,walkOff,walkOff,walkOff,walkOff,walkOff,walkOff,walkOff}},
//// {0x24, 0x00,	5,{dontWalkT,dontWalkT,dontWalkT,dontWalkT,dontWalkT,dontWalkT,dontWalkT,dontWalkT}},
//// {0x24, 0x02,	5, {walkOffT,walkOffT,walkOffT,walkOffT,walkOffT,walkOffT,walkOffT,walkOffT}},
//// {0x24, 0x00,	5, {walk,walkOffT,goS,goS,goW,goW,goW,goW}}
////};


////// ***** 2. Global Declarations Section *****

////// FUNCTION PROTOTYPES: Each subroutine defined
////void DisableInterrupts(void); // Disable interrupts
////void EnableInterrupts(void);  // Enable interrupts

////// ***** 3. Subroutines Section *****
////unsigned long S;  // index to the current state 
////unsigned long Input; 

////int main(void)
////{ volatile unsigned long delay;
////	TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
////	
////	
////	
////	//PLL_Init();       // 80 MHz, Program 10.1
////	SysTick_Init();   // Program 10.2
////	
////	
////	SYSCTL_RCGC2_R |= 0x32;      			// 1) Enable Clock in B E
////  delay = SYSCTL_RCGC2_R;      			// 2) no need to unlock
////	
////	//port E input pins 0.1.2
////  GPIO_PORTE_AMSEL_R &= ~0x07; 			// 3) disable analog function on PE2-1-0
////  GPIO_PORTE_PCTL_R &= ~0x00000FFF; // 4) enable regular GPIO
////  GPIO_PORTE_DIR_R &= ~0x07;   			// 5) inputs on PE2-1-0
////  GPIO_PORTE_AFSEL_R &= ~0x07; 			// 6) regular function on PE2-1-0
////  GPIO_PORTE_DEN_R |= 0x07;    			// 7) enable digital on PE2-1-0
////	//port B output pins 0:5
////  GPIO_PORTB_AMSEL_R &= ~0x3F; 			// 3) disable analog function on PB5-0
////  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF; // 4) enable regular GPIO
////  GPIO_PORTB_DIR_R |= 0x3F;    			// 5) outputs on PB5-0
////  GPIO_PORTB_AFSEL_R &= ~0x3F; 			// 6) regular function on PB5-0
////  GPIO_PORTB_DEN_R |= 0x3F;    			// 7) enable digital on PB5-0
////	//port F output pins 1.3  
////	GPIO_PORTF_AMSEL_R &= ~0x0A; 			// 3) disable analog function on PB5-0
////  GPIO_PORTF_PCTL_R &= ~0x0000F0F0; // 4) enable regular GPIO
////  GPIO_PORTF_DIR_R |= 0x0A;    			// 5) outputs on PB5-0
////  GPIO_PORTF_AFSEL_R &= ~0x0A; 			// 6) regular function on PB5-0
////  GPIO_PORTF_DEN_R |= 0x0A;    			// 7) enable digital on PB5-0
////	
////	
////  S = goS;   
////  
////  EnableInterrupts();
////	
////  while(1)
////	{
////    LIGHT_CARS = FSM[S].Car_OutPut;  // set lights
////		LIGHT_PEAPLE = FSM[S].Peaple_OutPut;
////    SysTick_Wait10ms(FSM[S].Time);
////    Input = SENSOR;     // read sensors
////    S = FSM[S].Next[Input];  
////  }
////}
// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// November 7, 2013

// Completed by Billy.Ljm
// 19 March 2014

// Southwards green,yellow,red light connected to PB0,PB1,PB2
// Westwards green,yellow,red light connected to PB3,PB4,PB5
// Walk,stop light connected to PF3,PF1
// Detectors for westwards,southwards,pedestrian connected to PE0,PE1,PE2

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****
#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))
#define TRAFFIC_LIGHTS			(*((volatile unsigned long *)0x400050FC))
#define PEDESTRIAN_LIGHTS		(*((volatile unsigned long *)0x40025028))
#define SENSORS							(*((volatile unsigned long *)0x4002401C))

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
void ports_Init(void){
	// Written by Billy.Ljm
	// Initialise port B,E,F specifically for this program
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x32; // activate clock for Port B,E,F
  delay = SYSCTL_RCGC2_R; // allow time for clock to start
	
	// Port B
  GPIO_PORTB_LOCK_R = 0x4C4F434B;   // unlock port
  GPIO_PORTB_CR_R = 0x3F;           // allow changes to PB5-0
	GPIO_PORTB_PCTL_R = 0x00000000;   // clear PCTL
  GPIO_PORTB_AMSEL_R &= ~0x3F;      // disable analog on PB5-0
  GPIO_PORTB_AFSEL_R &= ~0x3F;      // disable alt funct on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;         // enable digital I/O on PB5-0
	GPIO_PORTB_DIR_R |= 0x3F;         // PB5-0 outputs
	
	// Port E
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // unlock port
  GPIO_PORTE_CR_R = 0x07;           // allow changes to PE2-0
	GPIO_PORTE_PCTL_R = 0x00000000;   // clear PCTL
  GPIO_PORTE_AMSEL_R &= ~0x07;      // disable analog on PE2-0
  GPIO_PORTE_AFSEL_R &= ~0x07;      // disable alt funct on PE2-0
  GPIO_PORTE_PUR_R &= ~0x07;        // disableb pull-up on PE2-0
  GPIO_PORTE_DEN_R |= 0x07;         // enable digital I/O on PE2-0
	GPIO_PORTE_DIR_R &= ~0x07;        // PE2-0 inputs

	// Port F
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock port
  GPIO_PORTF_CR_R = 0x0A;           // allow changes to PF1 & PF3
	GPIO_PORTF_PCTL_R = 0x00000000;   // clear PCTL
  GPIO_PORTF_AMSEL_R &= ~0x0A;      // disable analog on PF1 & PF3
  GPIO_PORTF_AFSEL_R &= ~0x0A;      // disable alt funct on PF1 & PF3
  GPIO_PORTF_DEN_R |= 0x0A;         // enable digital I/O on PF1 & PF3
	GPIO_PORTF_DIR_R |= 0x0A;         // PF1 & PF3 outputs
}

void SysTick_Init(void){
	// Initalise systick
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}

void SysTick_Wait10ms(){
	// Function to delay for 10ms
  NVIC_ST_RELOAD_R = 8000000 - 1;  // number of counts to wait (80Mhz PLL)
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}
void SysTick_Wait(unsigned long delay){
	// Written by Billy.Ljm
	// Fucntion to delay for integer multiples of 10ms
	unsigned long i;
	for(i=0; i < delay; i++)
		SysTick_Wait10ms();
}

typedef struct Stype{
	// Written by Billy.Ljm
	// struct of a single state in the FSM
	unsigned long TrafficOut; // output for traffic lights (portB)
	unsigned long WalkOut; // output for pedestrian lights (portF)
	unsigned long Time; // delay time (Time * 0.5s)
	unsigned long Next[8]; // next state
} SType;

int main(void){ 
	// Written by Billy.Ljm
	// Variables Used
	unsigned long S = 0; // current state
	SType FSM[11]={
		// States of Finite State Machine (see attached table)
		{0x0C,0x02,20,{0,0,1,1,1,1,1,1}},
		{0x14,0x02,30,{1,0,2,2,4,4,2,2}},
		{0x21,0x02,20,{2,3,2,3,3,3,3,3}},
		{0x22,0x02,30,{3,0,2,0,4,0,4,4}},
		{0x24,0x08,20,{4,5,5,5,4,5,5,5}},
		{0x24,0x00,5,{4,6,6,6,4,6,6,6}},
		{0x24,0x02,5,{4,7,7,7,4,7,7,7}},
		{0x24,0x00,5,{4,8,8,8,4,8,8,8}},
		{0x24,0x02,5,{4,9,9,9,4,9,9,9}},
		{0x24,0x00,5,{4,10,10,10,4,10,10,10}},
		{0x24,0x02,5,{5,0,2,0,4,0,2,0}}
	};
	
	// Initialisation
	TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
	ports_Init(); // Initialise port B,E,F
	SysTick_Init(); // Initialise systick
  EnableInterrupts();
	
	// Looping through FSM
  while(1){
    TRAFFIC_LIGHTS = FSM[S].TrafficOut;  // set traffic lights
		PEDESTRIAN_LIGHTS = FSM[S].WalkOut; // set pedestrian lights
    SysTick_Wait(FSM[S].Time); // delay
    S = FSM[S].Next[SENSORS];  // next state
  }
}