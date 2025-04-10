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
 * Version - 1.2.1 - added while loop to improve inout taking
 * Created on April 7, 2025
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "C:/Program Files/Microchip/xc8/v3.00/pic/include/proc/pic18f47k42.h"


#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4



// functions, with names that indecate function. stack in name reffers to stack array, while the rest are operators with an a at the end to denote they require an array in
void adda(signed int s[], int c[]);
void suba(signed int s[], int c[]);
void mula(signed int s[], int c[]);
void diva(signed int s[], int c[]);
void stacku(signed int s[], int c[]);
void stackd(signed int s[], int c[]);
void stackc(signed int s[], int c[]);

void takeIn(int s[], int c[]);
int takeOut(int s[],int c[]);



signed int stack[4] = {0,0,0,0};
int ctrl[5] = {0,0,0,0,0}; //control array, first digit is operator, second digit is digits place exponent, third is stack levels full, fourth is wether to display stack level or x reg
/* C Controls
 * C[0] = Operator
 * C[1] = Digits Place
 * C[2] = Full Stack Levels
 * C[3] = Display Stack Levels (0) or Display Stack (1)
 * C[4] = Unused
 */


void main(void) {
    PORTC = 0; LATC = 0; ANSELC = 0; TRISC = 0b11110000; // port setup, ports C 4,5,6,7 as input rest of B and D as out
    PORTD = 0; LATD = 0; ANSELD = 0; TRISD = 0;
    
    while (true){ //main loop
    
        takeIn(stack,ctrl); //takes input from the user
        __delay_ms(1);
        
        switch (ctrl[0]){ //checks operators and preforms proper calculations
            case 1: adda(stack,ctrl); break;
            case 2: suba(stack,ctrl); break;
            case 3: mula(stack,ctrl); break;
            case 4: diva(stack,ctrl); break;
            case 5: stacku(stack,ctrl); ctrl[3]++;ctrl[1] = 0; break;
            case 6: stackc(stack,ctrl); break;
        }
        
        ctrl[0] = 0;
        
        PORTD = takeOut(stack,ctrl);
        
        //display result
           __delay_ms(300);
    }
    
    
    return;
    }




void adda(int s[], int c[]){ //adding both x and y registers
    s[1] += s[0];
    stackd(s,c); //stack down
    return;
}
void suba(int s[], int c[]){ //subtracting both x and y registers
    s[1] -= s[0];
    stackd(s,c); //stack down
    return;
}
void mula(int s[], int c[]){ //multiplying both x and y registers
    s[1] *= s[0];
    stackd(s,c); //stack down
    return;
}
void diva(int s[], int c[]){ //diving both x and y registers
    s[1] /= s[0];
    stackd(s,c); //stack down
    return;
}
void stacku(int s[], int c[]){ //pushes the stack up one level
    
    for (int i= 2; i>=0; i--){   //using this for loop to iterrate through it and push values
        s[i+1] = s[i];
    }
    c[2] ++;
    c[3] = 0;
    return;
}
void stackd(int s[], int c[]){ // a similar method to pushing the stack up, but inverse

    for (int i= 0; i<=3; i++){           
        s[i-1] = s[i];
    }
    s[3] = 0; // this is required as the top level of the stack needs to be set to zero as nothing is in it anymore
    c[3] = 1;
    c[2] --; 
    return;
}
void stackc(int s[], int c[]){ //clears stack and resets all values
    for (int i = 0; i <=3; i++){
        s[i] = 0;
        c[i] = 0;
    }
    
    return;
}
void takeIn(int s[], int c[]){ //loosely based on the keypad input assembly code, taking input based on C and R values
    while (true){
    PORTCbits.RC3 = 0;
    PORTCbits.RC0 = 1;
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
    PORTCbits.RC0 = 0;
    PORTCbits.RC1 = 1;
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
    PORTCbits.RC1 = 0;
    PORTCbits.RC2 = 1;
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
    PORTCbits.RC2 = 0;
    PORTCbits.RC3 = 1;
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
int takeOut(int s[],int c[]){
    int tempOut = s[1];
    switch (c[3]){
        case 0:
            tempOut =  c[2];
            break;
        case 1:
            tempOut = (s[0]>255)? 255 : s[0]; //checking if number is larger than max allowed and setting it
            if (tempOut < 0){ //checking if number is negative and then turning it into its 2s compliment
                tempOut *= -1;
                tempOut = 255 - tempOut;
            }
             
            break;
    } 
   return tempOut; 
}
