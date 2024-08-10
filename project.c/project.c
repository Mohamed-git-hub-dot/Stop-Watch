#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void TIMER1_INIT(unsigned short);
void INT0_INIT();
void INT1_INIT();
void INT2_INIT();
void PORT_INIT();

unsigned char flag = 0;
unsigned int sec = 0;
unsigned int min = 0;
unsigned int hour = 0;


int main()
{
	PORT_INIT();
	INT0_INIT();
	INT1_INIT();
	INT2_INIT();

	TIMER1_INIT(1000);

	while(1)
	{
		PORTA = (1 << PA5);
		PORTC = (PORTC & 0xF0) | ((sec%10) & 0x0F);
		_delay_us(10);

		PORTA = (1 << PA4);
		PORTC = (PORTC & 0xF0) | ((sec/10) & 0x0F);
		_delay_us(10);

		PORTA = (1 << PA3);
		PORTC = (PORTC & 0xF0) | ((min%10) & 0x0F);
		_delay_us(10);

		PORTA = (1 << PA2);
		PORTC = (PORTC & 0xF0) | ((min/10) & 0x0F);
		_delay_us(10);

		PORTA = (1 << PA1);
		PORTC = (PORTC & 0xF0) | ((hour%10) & 0x0F);
		_delay_us(10);

		PORTA = (1 << PA0);
		PORTC = (PORTC & 0xF0) | ((hour/10) & 0x0F);
		_delay_us(10);
	}

	return 0;
}

void PORT_INIT()
{
	DDRC |= 0x0F;
	DDRA |= 0x3F;

	PORTC &= 0xF0;
	PORTA &= 0xC0;
}

void TIMER1_INIT(unsigned short PWM)
{
	TCCR1A = (1 << FOC1A);
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
	TCNT1 = 0;
	OCR1A = PWM;
	TIMSK |= (1 << OCIE1A);
	SREG |= (1 << 7);
}

void INT0_INIT()
{
	PORTD &= ~(1 << PD2);
	SREG |= (1 << 7);
	GICR |= (1 << INT0);
	MCUCR |= (1 << ISC01);
}

void INT1_INIT()
{
	PORTD &= ~(1 << PD3);
	GICR |= (1 << INT1);
	MCUCR |= (1 << ISC10);
	MCUCR |= (1 << ISC11);

}

void INT2_INIT()
{
	PORTB &= ~(1 << PB2);
	GICR |= (1 << INT2);
	MCUCSR &= ~(1 << ISC2);
}

ISR(INT0_vect)
{
	sec = 0;
	min = 0;
	hour = 0;
}

ISR(INT1_vect)
{
	TCCR1B &= ~(1 << CS10);
	TCCR1B &= ~(1 << CS12);
}

ISR(INT2_vect)
{
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

ISR(TIMER1_COMPA_vect)
{
	sec++;
	if(sec > 59)
	{
		min++;
		sec -= 60;
	}
	if(min > 59)
	{
		hour++;
		min -= 60;
		sec -= 60;
	}
}

