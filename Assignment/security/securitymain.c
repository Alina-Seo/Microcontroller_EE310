/*
 * File:   securitymain.c
 * Author: Alina Matchette
 *
 * A short program to control a safe based on user entered code. ideally uses photoresistors input however current config
 * is for buttons due to my photoresistors deciding that now is the perfect time to display that perfect resistors can exist in the real world
 * 
 * inputs: 2 photoresistors (or button) input for registers, connected to pins C2 and C3, 
 * Enter button connected to C4, and interrupt button connected to INT0
 * 
 * outputs: beeper connected to C6, motor connected to C5 through motor controller
 * 
 * dependencies: functions.h, xc.h (on git repo)
 * Version 1.0
 * Created on April 17, 2025, 1:31 PM
 */

// CONFIG1L


#include <./xc.h>
#include "functions.h"
#include "C:/Program Files/Microchip/xc8/v3.00/pic/include/proc/pic18f47k42.h"
#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
#pragma config WDTE = OFF

void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void) //code for interupt, when triggered beeps speaker 3 times
{
    if (PIR1bits.INT0IF == 1){
        for (int i =1;i<= 6;i++){
        PORTCbits.RC6  = !PORTCbits.RC6;
        __delay_ms(250);
        }
       PIR1bits.INT0IF =0;
       PORTCbits.RC6 = 0;     
    }
    
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

int code[2] = {2,3}; //setting up both the code and user entry arrays
int usercode[2];

void main(void) {
    PORTD = 0; LATD = 0; ANSELD = 0; TRISD = 0; //port setup and innit
    PORTC = 0; LATC = 0; ANSELC = 0; TRISC = 0x00011111;
    PORTCbits.RC6 = 0; 
    PORTDbits.RD7 = 1; //always on LED connected and turned on here
    INTERRUPT_Initialize();
while (1){
    
    if (PORTCbits.RC2 == 1){ //check first digit switch
        usercode[0] += 1;
        if (usercode[0] > 4){ //reset value if over 4, due to max entry being 4
        usercode[0] = 0;
        }
    PORTD = sevenseg(usercode[0]); //outputs number in the reg to the display
}
    else if (PORTCbits.RC3 == 1){ //check second digit switch
        usercode[1] += 1;
        if (usercode[1] > 4){
        usercode[1] = 0;
        }
        PORTD = sevenseg(usercode[1]);
    }
    else if (PORTCbits.RC4 == 1){ //when enter button pressed, checks arrays and 
        if (code[0] ==  usercode[0] &&code[1] ==  usercode[1]){
        PORTCbits.RC5 = 1; //trigger motor to open door on correct
        __delay_ms(2000);
        PORTCbits.RC5 = 0;
        }
        else{ //beep if not
        PORTCbits.RC6 = 1;
        __delay_ms(500);
        PORTCbits.RC6 = 0;
        }
        
        
        
    }


    
    __delay_ms(400); //input delay to avoid accidental entry
    
}    
    
    return;
}



    
    
