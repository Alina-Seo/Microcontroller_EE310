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
void adda(int array[]);
void suba(int array[]);
void mula(int array[]);
void diva(int array[]);
void stacku(int array[]);
void stackd(int array[]);
void stackc(int array[]);

void takeIn(int s[], int c[]);
void takeOut(int s[],int c[]);



signed int stack[4] = {0,0,0,0};
int ctrl[4] = {0,0,0,0,0}; //control array, first digit is operator, second digit is digits place exponent, third is stack levels full, fourth is wether to display stack level or x reg



void main(void) {
    PORTB = 0; LATB = 0; ANSELB = 0; TRISB = 0b11110000; // port setup, ports B 4,5,6,7 as input rest of B and D as out
    PORTD = 0; LATD = 0; ANSELD = 0; TRISD = 0;
    
    while (1){ //main loop
    
        takeIn(stack,ctrl); //takes input from the user
        __delay_ms(1);
        
        switch (ctrl[0]){ //checks operators and preforms proper calculations
            case 1: adda(stack); break;
            case 2: suba(stack); break;
            case 3: mula(stack); break;
            case 4: diva(stack); break;
            case 5: stacku(stack); ctrl[3]++;ctrl[1] = 0; break;
            case 6: stackc(stack); break;
        }
        
        ctrl[0] = 0;
        takeOut(stack,ctrl);
        PORTD = stack[0];
        
        //display result
           __delay_ms(300);
    }
    
    
    return;
    }




void adda(int array[]){ //adding both x and y registers
    array[1] += array[0];
    stackd(array); //stack down
    return;
}
void suba(int array[]){ //subtracting both x and y registers
    array[1] -= array[0];
    stackd(array); //stack down
    return;
}
void mula(int array[]){ //multiplying both x and y registers
    array[1] *= array[0];
    stackd(array); //stack down
    return;
}
void diva(int array[]){ //diving both x and y registers
    array[1] /= array[0];
    stackd(array); //stack down
    return;
}
void stacku(int array[]){ //pushes the stack up one level
    
    for (int i= 2; i>=0; i--){   //using this for loop to iterrate through it and push values
        array[i+1] = array[i];
    }
    
    return;
}
void stackd(int array[]){ // a similar method to pushing the stack up, but inverse

    for (int i= 0; i<=3; i++){           
        array[i-1] = array[i];
    }
    array[3] = 0; // this is required as the top level of the stack needs to be set to zero as nothing is in it anymore
    
    return;
}
void stackc(int array[]){ //clears stack and resets all values
    for (int i = 0; i <=3; i++){
        array[i] = 0;
    }
    
    return;
}
void takeIn(int s[], int c[]){ //loosely based on the keypad input assembly code, taking input based on C and R values
    
    PORTBbits.RB3 = 0;
    PORTBbits.RB0 = 1;
    if (PORTBbits.RB4 ==1){
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
    else if (PORTBbits.RB5 ==1){
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
    else if (PORTBbits.RB6 ==1){
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
    else if (PORTBbits.RB7 ==1){
        c[0] = 6;
        return;
    }
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 1;
    if (PORTBbits.RB4 ==1){
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
    else if (PORTBbits.RB5 ==1){
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
    else if (PORTBbits.RB6 ==1){
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
    else if (PORTBbits.RB7 ==1){
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
    if (PORTBbits.RB4 ==1){
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
    else if (PORTBbits.RB5 ==1){
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
    else if (PORTBbits.RB6 ==1){
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
    else if (PORTBbits.RB7 ==1){
        c[0] = 5;
        return;
    }
    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 1;
    if (PORTBbits.RB4 ==1){
        c[0] = 1;
        return;
    }
    else if (PORTBbits.RB5 ==1){
        c[0] = 2;
        return;
    }
    else if (PORTBbits.RB6 ==1){
        c[0] = 3;
        return;
    }
    else if (PORTBbits.RB7 ==1){
        c[0] = 4;
        return;
    }
    
    return;
}
void takeOut(int s[],int c[]){
    int tempOut = 0;
    switch (c[3]){
        case 0:
            tempOut =  c[2];
            s[0] = tempOut; 
            break;
        case 1:
            tempOut = (s[0]>255)? 255 : s[0]; //checking if number is larger than max allowed and setting it
            if (tempOut < 0){ //checking if number is negative and then turning it into its 2s compliment
                tempOut *= -1;
                tempOut = 255 - tempOut;
            }
            s[0] = tempOut; 
            break;
    } 
      
}