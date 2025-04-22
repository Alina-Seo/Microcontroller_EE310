/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   functions
 * Author: Alina Matchette
 * Comments: this file is used for the security main project it comes with on github, see main file securitymain for more details
 * Revision history: 
 * 1.0 - original
 * 1.1 -added interrupt function
 */


#include <xc.h>

void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void) //code for interrupt, triggers long beep when pressed
{
    if (PIR1bits.INT0IF == 1){
        
        PORTCbits.RC6  = 1;
        __delay_ms(2000);
        
       PIR1bits.INT0IF =0; //reset interrupt
       PORTCbits.RC6 = 0;     
    }
    
}

int sevenseg(int a); // changes numerical binary input to code that can be read to CC 7 segment LED display
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
       default:
       return 0b01000000;
   
   } 
}
