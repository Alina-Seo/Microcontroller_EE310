/*
 * File:   RPNMAIN.c
 * Author: Alina Matchette
 * Program for RPN calculator
 * Dependancies: xc.h
 * 
 *
 *
 * Created on April 7, 2025, 9:06 PM
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
int takeIn(bool x);


bool tens = true;
int digitsAvalable = 2;
int stack[4] = {0,2,0,0};
int working;
int inputOp = 1;
int takeOut = 0;

void main(void) {
    while (true){
    
    working = takeIn(tens);
    if (working >= 10){
        stack[0] = working;
        tens = !tens;
    }
    else{
        stack[0] +=working;
        tens = !tens;
    }
    
    if (inputOp == 1){
            adda(stack);
    }
    else if (inputOp == 2){
            adda(stack);
    }
    else if (inputOp == 3){
            adda(stack);
    }
    else if (inputOp == 4){
            adda(stack);
    }
    else if (inputOp == 5){
            adda(stack);
    }
    
    
    takeOut = stack[0];
    takeOut = stack[0];
    inputOp = 0;
    
    }
    
    
    return;
    }




int takeIn(bool x){
    if (x == true){
        return 10;
        
    }
    else{
        return 1;
    }
    
}

void adda(int array[]){
    array[0] += array[1];
    return;
}
void suba(int array[]){
    array[0] -= array[1];
    return;
}
void mula(int array[]){
    array[0] *= array[1];
    return;
}
void diva(int array[]){
    array[0] /= array[1];
    return;
}
void stacku(int array[]){
    //int temp = array[0];
    for (int i= 2; i>=0; i--){   
        array[i+1] = array[i];
    }
    //array[0] = temp;
    return;
}
void stackd(int array[]){

    for (int i= 0; i<=3; i++){           
        array[i-1] = array[i];
    }
    array[3] = 0;
    return;
}
void stackc(int array[]){
    for (int i = 0; i <=3; i++){
        array[i] = 0;
    }
    return;
}