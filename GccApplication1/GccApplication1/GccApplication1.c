/*
 * GccApplication1.c
 *
 * Created: 27/04/2015 18:09:56
 *  Author: gijs Vermeulen
 */ 
#define F_CPU 1000000UL
// Interne clock 1MHz 
#include <avr/io.h>
#include <util/delay.h>


#define bit(x) (1 << x)

void Pinconfiguratie();
void Test();
void setPanel(unsigned char, unsigned char);

int main(void)
{
	
	Pinconfiguratie();
    while(1)
    {
		
		//Test();
		//PORTC ^= bit(6);
		//PORTC ^= bit(7);
		//PORTC ^= bit(5);
		//setPanel(0b11010011, 0b10000100);
		setPanel(0b11010011, 0b10010100);
		_delay_ms(1000);
        //TODO:: Please write your application code 
		
    }
}

void setPanel(unsigned char horizontaal, unsigned char verticaal) {
	// rechts
	unsigned char testhorizontaal = verticaal;
	unsigned char testverticaal = horizontaal;
	
	PORTD &= ~bit(5);
	for(int i = 0 ; i < 8 ; i++) {
		//set data lijn juist
		if(horizontaal & bit(i)) PORTC |= bit(2);
		else PORTC &= ~bit(2);
		if(verticaal & bit(i)) PORTC |= bit(3);
		else PORTC &= ~bit(3);
		if(horizontaal & bit(i)) PORTC |= bit(0);
		else PORTC &= ~bit(0);
		if(verticaal & bit(i)) PORTC |= bit(1);
		else PORTC &= ~bit(1);
		if(horizontaal & bit(i)) PORTD |= bit(6);
		else PORTD &= ~bit(6);
		if(verticaal & bit(i)) PORTD |= bit(7);
		else PORTD &= ~bit(7);		
		//rising edge op de clk
		_delay_ms(20);
		PORTD |= bit(5);
		//clk terug naar beneden
		_delay_ms(20);
		PORTD &= ~bit(5);
		_delay_ms(20);
	
	}
	
	// midden
	
	PORTC &= ~bit(5);
	for(int i = 0 ; i < 8 ; i++) {
		//set data lijn juist
		
		if(horizontaal & bit(i)) PORTB |= bit(2);
		else PORTB &= ~bit(2);
		if(verticaal & bit(i)) PORTB |= bit(3);
		else PORTB &= ~bit(3);
		
		if(testhorizontaal & bit(i)) PORTB |= bit(0);
		else PORTB &= ~bit(0);
		if(testverticaal & bit(i)) PORTB |= bit(1);
		else PORTB &= ~bit(1);
		
		if(horizontaal & bit(i)) PORTC |= bit(6);
		else PORTC &= ~bit(6);
		if(verticaal & bit(i)) PORTC |= bit(7);
		else PORTC &= ~bit(7);
		
		//rising edge op de clk
		_delay_ms(20);
		PORTC |= bit(5);
		//clk terug naar beneden
		_delay_ms(20);
		PORTC &= ~bit(5);
		_delay_ms(20);
	}
	
	// links
	
		PORTA &= ~bit(1);
		for(int i = 0 ; i < 8 ; i++) {
			//set data lijn juist
			
			if(horizontaal & bit(i)) PORTA |= bit(6);
			else PORTA &= ~bit(6);
			if(verticaal & bit(i)) PORTA |= bit(7);
			else PORTA &= ~bit(7);
			
			if(horizontaal & bit(i)) PORTA |= bit(4);
			else PORTA &= ~bit(4);
			if(verticaal & bit(i)) PORTA |= bit(5);
			else PORTA &= ~bit(5);
			
			if(horizontaal & bit(i)) PORTA |= bit(2);
			else PORTA &= ~bit(2);
			if(verticaal & bit(i)) PORTA |= bit(3);
			else PORTA &= ~bit(3);
			
			//rising edge op de clk
			_delay_ms(20);
			PORTA |= bit(1);
			//clk terug naar beneden
			_delay_ms(20);
			PORTA &= ~bit(1);
			_delay_ms(20);
		
	}
	
}

void Pinconfiguratie()
{
	DDRA |= 0xff;
	DDRB |= 0xff; //  0b11111111 set alle pinnen op poort C als uitgang
	DDRC |= 0xff; //  0b11111111 set alle pinnen op poort C als uitgang
	DDRD |= 0xff; //  0b11110000		of bit(7) | bit(6) | ... // set  pinnen 4 tot 7 als uitgang
	PORTA |= 0xff;
	PORTB |= 0xff;
	PORTC |= 0xff;
	PORTD |= 0xff;
}