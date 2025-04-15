/*
 * File:   RPNMAIN.c
 * Author: Alina Matchette
 * Program for RPN calculator
 * Dependancies: xc.h, stdbool.h
 * 
 * Operation: use keypad, 4x4 to enter numbers, 2 digits, into the calculator, RPN style. then press an operator key
 * A - add
 * B - subtract
 * C - multiply
 * D - divide
 * Pound - stack push {ENTER}
 * Star - stack clear
 * Version - 1.0 original
 * Version - 1.1  - updated output function to return a value rather than set value in stack. this allows for original unfiltered output value to remain in the stack
 * Version - 1.2.1 - added while loop to improve input taking
 * version - 1.3 - fixed several errors that only recently appeared, including improper display, program resetting frequently, and issues with port C not outputting
 * Version - 1.3.1 - changed default stack levels full to display a 1, considering that in RPN to properly display how many numbers you are entering it would display 1 as you are entering first number, ect, not after
 * Version - 1.3.2 - changes stack to signed char, since output is always 1 byte wide
 * Created on April 7, 2025
 */



#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "C:/Program Files/Microchip/xc8/v3.00/pic/include/proc/pic18f47k42.h"
#pragma config WDTE = OFF 

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4



// functions, with names that indecate function. stack in name reffers to stack array, while the rest are operators with an a at the end to denote they require an array in
void adda(signed char s[], int c[]);
void suba(signed char s[], int c[]);
void mula(signed char s[], int c[]);
void diva(signed char s[], int c[]);
void stacku(signed char s[], int c[]);
void stackd(signed char s[], int c[]);
void stackc(signed char s[], int c[]);

void takeIn(signed char s[], int c[]);
int takeOut(signed char s[],int c[]);



signed char stack[4];
int ctrl[5]; //control array, first digit is operator, second digit is digits place exponent, third is stack levels full, fourth is wether to display stack level or x reg
/* C Controls
 * C[0] = Operator
 * C[1] = Digits Place
 * C[2] = Full Stack Levels
 * C[3] = Display Stack Levels (0) or Display Stack (1)
 * C[4] = Unused
 */


void main(void) {
    PORTB = 0; LATB = 0; ANSELB = 0; TRISB = 0;
    PORTC = 0; LATC = 0; ANSELC = 0; TRISC = 0b11110000; // port setup, ports C 4,5,6,7 as input rest of B and D as out
    PORTD = 0; LATD = 0; ANSELD = 0; TRISD = 0;
    stack[0] = -1;
    ctrl[2] = 1;
    while (1){ //main loop
         PORTD = takeOut(stack,ctrl);
        
        takeIn(stack,ctrl); //takes input from the user
       
        
        switch (ctrl[0]){ //checks operators and preforms proper calculations
            case 1: adda(stack,ctrl); break;
            case 2: suba(stack,ctrl); break;
            case 3: mula(stack,ctrl); break;
            case 4: diva(stack,ctrl); break;
            case 5: stacku(stack,ctrl); break;
            case 6: stackc(stack,ctrl); break;
        }
        
        ctrl[0] = 0;
          PORTD = takeOut(stack,ctrl);
      
        
        //display result
           __delay_ms(500);
    }
    
    
    return;
    }




void adda(signed char s[], int c[]){ //adding both x and y registers
    s[1] += s[0];
    stackd(s,c); //stack down
    return;
}
void suba(signed char s[], int c[]){ //subtracting both x and y registers
    s[1] -= s[0];
    stackd(s,c); //stack down
    return;
}
void mula(signed char s[], int c[]){ //multiplying both x and y registers
    s[1] *= s[0];
    stackd(s,c); //stack down
    return;
}
void diva(signed char s[], int c[]){ //diving both x and y registers
    s[1] /= s[0];
    stackd(s,c); //stack down
    return;
}
void stacku(signed char s[], int c[]){ //pushes the stack up one level
    
    for (int i= 2; i>=0; i--){   //using this for loop to iterrate through it and push values
        s[i+1] = s[i];
    }
    c[1] = 0;
    c[2] ++;
    c[3] = 0;
    return;
}
void stackd(signed char s[], int c[]){ // a similar method to pushing the stack up, but inverse

    for (int i= 0; i<=3; i++){           
        s[i-1] = s[i];
    }
    s[3] = 0; // this is required as the top level of the stack needs to be set to zero as nothing is in it anymore
    c[3] = 1;
    c[2] --; 
    return;
}
void stackc(signed char s[], int c[]){ //clears stack and resets all values
    for (int i = 0; i <=3; i++){
        s[i] = 0;
        c[i] = 0;
    }
    c[2] = 1;
    return;
}
void takeIn(signed char s[], int c[]){ //loosely based on the keypad input assembly code, taking input based on C and R values
    while (true){
    PORTBbits.RB3 = 0;
    PORTBbits.RB0 = 1;
    if (PORTCbits.RC4 ==1){
        switch(c[1]){
            case 0: 
                s[0] = 1;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                s[0] += 1;
                c[1] = 0;
                break;
        }
        return;
    }
    else if (PORTCbits.RC5 ==1){
        switch(c[1]){
            case 0: 
                s[0] = 4;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                s[0] += 4;
                c[1] = 0;
                break;
        }
        return;
    }
    else if (PORTCbits.RC6==1){
        switch(c[1]){
            case 0: 
                s[0] = 7;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                s[0] += 7;
                c[1] = 0;
                break;
        }
        return;
    }
    else if (PORTCbits.RC7 ==1){
        c[0] = 6;
        return;
    }
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 1;
    if (PORTCbits.RC4 ==1){
        switch(c[1]){
            case 0: 
                s[0] = 2;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                s[0] += 2;
                c[1] = 0;
                break;
        }
        return;
    }
    else if (PORTCbits.RC5 ==1){
        switch(c[1]){
            case 0: 
                s[0] = 5;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                s[0] += 5;
                c[1] = 0;
                break;
        }
        return;
    }
    else if (PORTCbits.RC6 ==1){
        switch(c[1]){
            case 0: 
                s[0] = 8;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                s[0] += 8;
                c[1] = 0;
                break;
        }
        return;
    }
    else if (PORTCbits.RC7 ==1){
        switch(c[1]){
            case 0: 
                s[0] = 0;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                c[1] = 0;
                break;
        }
        return;
    }
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 1;
    if (PORTCbits.RC4 ==1){
        switch(c[1]){
            case 0: 
                s[0] = 3;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                s[0] += 3;
                c[1] = 0;
                break;
        }
        return;
    }
    else if (PORTCbits.RC5 ==1){
        switch(c[1]){
            case 0: 
                s[0] = 6;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                s[0] += 6;
                c[1] = 0;
                break;
        }
        return;
    }
    else if (PORTCbits.RC6 ==1){
        switch(c[1]){
            case 0: 
                s[0] = 9;
                c[1] = 1;
                break;
            case 1:
                s[0] *=10;
                s[0] += 9;
                c[1] = 0;
                break;
        }
        return;
    }
    else if (PORTCbits.RC7 ==1){
        c[0] = 5;
        return;
    }
    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 1;
    if (PORTCbits.RC4 ==1){
        c[0] = 1;
        return;
    }
    else if (PORTCbits.RC5 ==1){
        c[0] = 2;
        return;
    }
    else if (PORTCbits.RC6 ==1){
        c[0] = 3;
        return;
    }
    else if (PORTCbits.RC7 ==1){
        c[0] = 4;
        return;
    }
}
    return;
}
int takeOut(signed char s[],int c[]){
    int tempOut = s[1];
    switch (c[3]){
        case 0:
            tempOut =  c[2];
            break;
        case 1:
            tempOut = s[0];
            break;
    } 
   return tempOut; 
}
