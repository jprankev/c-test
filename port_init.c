//**********************************************
//Port initialization file
//**********************************************


void port_init(void)
{
	ANSELA = 0x00;
	ANSELB = 0x00;
	ANSELC = 0x00;
	ANSELD = 0x00;
	ANSELE = 0x00;

	PORTA = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTE = 0x07;
	
	TRISA = 0x03;
	TRISB = 0x3F;
	TRISC = 0x1F;
	TRISD = 0xC0;
	TRISE = 0x00;

	WPUB  = 0x3C;
	INTCON2bits.RBPU = 0;

 	OSCCONbits.IRCF0 = 0;
	OSCCONbits.IRCF1 = 1;
	OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS0 = 0;
	OSCCONbits.SCS1 = 1;
    OSCTUNEbits.PLLEN = 0;
 //   #pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block)
    
    
    //***************************************
    //Following Section Configures USART
    //SPBRG1 = 15 (sets baud rate to 31250)
    // 
    
    
    SPBRG2 = 0x0F;
    
    //TXSTA1 
    //Bit 0 - don't care
    //Bit 1 - 1 (high speed baud rate select))
    //Bit 2 - flag, don't care
    //Bit 3 - 0 (sync break transmission completed)
    //Bit 4 - 0 (asynchronous mode))
    //Bit 5 - 0 (transmit disabled during init)
    //Bit 6 - 0 (selects 8 bit transmission))
    //Bit 7 - Don't care
    
    TXSTA2bits.BRGH = 1;
    TXSTA2bits.SENDB = 0;
    TXSTA2bits.TX9 = 0;
    TXSTA2bits.SYNC = 0;
    
    //auto baud detect is disabled.
    BAUDCON2bits.ABDEN = 0;
    
    //BRG16 selects 8 bit baud rate generator        
            
    BAUDCON2bits.BRG16 = 0;
    RCSTA2bits.SPEN = 1;
    RCSTA2bits.CREN = 1;
    //The following enables USART transmitter.        
            
    TXSTA2bits.TXEN = 1; 
            

    
}



