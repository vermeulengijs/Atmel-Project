/*
 * Project2.c
 *
 * Created: 23/03/2015 18:01:52
 *  Author: gijs
 */ 

/* 
	Project 2 : LedMatrixen aansturen via Atmega16A --> 
	Er komt DATA binnen via de USB poort van de Pc aan de FTR232 chip deze zijn Rx en Tx zijn verbonden met Tx en Rx van de atmega
	hierdoor versturen we onze data serieel via een bepaalde baud rate bit/s (9600)
	De Interne clock die gebruikt gaat worden is 1MHz
	Om te beginnen zet je de I/O juist (alle pinnen in uitgang/ingang modus)
	Nu het ding is om de Led matrixen aantesturen moetje ze lijn per lijn aansturen ( anders kan het zijn dat er meerdere leds aangaan terwijl je het niet wilt
	je doet het lijn per lijn maar je doet het zo snel dat je het menselijk oog kunt bedriegen.) ( je doet dit op 25Hz )
	
*/
#define x_size 8		// X grote van de array
#define y_size 8		// Y grote van de array
#define F_CPU 1000000UL	// snelheid van de klok
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <ctype.h>
#define USART_BAUDRATE 4800UL // 4800 baudrate
#define BAUD_PRESCALE ((F_CPU / (16UL * USART_BAUDRATE)) - 1) // berekening voor in de UBBR 
// baudvalue = 13.02 dus goed
// voor pin configuratie en pin status aan te passen
#define bit(x) (1 << x)		// kleine functie om een bit hoog te zetten

void Configuration();		// declaratie van bepaalde functies
void DataTerugsturen(char [x_size][y_size]);
// poof there we go
int main (void)
{
	Configuration();
	char Matrix[x_size][y_size];
	uint8_t x = 0;
	uint8_t y = 0;
		while(1) // Loop forever
		{			
			while (!(UCSRA & (1<<RXC)));	// LEES DATA BINNEN
			Matrix[x][y] = UDR;	// schrijf de ingelezen data bit naar de matrix op positie [x][y]
			x++;
			if(!(x < x_size)) 
			{
				y++;
				x =0;
				if(!(y < y_size))
				{
					x = 0;
					y =0;
					// als de array vol staat gaan we de data terug sturen
					Dataterugsturen(Matrix);	
					// CONVERTIE VAN DATA 
				}
			}				 
		}
		return 0;
}
void Configuration()
{
	// USART
	UBRRH = (BAUD_PRESCALE >> 8);									 // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE;											 // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	UCSRB = bit(RXEN)|bit(TXEN);								 // Turn on the transmission and reception circuitry
	UCSRC = bit(URSEL)|(1<<UCSZ0)|bit(UCSZ1)|bit(USBS)| bit(UCSZ2);				 // USBS ( keuze voor hoeveel stop bits) UCSZ[2:0] dienen voor hoeveel data bits er zijn ( 8 )
	// omdat we gebruik maken van de interne clock en zo voor ASYNCHRONE normal mode gebruik maken (voor de baud rate) --> UBRRH en UCSRC delen I/O locaties
	// Hiervoor moet de URSEL bit hoog gemaakt worden
	
	
	
		DDRA |= 0xff; //  0b11111111 set alle pinnen op poort A als uitgang
		DDRB |= 0xff; //  0b11111111 set alle pinnen op poort B als uitgang
		DDRC |= 0xff; //  0b11111111 set alle pinnen op poort C als uitgang
		DDRD |= 0xf0; //  0b11110000 set pinnen 4 tot 7 als uitgang
		PORTA |= 0xff; // set alle outputs hoog(reset)
		PORTB |= 0x00; // set alle outputs laag(reset)
		PORTC |= 0x00; // set alle outputs laag(reset)
		PORTD |= 0x00; // set alle outputs laag(reset)
}
void Dataterugsturen(char Matrix[x_size][y_size])
{
	for (uint8_t i = 0;i<x_size;i++)
	{
		for (uint8_t j =0;j<y_size;j++)
		{
			while ( !(UCSRA & (bit(UDRE))) ) {};				// Do nothing until UDR is ready for more data to be written to it
			UDR = Matrix[j][i];									// Stuur de data terug
		}
	}
}