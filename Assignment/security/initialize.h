

#include <xc.h>

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
void init(void) {
    PORTD = 0; LATD = 0; ANSELD = 0; TRISD = 0; //port setup and innit
    PORTC = 0; LATC = 0; ANSELC = 0; TRISC = 0x00011111;
    PORTB = 0;LATB = 0;ANSELB = 0; TRISB = 1;
    PORTD = 0b01000000;
    PORTCbits.RC6 = 0; 
    PORTDbits.RD7 = 1;
}

void INTERRUPT_Initialize (void) //setup and init for interupt
{

    INTCON0bits.IPEN = 1;
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;
    INTCON0bits.INT0EDG = 1;
    IPR1bits.INT0IP = 1;
    PIE1bits.INT0IE = 1;
    PIR1bits.INT0IF =0; 

    IVTBASEU = 0x00;
    IVTBASEH = 0x40;
    IVTBASEL = 0x08;
            
}