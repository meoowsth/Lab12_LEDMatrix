/*	Author: Tinghui Song
 *  Partner(s) Name: none	
 *	Lab Section: 24
 *	Assignment: Lab #12  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: https://drive.google.com/file/d/1oWbxb8BFeQSaI3a3rwoFwOR1wpn2gDa6/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "Timer.h"
#endif

unsigned char tempA;
unsigned char pattern;
unsigned char row;

//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
enum Column_States {Start, Wait} Column_State;

void Column_Tick() {
	switch(Column_State){
		case Start:
			pattern = 0x80;
			row = 0x1F;
			Column_State = Wait;
			break;
		case Wait:
			if (tempA == 0x04 && pattern < 0x80) {
				pattern <<= 1;
			}
			else if (tempA == 0x08 && pattern > 0x01){
				pattern  >>= 1;
			}
			Column_State = Wait;
			break;
		default:
			break;
	}
	PORTC = pattern;
	PORTD = ~row;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
    /* Insert your solution below */
	TimerSet(100);
	Column_State = Start;
	TimerOn();

    while (1) {
	tempA = ~PINA;
	Column_Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}

