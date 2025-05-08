/*
 * Init file for Etch-A-Sketch main. for more information see main code header
 */


#include <xc.h>


#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
void INTERRUPT_Initialize (void);


void sys_init(void);

void sys_init(void) //sets up the ports in accordance with input and output in main function, in addition to weak pullup for interrupt
{
    PORTD = 0; TRISD = 0; ANSELD = 0; LATD = 0;
    PORTB = 0; TRISB = 0x01; ANSELB = 0x00; LATB = 0;
    PORTC = 0; TRISC = 0xFF; ANSELC = 0x00; LATC = 0;
    WPUB = 0xff;
}

void INTERRUPT_Initialize (void) //setup and init for interrupt
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



