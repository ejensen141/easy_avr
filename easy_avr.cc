#include "easy_avr.h"
#include "easy_avr_def.h"





void usart0::write(unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSR0A & (1<<UDRE0)) );
/* Put data into buffer, sends the data */
UDR0 = data;
}


void usart0::write(char * data )
{
	int i=0;

	for (i=0; i<strlen(data); i++)
	{
		write(data[i]);
	}
}




void pwm::setup()
{
		setBitHigh(TCCR0A,COM0A1);
		setBitHigh(TCCR0A,WGM01);
		setBitHigh(TCCR0A,WGM00);
		setBitHigh(TCCR0B,CS00);

}






















//
