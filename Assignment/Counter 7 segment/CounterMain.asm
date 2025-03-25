;---------------------
; Title: Counter
;---------------------
; Program Details:
; seven segment display counter, when button a is pressed the counter increments, when button b is pressed counter decrements, and on both pressed it resets
    
    
; Inputs: Button A, Button b, set to RB0 and RB1 respectivly
; Output: 7 segment display
    
; Date:
; File Dependencies / Libraries: AssemblyConfig.inc, placed in header files 
;   
; Compiler: PICAS, v3
; Author: Alina Matchette, with loop segement and port innitialization written by Farid Farahmand
; Versions:
;       V1.0: Original
; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 

;---------------------
; Initialization
;---------------------
#include "./AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
Inner_loop  equ 256 // in decimal
Outer_loop  equ 256
 
;---------------------
; Definitions
;--------------------- 
#define SwitchA      PORTB,0
#define SwitchB      PORTB,1
REG10   equ     10h   // in HEX
REG11   equ     11h
 
;--------------------- 
    PSECT absdata,abs,ovrld        ; Do not change
    ORG          0                ;Reset vector


               ; Begin assembly at 0020H
ORG          0020H	       
_start:
    
    CALL _setupPortD
    CALL _setupPortB
    CALL CLRDISP
    
_main:    ; main loop of program
   

   
   
   BTFSC    SwitchA ;test to see if button A is pressed
   GOTO	    _up ;passes to counter increment code
   BTFSC    SwitchB ;test to see if button B is pressed
   GOTO	    _down   ;passes to counter decrement code
   GOTO	    _main
_up:
    BTFSC    SwitchB ;check to see if both are pressed
    GOTO     _reset ;since both are presse, passes to the reset code
    CALL    CountUp ;calls code to count up on display
    GOTO     _ending
_down:
    CALL    CountDown ;calls code to count down on display
    GOTO     _ending    
_reset:
    CALL    CLRDISP ;calls code to clear display
    GOTO     _ending
_ending:    ;this ending segment calls loop for timing functions and then passes back to main
    CALL    loopDelay
   CALL	    loopDelay 
   GOTO _main

    

    

    
org 0x80  ;this and below are all of the hex charectors for the display to read from

db  0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
    
CountUp: ;code for coutning up
    TBLRD+* ;read from table and increment, before moving to output
    MOVFF   TABLAT,PORTD
    MOVLW 0x8F ;value check, ensures table points does not wander higher than 0x89, the highest stored value
    CPFSGT  TBLPTRL
    RETURN 
    MOVLW 0x80
    MOVWF   TBLPTRL
    RETURN
    

CountDown:
    TBLRD*- ;read from table and decrement, before moving to output
    MOVFF   TABLAT,PORTD
    MOVLW 0x80 ;value check, ensures table points does not wander lower than 0x80, the lowest stored value
    CPFSLT  TBLPTRL
    RETURN  
    MOVLW 0x8F
    MOVWF   TBLPTRL
    RETURN
    
CLRDISP: ;writes 0x80 to table pointer, adress for display zerio, then returns
    MOVLW 0x80
    MOVWF   TBLPTRL
    TBLRD*
    MOVFF   TABLAT,PORTD
    RETURN

;-----The Delay Subroutine    
loopDelay: 
    MOVLW       Inner_loop
    MOVWF       REG10
    MOVLW       Outer_loop
    MOVWF       REG11
_loop1:
    DECF        REG10,1
    BNZ         _loop1
    MOVLW       Inner_loop ; Re-initialize the inner loop for when the outer loop decrements.
    MOVWF       REG10
    DECF        REG11,1 // outer loop
    BNZ        _loop1
    RETURN

_setupPortD:
    BANKSEL	PORTD ;
    CLRF	PORTD ;Init PORTA
    BANKSEL	LATD ;Data Latch
    CLRF	LATD ;
    BANKSEL	ANSELD ;
    CLRF	ANSELD ;digital I/O
    BANKSEL	TRISD ;
    MOVLW	0b00000000 ;Set RD[7:0] as output
    MOVWF	TRISD
    RETURN
 
_setupPortB:
    BANKSEL	PORTB ;
    CLRF	PORTB ;Init PORTC
    BANKSEL	LATB ;Data Latch
    CLRF	LATB ;
    BANKSEL	ANSELB ;
    CLRF	ANSELB ;digital I/O
    BANKSEL	TRISB ;
    MOVLW	0b11111111 ;Set RC[7:0] as inputs
    MOVWF	TRISB
    RETURN    
    
    END