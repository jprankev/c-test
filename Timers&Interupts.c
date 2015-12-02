//*********************************************
//;This file contains the subroutines associated
//;with the Timers/Counters.
//;By: Jeff Prankev
//;Date: Nov 14th, 2015
//;*********************************************


// volatile int RED_LED_FLAG = 0;
volatile int sequence_count = 5;
volatile int note_count = 1;
volatile int sequence_flag = 0;
volatile int count_highbyte = 0xF6;
volatile int count_lowbyte = 0x3C;
volatile int direction = 0;
volatile int note_value = 0x3C;

//;*****************************************************************************
//;The following initializes Timer0:
//;Bit 7 - Clear to disable timer
//;Bit 6 - Clear to set to 16bit 
//;Bit 5 - Clear to use internal clock
//;Bit 4 - clear for falling edge
//;Bit 3 - Clear to enable Prescaler
//;Bit 2 - Set these as such for 1:64 prescaler
//;Bit 1 - Clear 
//;Bit 0 - Set
//;'10010101'= 85h
//******************************************************************************


void TMR0_INIT(void)
{
    INTCONbits.TMR0IF = 0;      //clears the TIMER0 interrupt flag
    RCONbits.IPEN = 1;          //enable interrupt priority
    INTCONbits.GIEH = 1;        //enable high priority interrupts
    INTCONbits.GIEL = 1;        //enable low priority interrupts
    INTCONbits.TMR0IE = 1;      //enable TIMER0 interrupt
    INTCON2bits.TMR0IP = 0;     //sets TIMER0 to LOW priority
    
    T0CON = 0x05;               //Inits the TIMER0 Control 
    TMR0H = 0xFF;               //Sets the high byte of the count
    TMR0L = 0xFF;               //Sets the low byte of the count to count
                                //1 until overflow
    
}

void INT_INIT(void)
{
//      INTCONbits.INT0IE = 1;
//      INTCON2bits.INTEDG0 = 0;
    
}

//******************************************************************************
//                                 Interrupts
//******************************************************************************


//****************************************************************************
//This function contains the functionality of the Low Interrupt.  It resets the 
//interrupt and restores the count value for the timer.  It also counts down 
//RED_LED_COUNT until 0, at which point it toggles RED_LED_FLAG which in turn 
//toggles RED_LED in Main.
//****************************************************************************

void interrupt low_priority TOGGLE_SEQUENCE(void)
{
    INTCONbits.TMR0IF = 0;      //resets the interrupt flag
    
    if (direction == 0)
    {
        T0CONbits.TMR0ON = 0;        
        count_highbyte = 0xFF;
        count_lowbyte =  0xFF;
        note_value = 60;
    }
    else if (direction == 1)
    {   
        count_highbyte = 0xF3;
        count_lowbyte =  0xCB;
    }
    else if (direction == 2)
    {
        count_highbyte = 0xF9;
        count_lowbyte =  0xE6;
    }
    
    TMR0H = count_highbyte;     //Sets the high byte of the count
    TMR0L = count_lowbyte;      //Sets the low byte of the count
                                //3125 until overflow

    
//flag Toggle:
    sequence_count = sequence_count - 1;
    
    if (sequence_count == 0)
    {
        if (sequence_flag == 0)
        {
            sequence_flag = 1;
        }
        else
        {
            sequence_flag = 0;
        }    
        sequence_count = 5;
    }
    
}

