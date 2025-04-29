

#include <xc.h>


#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
void INTERRUPT_Initialize (void);
void ADC_Init(void);

void sys_init(void);

void sys_init(void)
{
PORTD = 0; TRISD = 0; ANSELD = 0; LATD = 0;
PORTB = 0; TRISB = 0x01; ANSELB = 0; LATB = 0;
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



void ADC_Init(void)
{
    //------Setup the ADC
    //DO: using ADCON0 set right justify
    //DO: Using ADCON0 set ADCRC Clock
    ADCON0bits.FM = 1;
    ADCON0bits.CS = 1;
    //DO: Set RA0 to input
    //DO: Set RA0 to analog
    TRISAbits.TRISA0 = 1; 
    ANSELAbits.ANSELA0 =1;
    //DO: Set RA0 as Analog channel in ADC ADPCH
    //DO: set ADC CLOCK Selection register to zero
    ADPCH = 0;
    ADCLK = 0;
    //DO: Clear ADC Result registers
    
    //DO: set precharge select to 0 in register ADPERL & ADPERH
    ADPREL = 0;
    ADPREH = 0;
    //DO: Set qcquisition LOW and HIGH bytes to zero
    ADACQL = 0;
    ADACQH = 0;
    //DO: Turn ADC On on register ADCON0
    ADCON0bits.ON =1;
}

