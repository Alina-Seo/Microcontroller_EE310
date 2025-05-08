/*
 * File:   Etch-A-Main.c
 * Author: Alina Matchette
 * 
 * Program: Etch-A-Sketch
 *      a program that, given a 16x2 LCD display, takes user input to draw on it in a facsimile of an etch-a-sketch. 
 *      user is free to draw by directing the blinking cursor using 3 buttons, listed below in Inputs, and using another to toggle if the device is drawing to the screen
 *      similar to an etch-a-sketch, a tilt sensor on interrupt RB0 detects if the device is tilted backward or shaken, which clears the screen with an animation
 *      program does not allow drawing off screen area
 * 
 * Inputs:
 *      Movement buttons: RC3 swaps between the layers of the LCD, RC4 moves the cursor left, RC5 moves the cursor right
 *      Draw button: RC2 switches between the program drawing on the screen and not drawing, as a toggle
 *      Interrupt: RB0 interrupts, connected to a tilt sensor
 * Outputs;
 *      Screen Data: Port D bits RD0:RD7 are connected to LCD D0:D7, in that order
 *      Screen Control: RB1 is set to RS pin on LCD, RB2 is set to Enable pin on LCD, RW on LCD is tied low and cannot be read from
 * Requires use of associated header files xc.h, LCD.H, init.h found in related github
 *      no other dependencies
 * Programmed and compiled with: MPlabX IDE v6.25, XC8 V3.00
 * Versions:
 *      1.0 - original
 *
 * Created on May 7, 2025, 8:02 PM
 */


#include "xc.h"
#include "LCD.H"
#include "init.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "C:/Program Files/Microchip/xc8/v3.00/pic/include/proc/pic18f47k42.h"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

signed char row = 0; //program uses these three values to know where the cursor is on the screen so it can move it and keep it within the bounds properly
signed char col = 0;
char draw = 0;

void movleft(void);
void movright(void);
void drawswap(void);
void layerswap(void);



void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void) // interupt on trigger clears screen
{

    if (PIR1bits.INT0IF == 1){ 
        LCD_write(40,"################"); //animation, writes # to screen like a wave motion from the bottom of the screen up
        __delay_ms(350);
        LCD_write(0,"################");
        __delay_ms(350);
        LCD_write(40,"                ");
        __delay_ms(350); //end animation
        LCD_command(0x01); //clear screen and reset position and draw values
                row = 0;
                col = 0;
                draw = 0;
        
        PIR1bits.INT0IF =0; //reset interrupt vector
        
          
    }
    
}

void main(void) {
    sys_init(); //calls init functions from related headers
    LCD_init();
    INTERRUPT_Initialize();
    

    
    while(1) //main loop
    {
        if (PORTCbits.RC3 == 1) //extended if statement checks for the button the user has pressed, and preforms the appropriate interaction
        {
            layerswap();   
        }
        else if (PORTCbits.RC2 == 1)
        {
            drawswap();
        }
        else if (PORTCbits.RC4 == 1)
        {
            movleft();
        }
        else if (PORTCbits.RC5 == 1)
        {
            movright();
        }
        
        
        if (col > 15) //checks the position of the cursor in the x direction and ensures it is unable to leave the screen
        {
           movleft();
            
        }
        else if (col < 0)
        {
           movright();
        }
        
        if (draw == 1) // draws on the screen if the draw toggle is set
        {
            LCD_disp();
        }
        
        
        
        
        
    }
    return;
}

//all functions have built in delay so that if they are not called then the program responds to button input quickly, but repeated function calls slows program so that unintended inputs are not entered

void movleft(void)
{
    LCD_command(0x10); //command to shift hd44780 cursor left then reduce col value for positioning
    col --;
    __delay_ms(200);
}

void movright(void)
{
    LCD_command(0x14); //command to shift hd44780 cursor right then increment col value for positioning
    col ++;
    __delay_ms(200);
}

void drawswap(void)
{
    draw = !draw; //simple command toggles the draw value
    __delay_ms(200);
}

void layerswap(void)
{
    row = !row; //swaps the value that the cursor is on in the y position
            LCD_command(0x02); //hd44780 command to move cursor to the "home position" or first col first row
           
           if (row == 1)
            {
                LCD_command(0xA8); //on checking if the row value is on the second row, it shifts the cursor to match
           }
            
            for (int i = 0; i < col; i++) {
                LCD_command(0x14); //uses the col value to then finally shift the cursor back to the horizontal position it is required to be in
            }
            __delay_ms(200);
}