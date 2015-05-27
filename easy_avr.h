//##############Easy_Avr Library C++###################
/*
Copyright (c) 2015 Elijah Jensen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software/hardware and associated documentation files
(the "Software" or "Hardware" design plans), to deal in the Software/Hardware
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software/Hardware.

THE SOFTWARE/HARDWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name(s) of the above copyright holders shall
not be used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization.




This library attempts to increase the readablility of avr code, without reducing speed and efficiency.

*/



#ifndef _Easy_AVR_H
#define _Easy_AVR_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include <string.h>
#include "easy_avr_def.h"

//  #################PWM Section ###################3

class pwm{

public:
        inline void setup();
};

//##############################################


// ################SPI section #######################

//##############SPI SPEC###################
/*
SPCR - SPI Control Register

 --------------------------------------------------------------------------------------------
 |SPIE    SPE     DORD    MSTR    CPOL    CPHA    SPR1    SPR0 |
 --------------------------------------------------------------------------------------------
0b01011101

SPIE --> Interrupt Enable if 1
SPE --> SPI enable if 1
DORD --> Data Order---   1=LSB first    0=MSB first
MSTR --> master or slave      1=master  0 = slave
CPOL --> Polarity    if 1  SCK is high while idle   if 0 SCK is low when idle
CPHA --> Phase     if 1 sample on trailing         if 0 sample on leading

SPR1 -->
SPR2 -->
These control Clock Frequency
00 = f/4
01 = f/16
10 = f/64
11 = f/128



SPSR   SPI Status Register
-------------------------------------------------------------------------
| SPIF  WCOL    -       -       -       -       -       SPI2X |
-------------------------------------------------------------------------

When a tranfer is comple the SPIF bit is set to 1

WCOL Write collision

SPI2X Double Speed /////  Doubles speed if 1

*/ //###################################


class spi{

private:
        uint8_t setting;
        uint8_t spcr_value;





public:
        uint8_t data_direction;

        uint8_t write(uint8_t data)
        {
        SPDR = data;
        while (!(SPSR & (1<<SPIF)));
        return SPDR;
        }


        uint32_t write32_msb(uint32_t data)
        {


                uint32_t buffer32;
                uint8_t   buffer;

                //MSB first !
                buffer = data>>24;
                buffer=write(buffer);
                buffer32 = uint32_t(buffer)<<24;


                buffer = data>>16;
                buffer =write(buffer);
                buffer32 |= uint32_t(buffer)<<16;


                buffer = data>>8;
                buffer = write(buffer);
                buffer32 |= uint32_t(buffer)<<8;


                buffer = data;
                buffer = write(buffer);
                buffer32 |= buffer;

                return buffer32;
        }


        uint32_t write32_lsb(uint32_t data)
        {


                uint32_t buffer32;
                uint8_t   buffer;

                //LSB first !
                buffer = data;
                write(buffer);
                buffer32 = uint32_t(buffer);


                buffer = data<<8;
                write(buffer);
                buffer32 |= uint32_t(buffer)<<8;


                buffer = data<<16;
                write(buffer);
                buffer32 |= uint32_t(buffer)<<16;


                buffer = data<<24;
                write(buffer);
                buffer32 |= uint32_t(buffer)<<24;
        }




        //Define inline functions

        // to set the SPCR manually
        inline void  setup(uint8_t value)
        {
                SPCR = value;
        }

        inline void enable()
        {
                setBitHigh(SPCR,SPE);
        }

        inline void polarity(uint8_t var)
        {
                switch(var)
                {
                        case 1:
                        setBitHigh(SPCR,CPOL);
                        break;

                        case 0:
                        setBitLow(SPCR,CPOL);
                        break;


                }
        }


        inline void phase(uint8_t var) // 1 is trailing
        {
                switch(var)
                {
                        case 1:
                        setBitHigh(SPCR,CPHA);
                        break;

                        case 0:
                        setBitLow(SPCR,CPHA);
                        break;


                }
        }

        inline void init()
        {
                setup(0x00);
        }

        inline void setMaster()
        {
                setBitHigh(SPCR,MSTR);
                setOutput(DDRB,2); // Set SS pin output
                setOutput(DDRB,5);  // MOSI line
                setOutput(DDRB,3); // SCK line
        }

        inline void setSlave()
        {
                setBitLow(SPCR,MSTR);
                setOutput(DDRB,4); // set MISO output
                setInput(DDRB,2); // Set SS pin output
                setInput(DDRB,5);  // MOSI line
                setInput(DDRB,3); // SCK line
        }

        inline void data_dir(uint8_t var =1)  //defaults to MSB first
        {
                switch(var)
                {
                        //MSB First
                        case 1:
                        data_direction=1;
                        setBitLow(SPCR,DORD);
                        break;

                        // LSB First
                        case 0:
                        data_direction=0;
                        setBitHigh(SPCR,DORD);
                        break;


                }
        }


        // Takes the divisor and sets the correct bits for that speed... 4= clock/4  16 = clock/16 etc..
         inline void speed(uint8_t var)
         {
                 //set to slow mode natively
                 setBitHigh(SPCR,SPR1);
                 setBitHigh(SPCR,SPR0);

                 switch(var)
                 {
                         case 4:
                         setBitLow(SPCR,SPR0);
                         setBitLow(SPCR,SPR1);
                         break;

                         case 16:
                         setBitLow(SPCR,SPR1);
                         setBitHigh(SPCR,SPR0);
                         break;

                         case 64:
                         setBitHigh(SPCR,SPR1);
                         setBitLow(SPCR,SPR0);
                         break;

                         case 128:
                         setBitHigh(SPCR,SPR1);
                         setBitHigh(SPCR,SPR0);
                        break;
                 }

         }


         inline void speed2x(uint8_t var) //high if true
         {
                switch(var)
                {
                case high:
                 setBitHigh(SPSR,SPI2X);
                break;

                case low:
                setBitLow(SPSR,SPI2X);

                }
         }










};


//########################################


//################USART0###################

class usart0 {

public:
        unsigned int ubrr;

        //TODO: make helper function(s) to set these values

        inline void init( )
        {
        	/*Set baud rate */
        	UBRR0H = (unsigned char)(ubrr>>8);
        	UBRR0L = (unsigned char)ubrr;
        	//Enable receiver and transmitter */
        	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
        	//UCSR0B |= (1 << RXCIE0); //Enable interrupt
        	/* Set frame format: 8data, 2stop bit */
        	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
        }


        void write(unsigned char data );
        void write(char * data );



        inline void interrupt()
        {
                	setBitHigh(UCSR0B, RXCIE0); //Enable interrupt
                        setBitHigh(UCSR0B, RXEN0);


        }




};



#endif





























//EOF
