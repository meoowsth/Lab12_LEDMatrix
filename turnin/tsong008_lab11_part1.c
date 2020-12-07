/*	Author: Tinghui Song
 *  Partner(s) Name: none	
 *	Lab Section: 24
 *	Assignment: Lab #12  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: https://drive.google.com/file/d/1SHFTqL-XBhJzjb1LSX_jYeKn5x8D3ycA/view?usp=sharing
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
 enum Row_States {Start, Wait} Row_State;

 void Row_Tick() {
 	switch(Row_State){
 		case Start:
			Row_State = Wait;
 			pattern = 0xFF;
 			row = 0x01;
 			break;
 		case Wait:
		 	if (tempA == 0x02 && row < 0x10) {
		 		row <<= 1;
 			}
			else if (tempA == 0x01 && row > 0x01){
 				row >>= 1;
 			}
 			Row_State = Wait;
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
 	TimerSet(100);
 	Row_State = Start;
 	TimerOn();
     /* Insert your solution below */
     while (1) {
	tempA = ~PINA;
	Row_Tick();
	while(!TimerFlag);		
 	TimerFlag = 0;
     }
     return 1;
 }




