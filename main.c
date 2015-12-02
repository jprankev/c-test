#include "Configuration_Bits.h"
#include <xc.h>
#include "function_prototypes.h"
#include "port_init.c"
#include "my_definitions.c"
#include "Timers&Interupts.c"
#include <stdarg.h>

//int tick_count=0x0;

int note_sent = 0;


void main(void)
{
	int x = 0, run = 0;

	port_init();
	TMR0_INIT();
    INT_INIT();
	while(x == 0)
	{
	//	buttons();
        
        if (SW6 == 0)
        {
            if run == 0
                    run = 1;
            if run == 1
                    run = 0;
        }
        switch (run)
        {
            case 0:
            {
                note_sent = 0;
                sequence_count = 5;
                note_count = 1;
                sequence_flag = 0;
                count_highbyte = 0xF6;
                count_lowbyte = 0x3C;
                direction = 0;
                note_value = 0x3C;
                while (note_value < 0x47)
                {
                    TXREG2 = 0b10000000;
                    while (PIR3bits.TX2IF == 0);
                    TXREG2 = note_value;
                    while (PIR3bits.TX2IF == 0);
                    TXREG2 = 0b00000000;
                    while (PIR3bits.TX2IF == 0);  
                    note_value = note_value++;
                }
                break;
            }
            {
                T0CONbits.TMR0ON = 1;
                direction = 1;
                __delay_us(50);   

            }
            switch (direction) 
            {
                case 0:
                {
                    break;
                }
                case 1:
                {
                    sequence_up();
                    break;
                }        
                case 2:
                {
                    sequence_down();
                    break;
                }
                default:
                    break;
            }       
        }    
    }    
}

//********************************************************************
//                        Functions
//********************************************************************

void buttons(void)
{
//	if (sequence_flag == 1)
//		RED_LED = 1;
//	else RED_LED = 0;
	
//	if (SW6 == 1)
//		YELLOW_LED = 1;
//	else YELLOW_LED = 0;
}

//******************************************************
//This function checks sequence flag 
//which is toggled in the TOGGLE_SEQUENCE
//low priority interrupt.  If it is cleared 
//and the note_sent flag is clear, it calls
//the SEND_NOTE function to trigger a note
//via the USART2 transmit pin.
//If sequence_flag is set and a note has been sent, 
//a not off is sent instead.  If none of the conditions 
//are met, the function does nothing.
//******************************************************


void sequence_up(void)
{
   	if (YELLOW_LED == 1)
		YELLOW_LED = 0;
    else YELLOW_LED = 1;
    
    if ((sequence_flag == 0) && (note_sent == 0))
        SEND_NOTE();
    else if ((sequence_flag == 1) && (note_sent == 1))
        SEND_NOTEOFF();

}

//*****************************************************
//This function works the same as sequence up, but  
//calls SEND_NOTEOFF_DOWN instead of NOTE_OFF.  This 
//the notes to play in descending order rather than 
//ascending.
//*****************************************************

void sequence_down(void)
{
    if ((sequence_flag == 0) && (note_sent == 0))
    {
        SEND_NOTE();
    }
    else if ((sequence_flag == 1) && (note_sent == 1))
        SEND_NOTEOFF_DOWN();   
}

//*****************************************************
//This function sends a NOTE ON Midi command to the USART
// 2 transmitter by moving bytes to the TXREG2 register
//followed by the current note in "note_value" at velocity 
//100.  The "note_sent" flag is then set.
//*****************************************************


void SEND_NOTE(void)
{
    TXREG2 = 0b10010000;
    while (PIR3bits.TX2IF == 0);
    TXREG2 = note_value;
    while (PIR3bits.TX2IF == 0);
    TXREG2 = 0b01100100;
    while (PIR3bits.TX2IF == 0);  
    note_sent = 1;
    RED_LED = 1;    
}

//*****************************************************
//The following function sends a NOTE OFF command
//to the USART 2 transmitter followed by the current note
// in "note_value" and the velocity 0.  
//The "note_sent" flag is cleared.  It then checks
//to see if 12 ascending notes have been played.  If yes,
//if sets the direction flag to "2" which will cause the
//playback direction to reverse on the next pass.  If
//12 notes have not yet been played, "note_value" is 
//incremented.
//*****************************************************


void SEND_NOTEOFF(void)
{
    TXREG2 = 0b10000000;
    while (PIR3bits.TX2IF == 0);
    TXREG2 = note_value;
    while (PIR3bits.TX2IF == 0);
    TXREG2 = 0b00000000;
    while (PIR3bits.TX2IF == 0);  
    RED_LED = 0;
    note_sent = 0;
    if (note_value >= 0x47)
    {    
  
        direction = 2;
    }
    else 
    {
        direction = 1;
        note_value = (note_value + 1);
    }
}

//*****************************************************
//The following function sends a NOTE OFF command
//to the USART 2 transmitter followed by the current note
// in "note_value" and the velocity 0.  
//The "note_sent" flag is cleared.  It then checks
//to see if 12 ascending notes have been played
//by checking to see if "note_value" is less than 60.
//If yes,
//if sets the direction flag to "0" which will cause
//TIMER0 to be disabled and the TIMER0 interrupt count
//to trigger on a single count the next time the program
//is interrupted. If 12 notes have not yet been played, 
//"note_value" is decremented.
//*****************************************************

void SEND_NOTEOFF_DOWN(void)
{
    TXREG2 = 0b10000000;
    while (PIR3bits.TX2IF == 0);
    TXREG2 = note_value;
    while (PIR3bits.TX2IF == 0);
    TXREG2 = 0b00000000;
    while (PIR3bits.TX2IF == 0);  
    note_sent = 0;
    RED_LED = 0;
    if (note_value <= 0x3C)
    {
        direction = 1;
    }
    else
    {    
        direction = 2;
        note_value = (note_value - 1);
    }

}