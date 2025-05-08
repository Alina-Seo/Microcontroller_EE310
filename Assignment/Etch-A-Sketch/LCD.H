
#include <xc.h>
#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
void LCD_init();
void LCD_command(char );
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char ,char ,const char*);
void MSdelay(unsigned int );
void LCD_write(char pos,const char *msg);
void LCD_disp();

#define RS LATB1                   /* PORTD 0 pin is used for Register Select */
#define EN LATB2                   /* PORTD 1 pin is used for Enable */
#define ldata LATD                /* PORTB is used for transmitting data to LCD */

#define LCD_Port TRISD              
#define LCD_Control TRISB


void LCD_init()
{
    MSdelay(15);           /* 15ms,16x2 LCD Power on delay */
    LCD_Port = 0x00;       /* Set PORTD as output PORT for LCD data(D0-D7) pins */
    LCD_Control = 0x00;    /* Set PORTB as output PORT LCD Control(RS,EN) Pins */
    LCD_command(0x01);     /* clear display screen */
    LCD_command(0x38);     /* uses 2 line and initialize 5*7 matrix of LCD */
    LCD_command(0x0F);     /* display on cursor blinking */
    LCD_command(0x06);     /* increment cursor (shift cursor to right) */
}

void LCD_disp(){
LCD_Char(0b00100011);
LCD_command(0x10);
}

void LCD_command(char cmd )
{
    ldata= cmd;            /* Send data to PORT as a command for LCD */   
    RS = 0;                /* Command Register is selected */
    EN = 1;                /* High-to-Low pulse on Enable pin to latch data */ 
    NOP();
    EN = 0;
    MSdelay(4); 
}

void LCD_Char(char dat)
{
    ldata= dat;            /* Send data to LCD */  
    RS = 1;                /* Data Register is selected */
    EN=1;                  /* High-to-Low pulse on Enable pin to latch data */   
    NOP();
    EN=0;
    MSdelay(1);
}


void LCD_String(const char *msg)
{
    while((*msg)!=0)
    {       
      LCD_Char(*msg);
      msg++;    
        }
}

void LCD_write(char pos,const char *msg)
{
    pos += 0x80;
    LCD_command(pos);
    LCD_String(msg);

}


/*********************************Delay Function********************************/
void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
}