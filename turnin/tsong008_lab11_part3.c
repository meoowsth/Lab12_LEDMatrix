/*	Author: Tinghui Song
 *  Partner(s) Name: none	
 *	Lab Section: 24
 *	Assignment: Lab #12  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link:
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "Timer.h"
#endif

unsigned char pattern[3] = {0x3C,0x24,0x3C};
unsigned char row[3] = {0x08,0x04,0x02};
unsigned char i;

enum States{Start, Seq_1, Seq_2, Seq_3} State;

void Rec_Tick(){
	switch(State){
		case Start:
			State = Seq_1;
			break;
		case Seq_1:
			i = 0;
			PORTC = pattern[i];
			PORTD = ~row[i];
			State = Seq_2;
			break;
		case Seq_2:
			i++;
			PORTC = pattern[i];
			PORTD = ~row[i];
			State = Seq_3;
			break;
		case Seq_3:
			i++;
			PORTC = pattern[i];
			PORTD = ~row[i];
			State = Seq_1;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;

    /* Insert your solution below */

	TimerSet(1);
	State = Start;
	TimerOn();

    while (1) {
	Rec_Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}

