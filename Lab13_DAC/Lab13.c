// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014

// Port B bits 3-0 have the 4-bit DAC   Output
// Port E bits 3-0 have 4 piano keys		Input

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);

unsigned long piano,prevPiano;

int main(void) // Real Lab13 
{ 
	// for the real board grader to work 
	// you must connect PD3 to your DAC output
	// activate grader and set system clock to 80 MHz
   TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn);
	
	// PortE used for piano keys, PortB used for DAC
	// initialize SysTick timer and DAC	
  Sound_Init(); 
  Piano_Init();
  EnableInterrupts();  // enable after all initialization are done
  while(1)
	{                
		// input from keys to select tone
		piano = Piano_In();
	
		if(piano != prevPiano){ // only react if piano keys pressed
			if(piano == 0x00)
				Sound_Off();
			else if(piano == 0x01)
				Sound_Tone(2389);
			else if(piano == 0x02)
				Sound_Tone(2128);
			else if(piano == 0x04)
				Sound_Tone(1896);
			else if(piano == 0x08)
				Sound_Tone(1594);
			delay(10);
		}
		
		prevPiano = piano;
			

  }          
}

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec)
{ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


