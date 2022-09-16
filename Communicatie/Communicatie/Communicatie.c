/*
 * Communicatie.c
 *
 * Created: 19/05/2015 18:40:32
 *  Author: gijsimus
 */ 
#define x_size 1		// X grote van de array
#define y_size 8		// Y grote van de array
#define F_CPU 1000000UL	// snelheid van de klok
#include <avr/io.h>
#include <util/delay.h>
#define USART_BAUDRATE 4800UL // 4800 baudrate
#define BAUD_PRESCALE  12// ((F_CPU / (16UL * USART_BAUDRATE)) - 1) // berekening voor in de UBBR
// baudvalue = 13.02 dus goed
// voor pin configuratie en pin status aan te passen
#define bit(x) (1 << x)		// kleine functie om een bit hoog te zetten

void Configuration();		// declaratie van bepaalde functies	
void setPanel(unsigned char, unsigned char);
int main(void)
{
	char data;
	Configuration();
	while(1) // Loop forever
	{
		// receive
		while (!(UCSRA & (1<<RXC)));	// De RXC flag zit in het UCSRA register deze word geset van af het moment dat er ongelezen data in de receive buffer zit	
		data = UDR;
		// transmit
		while (!(UCSRA & (1<<UDRE)));	// de UDRE flag toont aan of de UDR klaar is om nieuwe data te krijgen, we gaan dus wachten tot dat deze leeg is.
		UDR = data;
		
		//setPanel(0b11010011, 0b10010100);
		//_delay_ms(10);
	}
		
}
void Configuration()
{
	// USART
	UBRRH = (BAUD_PRESCALE >> 8);									 // Laad de 4 MSB in 12 bit register
	UBRRL = BAUD_PRESCALE;											 // Laad de 8 LSB in 12 bit register
	UCSRB = bit(RXEN)|bit(TXEN);								 	 // Zet de transmition flag en receiver flag hoog
	UCSRC = bit(URSEL)|(1<<UCSZ0)|bit(UCSZ1)|bit(USBS);	 // USBS ( keuze voor hoeveel stop bits) UCSZ[1:0] dienen voor hoeveel data bits er zijn ( 8 )
	// omdat we gebruik maken van de interne clock en zo voor ASYNCHRONE normal mode gebruik maken (voor de baud rate) --> UBRRH en UCSRC delen I/O locaties
	// Hiervoor moet de URSEL bit hoog gemaakt worden
		//	DDRA |= 0xff; //  0b11111111 set alle pinnen op poort A als uitgang
			DDRB |= 0xff; //  0b11111111 set alle pinnen op poort B als uitgang
			DDRC |= 0xff; //  0b11111111 set alle pinnen op poort C als uitgang
			DDRD |= 0xf0; //  0b11110000 set pinnen 4 tot 7 als uitgang
		//	PORTA |= 0x00; // set alle outputs hoog(reset)
			PORTB |= 0x00; // set alle outputs laag(reset)
			PORTC |= 0x0f; // set alle outputs laag(reset)
			PORTD |= 0xff; // set alle outputs laag(reset)
}
void setPanel(unsigned char horizontaal, unsigned char verticaal) 
{
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
	/*
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
	*/
}