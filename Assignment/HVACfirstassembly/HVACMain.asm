//-----------------------------
// Title: HVAC Assembly Program
//-----------------------------
// Purpose: takes user input temerature and runs a heating/cooling system to match said user specified temperature. 
//	    user should specify temperature using input keypad
// Dependencies: NONE
// Compiler: MPLABX version v6.20
// Author: Alina Matchette
// OUTPUTS: LEDs for heating, cooling system, heating, cooling system
// INPUTS: keypad for user input, temperature sensor for measuring enviromental temperature
// Versions:
//  	V1.0: 3/10/2025 - First version
//	v1.1: Add Control Reg Usage
//	V1.2: 3/12/2025 fixed issue where input sanitization considered certain numbers to be negative due to lower comparison value
//	v1.3: 3/12/2025 fixed temperature comparisons to allow for appropriate heating and cooling
//	v1.3.1: 3/13/2025 fixed gold program to allow for proper return if temp is equal
//-----------------------------

; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 

;---------------------
; Initialization
;---------------------
#include ".\AssemblyConfig.inc"
#include <xc.inc>
;----------------
; PROGRAM INPUTS
;----------------
;Inouts are as Follows. input keypad temperature, or refTempInout, is user specified reference temperature. measuredTempInput is enviromental

#define  measuredTempInput 	49; this is the input value
#define  refTempInput 		49 ; this is the input value

;---------------------
; Definitions
;---------------------
#define SWITCH    LATD,2  
#define LED0      PORTD,0
#define LED1	  PORTD,1

; below are the registers rthat will be used to store data for the program operation
REG20   equ     20h	// Storage of reference Temperature
REG21   equ     21h	// Storage of measured Temperature
REG22   equ     22h	// Storage of Control Word
REG24	equ	24h	// used to hold the value of division number during division
	
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG          0                ;Reset vector
    GOTO        _start

    ORG          0020H           ; Begin assembly at 0020H
_start:
    MOVLW	0x00	    //setting the ports for output
    MOVWF       TRISD,0
    
_main:
    
    MOVLW	refTempInput	    //moves the constants from inputs into the registers that are assigned to them above
    MOVWF	0x20,1   
    MOVLW	measuredTempInput
    MOVWF	0x21,1
    
    MOVLW	0x61	    //preparing to  do the division for the reference temperature, to be stored in 0x61 and 0x60 
    MOVWF	FSR0L	    //setting FSR to 0x61 for indirect incremeting of 0x61 udirn division
    MOVLW	refTempInput
    MOVWF	0x24,1	    //moving reference to divisor slot
    CALL	_div
    MOVLW	0x71	//preparing to  do the division for the measured temperature, to be stored in 0x71 and 0x70
    MOVWF	FSR0L	//setting FSR to 0x61 for indirect incremeting of 0x61 udirn division
    MOVLW	measuredTempInput
    MOVWF	0x24,1	//moving rmeasure to divisor slot
    MOVLW	0x3D	// sanitizes negative inputs by comparing them to max value
    CPFSLT	REG24
    NEGF	REG24	//negates negative values in divisor to get rid of their sign
    
    CALL	_div
    
    MOVLW	0x3D	// if negative treats temperature as though it was zero, to avoid underflow
    CPFSLT	REG21
    CLRF	REG21 
    
    MOVFF	0x20,WREG   //calling reference temp to WREG for comparison with measured temp, for controlling heater logic
    CPFSEQ	0x21	    
    GOTO    _hvacON //if measured and reference are equal, skip turning on the hvac system
    GOTO    _gold   //calls gold, which turns off HVAC, named after goldielocks for being "just right"
_hvacON:
    CPFSLT	0x21	//calls heating when too cold
    call    _heating
    CPFSGT	0x21	//calls heating when too hot
    call    _cooling
    MOVFF       REG22,PORTD
_cycleoff:
    NOP
    sleep   //to beb relaced with loop in working enviroment






_div:	    // function for doing division, starts at 10s digit due to expected inputs being under three digits
    MOVLW	10	//indecating 10s place division
_sub10:  
    CPFSGT	REG24	//checking to see if subtraction results in less than 10, indecating finished 10s place division
    GOTO	_ones	//moves to ones place division
    SUBWF	REG24,1	//subtracts as part of division
    INCF	INDF0,1  //increments times subtracted, to get the digit of the tens place
    GOTO    _sub10  //repeats tens place  division
_ones:	//sets up ones place division by setting FSR to correct register and setting wreg to 1, for ones place divison
    DECF	FSR0L,1
    MOVLW	1
    INCF	REG24	//increments REG 24 to allow for comparison to return proper number of times divided since it uses greater than logic
_sub1:    //ones place division, repeatedly subtracts 1 until target digit obtained
    CPFSGT	REG24
    GOTO	_enddiv	// if the target is obtained, goes to the finakized formatting segment for the registers
    SUBWF	REG24,1 //the subtraction as part of division
    INCF	INDF0,1  //increments digit counter
    GOTO    _sub1
    
 _enddiv:   //end div checks for edge case that results in tens place being 1 below where it should be and a 10 in the ones place
    MOVLW   0x0A    //moves 10 to wreg for comparison
    CPFSEQ  INDF0   //checks for edge case
    GOTO    _ending
    SUBWF   POSTINC0	//corrects for edge case by making obes place 0
    INCF    INDF0   // increments tenss place
_ending:
    RETURN  //returns to main function

_cooling:	//moves 0x04, or setting all low except for bit 2, to port d, setting cooling system
    MOVLW	0x04
    MOVWF       REG22,0
    return
_heating:   //moves 0x02, or setting all low except for bit 1, to port d, setting heating system
    MOVLW	0x02
    MOVWF       REG22,0
    return
_gold:	    //sets port d to zero, turning off HVAC
    MOVLW	0x00
    MOVWF       REG22,0
    GOTO	_cycleoff	

