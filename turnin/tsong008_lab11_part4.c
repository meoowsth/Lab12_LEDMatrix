/*	Author: Tinghui Song
 *  Partner(s) Name: none	
 *	Lab Section: 24
 *	Assignment: Lab #12  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: https://drive.google.com/file/d/1Qj5TfG3_iy2_73iN8MkoqX8YTv0rSokL/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "Timer.h"

unsigned char tempA;

unsigned char pattern[3] = {0x3C,0x24,0x3C};
unsigned char row[3] = {0x08,0x04,0x02};

unsigned char i;
unsigned char j;

unsigned char pressCounter;

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
		default:
			break;
	}
}

enum Button_States {Button_Start, Button_Wait} Button_State;

void Button_Tick() {
	switch(Button_State){
		case Button_Start:
			Button_State = Button_Wait;
			break;
		case Button_Wait:
			if (tempA == 0x01 && row[2] > 0x01){
				for (j = 0; j < 3; j++){
					row[j] >>= 1;
				}
			}
			else if (tempA == 0x02 && row[0] < 0x10){
				for (j = 0; j < 3; j++){
					row[j] <<= 1;
				}
			}
			else if (tempA == 0x04 && pattern[0] < 0x80) {
				for (j = 0; j < 3; j++){
					pattern[j] <<= 1;
				}
			}
			else if (tempA == 0x08 && pattern[2] > 0x0F){
				for (j = 0; j < 3; j++){
					pattern[j] >>= 1;
				}
			}
			Button_State = Button_Wait;
			break;
		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; 	PORTA = 0xFF;
	DDRD = 0xFF; 	PORTD = 0x00;
	DDRC = 0xFF; 	PORTC = 0x00;

    /* Insert your solution below */

		TimerSet(1);
		State = Start;
		TimerOn();

    while (1) {
	tempA = ~PINA;
	if (pressCounter < 200){
		pressCounter++;
	}
	else {
		Button_Tick();
		pressCounter = 0;
	}
	Rec_Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}

