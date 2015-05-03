// This is example "SPI_SLAVE" code
// for the pic16f877a using the XC8 compiler in MPLAB X
// by Peter Matthews
//

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// Set Clock Freq. & Delays
#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 4000000
//    #define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/20000000.0)))
//    #define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/20000.0)))
#endif

// SPI Ports
#define SCK RC3
#define SDI RC4
#define SDO RC5

void main(void);
void pic_init(void);
void spi_init(void);
void spi_reset(void);
void timer_on(void);
void timer_off(void);


/****************************************************************************
* main()
****************************************************************************/
void main(){
    unsigned char dummy;

    pic_init();
    spi_init();

    while(1)
    {
        while(BF==0)      // Wait for SSPBUF Reception complete
        {
            timer_on();
        }
        timer_off();
        dummy = SSPBUF;                     // Get data byte
        PORTD = ~dummy;
    }
}

/****************************************************************************
* pic_init()
****************************************************************************/
void pic_init(){
    ADCON1 = 7;
//    nRBPU = 0;
    PORTD = 0xff;
    TRISD = 0;
/* Timer Interupt Frequency
* f = XTAL_FREQ / 4 * prescalar * Timer ! Resolution
* f = XTAL_FREQ / 4 * 8 * 65536 = 2Hz   */
// Timer 1 prescaler 1:8
    T1CONbits.T1CKPS1 = 0;
    T1CONbits.T1CKPS0 = 0;
}

/****************************************************************************
* spi_init()
****************************************************************************/
void spi_init(void) {
    INTCON = 0;         // Disable all interrupts
    TRISC3 = 1;         // SCK=RC3 is the serial clock
    TRISC4 = 1;         // SDI=RC4 is serial data input
    PORTCbits.RC5 = 1;
    TRISC5 = 0;         // SDO=RC5 is serial data output
    SSPEN = 0;          // Allow Programming of serial port
    SMP = 0;            // Input data sampled at middle data output time
    CKP = 1;            // Idle state for clock is a HIGH level
    CKE = 1;            // Transmit occurs on idle to active clock state
    SSPM3 = 0;
    SSPM2 = 1;
    SSPM1 = 0;
    SSPM0 = 1;
    SSPIF = 0;
    SSPEN = 1;          // End programming and Start serial port
}

/****************************************************************************
* isr()
****************************************************************************/
void interrupt isr(void)
{
// Check all interupts - and reset them all !!
    if (PIR1bits.TMR1IF  == 1)  // Check for Timer overflow
    {
        PIR1bits.TMR1IF   = 0;  // Reset interupt flag
        spi_reset();
        timer_off();
    }
}

/****************************************************************************
*  spi_reset()
****************************************************************************/
void spi_reset(void)
{
unsigned char dummy;
        SSPEN             = 0;  //  Reset SPI module
        SSPEN             = 1;  //  Reset SPI module
        dummy             = SSPBUF;
        SSPIF             = 0;
        SSPEN             = 0;  //  Reset SPI module
        SSPEN             = 1;  //  Reset SPI module
}

/****************************************************************************
*  timer_on()
****************************************************************************/
void timer_on(void)
{
        PIE1bits.TMR1IE   = 1;  // Timer 1 overflow interupt enabled
        T1CONbits.TMR1ON  = 1;  // Timer 1 on
        INTCONbits.PEIE   = 1;  // Enable Peripheral interupts
        INTCONbits.GIE    = 1;  // Global Interupt enable
}
/****************************************************************************
*  timer_off()
****************************************************************************/
void timer_off(void)
{
        PIR1bits.TMR1IF   = 0;  // Reset interupt flag
        PIE1bits.TMR1IE   = 0;  // Timer 1 overflow interupt enabled
        T1CONbits.TMR1ON  = 0;  // Timer 1 on
        INTCONbits.PEIE   = 0;  // Enable Peripheral interupts
        INTCONbits.GIE    = 0;  // Global Interupt enable
}
