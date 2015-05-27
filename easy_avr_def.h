#ifndef _Easy_AVR_DEF_H
#define _Easy_AVR__DEF_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/sleep.h>

        //Define Macros for bit operations:

	#define setBitHigh(port,bit)   port  |=  (1<<bit)
	#define setBitLow(port,bit) port  &= ~(1<<bit)
	#define readBit(port,bit)  port  &   (1<<bit)
	#define setInput(port,bit) port  &= ~(1<<bit)
	#define setOutput(port,bit) port |=  (1<<bit)
	#define toggle(port,bit) port = port^(1<<bit)

	// Define Macros for constants
	#define high 1
	#define low  0

// these functions will work on ANY register .. you need to know
//  the name of the register to use them
// if we used as the arduino lib uses functions here we would waste
// valuable resources of which we do not have many (with only a bit of ram and speed)
// these all execute in one clock cycle and make a smaller overall binary file.....

#endif
