//*******************************************************
//Function Prototypes
//*******************************************************

void port_init(void);
void TMR0_INIT(void);
void buttons(void);
void interrupt low_priority TOGGLE_SEQUENCE(void);
void INT_INIT(void);
void SEND_NOTE(void);
void sequence_up(void);
void sequence_down(void);
void SEND_NOTEOFF(void);
void SEND_NOTEOFF_DOWN(void);