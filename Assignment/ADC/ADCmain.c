/*
 * The purpose of this program is to demonstrate how a 20x2 LCD can be interfaced with a PIC processor. 
 * Refer to the lecture slides for more details on the physical connections.
 * You may want to consider changing this code, as needed, depending on your hardware. 
 * Author: Farid Farahmand 
 */


// PIC18F46K42 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L


// CONFIG5L
         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

#include "xc.h" // must have this
#include "LCD.H"
#include "init.h"

#include "C:/Program Files/Microchip/xc8/v3.00/pic/include/proc/pic18f47k42.h"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4






/*****************************Main Program*******************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

#define Vref 3.3 // voltage reference 
int digital; // holds the digital value 
float voltage; // hold the analog value (volt))
char data[10];
// DO: Declare void ADC_Init
float footCandel;

void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void) //code for interrupt, triggers long beep when pressed
{
    if (PIR1bits.INT0IF == 1){
        
         //reset interrupt
        for (int i = 0; i<=60; i++)
        {
        PORTBbits.RB4 = !PORTBbits.RB4;
        __delay_ms(100);
        }
        PIR1bits.INT0IF =0;//reset interrupt
        PORTBbits.RB4 = 0;
        
        
    }
}

void main() {
    
    sys_init();
    ADC_Init(); 
    LCD_Init();
    INTERRUPT_Initialize();   
    while (1) {
        
        //DO: Set ADCON0 Go to start conversion
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO);  //Wait for conversion done
        
        digital = (ADRESH*256) | (ADRESL);/*Combine 8-bit LSB and 2-bit MSB*/
        // DO: define voltage = Vref/4096 (note that voltage is float type
        voltage = digital*((float)Vref/(float)(4096));
        

        footCandel = -13.28889*(voltage)+37.11978;
        sprintf(data,"%.2f",footCandel);
        strcat(data," ft candle ");	/*Concatenate result and unit to print*/
        LCD_write(0, "Light Level:");
        LCD_write(40,data);
    }
}




