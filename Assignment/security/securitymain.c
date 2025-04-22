/*
 * File:   securitymain.c
 * Author: Zebus
 *
 * Created on April 17, 2025, 1:31 PM
 */


#include <./xc.h>
#include "C:/Program Files/Microchip/xc8/v3.00/pic/include/proc/pic18f47k42.h"
#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
#pragma config WDTE = OFF
int sevenseg(int a);
int code[2];
int usercode[2];

void main(void) {
    PORTD = 0; LATD = 0; ANSELD = 0; TRISD = 0;

    PORTC = 0; LATC = 0; ANSELC = 0; TRISC = 0x00011111;
    PORTCbits.RC6 = 0;
while (1){
    
    if (PORTCbits.RC2 == 0){
        usercode[0] += 1;
    PORTD = sevenseg(usercode[0]);
}
    else if (PORTCbits.RC3 == 0){
        usercode[1] += 1;
        PORTD = sevenseg(usercode[1]);
    }
    else if (PORTCbits.RC4 == 1){
        PORTCbits.RC6 = 1;
        PORTCbits.RC5 = 1;
        __delay_ms(2000);
        PORTCbits.RC6 = 0;
        PORTCbits.RC5 = 0;
    }


    
    __delay_ms(400);
    
}    
    
    return;
}


int sevenseg(int a){
   switch (a)
   {
       case 0:
       return 0b0111111;
       case 1:
       return 0x06;
       case 2:
       return 0x5B;
       case 3:
       return 0x4F;
       case 4:
       return 0x66;
       case 5:
       return 0x6D; 
       case 6:
       return 0x7D; 
       case 7:
       return 0x07; 
       case 8:
       return 0x7F; 
       case 9:
       return 0b1101111; 
   
   
   } 
    
    
}