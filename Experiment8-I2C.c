// CONFIG1 

#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin) 
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled) 
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled) 
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR) 
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled) 
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled) 
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin) 
#pragma config IESO = ON        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled) 
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled) 

// CONFIG2 
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off) 
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software) 
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR) 
#pragma config PLLEN = ON       // Phase Lock Loop enable (4x PLL is always enabled) 
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset) 
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.) 
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled) 
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled) 

// #pragma config statements should precede project file includes. 
// Use project enums instead of #define for ON and OFF. 

#include <xc.h>
/*
 * 
 */

/*
void i2c_main_init(void) 
{
    TRISCbits.TRISC0 = 1; // Make i2c pin C0 input 
    TRISCbits.TRISC1 = 1; // Make i2c pin C1 input
    ANSELCbits.ANSC0 = 0; // Make C0 digital not analog
    ANSELCbits.ANSC1 = 0; // Make C1 digital not analog
    SSP1CON1 = 0x28; // Enable and choose i2c main, 0x28 = 0b00101000
    SSP1ADD = 79; // clock frequency = Fosc/ (4*(SSP1ADD+1))
    SSP1STAT = 0x80; // Standard 100kHz speed, disable slew rate
    SSPCLKPPS = 0x10; // PPS input make SCL on C0
    SSPDATPPS = 0x11;
    // PPS input make SDA on C1
    RC0PPS = 0x12; // PPS output make SCL on C0
    RC1PPS = 0X13; // PPS output make SDA on C1
    PIR1bits.SSP1IF = 0; // clear I2C flag
}

void i2c_main_wait() {
    while(!PIR1bits.SSP1IF) {}
    PIR1bits.SSP1IF = 0;
}

void i2c_main_start() {
    SSP1CON2bits.SEN = 1;
    i2c_main_wait();
}

void i2c_main_stop() {
    SSP1CON2bits.PEN = 1;
    i2c_main_wait();
}

void i2c_main_write_data(int d) {
    SSP1BUF = d;
    i2c_main_wait();
}

void i2c_main_write(int a, int d)
{
    i2c_main_start();
    i2c_main_write_data(a); // Send the secondary address
    i2c_main_write_data(d); // Send data to the secondary
    i2c_main_stop();
}
*/

void i2c_secondary_init(void) {
    TRISCbits.TRISC0 = 1; // Make i2c pin C0 input 
    TRISCbits.TRISC1 = 1; // Make i2c pin C1 input
    ANSELCbits.ANSC0 = 0; // Make C0 digital not analog
    ANSELCbits.ANSC1 = 0; // Make C1 digital not analog
    SSPCLKPPS = 0x10; // PPS output make SCL on C0
    SSPDATPPS = 0x11; // PPS output make SDA on C1
    RC0PPS = 0x12; // PPS output make SCL on C0
    RC1PPS = 0x13; // PPS output make SDA on C1
    SSP1STAT = 0x80; // Standard 100kHz speed, disable slew rate
    SSP1ADD = 0x92; // Secondary Address * 2 = 0x92 (Secondary Address = 0x49)
    SSP1CON1 = 0x36; // Enable, release clock, and choose i2c secondary with 7-bit address
    SSP1CON2 = 0x01; //Enable clock stretching 
    PIR1bits.SSP1IF = 0; // clear I2C flag
}

void i2c_secondary_poll(int p1, int p2) {
    int i2c_state;
    int temp;
    int data;
    i2c_state = SSP1STAT & 0b00101101; // Zero all bits except D_nA, S, R_nW, and BF bits
    switch(i2c_state) {
        
        // Main sent the start bit and a write command. The buffer is full and has an address
        case 0b00001001: // D_nA=0 S=1 R_nW=0 BF=1
            temp = SSP1BUF;
            break;
            
        // Main sent the start bit and a write command. The buffer is full and has data
        case 0b00101001: // D_nA=1 S=1 R_nW=0 BF=1
            data = SSP1BUF;
            break;
            
        // Main sent the start bit and a read command. The buffer is full and has an address
        case 0b00001101: // D_nA=0 S=1 R_nW=1 BF=1
            temp = SSP1BUF;
            // Because it was a read command, you need to send data back to the Main
            while(SSP1STATbits.BF) {}
            SSP1BUF = p1;
            break;
            
        // When you send data back, the SSP1IF flag is set. No start, nothing in the buffer, still read and still data
        case 0b00100100: // D_nA=1 S=0 R_nW=1 BF=0
            break;
            
        // Main sent the start bit because it is requesting more than one byte of data, still data but R_nW undetermined
        case 0b00101000: // D_nA=1 S=1 R_nW=0 BF=1
        case 0b00101100: // D_nA=1 S=1 R_nW=1 BF=1
            // Send another byte
            if (!SSP1CON1bits.CKP) {
                SSP1BUF = p2;
            }
            break;
            
        // Any other state is due to an error, errors could be handled by checking error bits.
        default:
            while(1) {}
            break;      
    }
    SSP1CON1bits.CKP = 1;
}

void main(void) 
{ 
    OSCCON = 0x70; // = 0b01110000
    
    /* Code for sig 1 & 2
    i2c_main_init();
    
    int a = 0x5E; //94; // 47*2
    int d = 0x1B; //27; // 27*2
    i2c_main_write(a, d);
    */
    
    i2c_secondary_init();
      
    while(1) {
        
        /* Code for sig 1 & 2
        i2c_main_write(a, d);
        
        for(int i=0; i<10000; i++) {} //Delay so that we can see it better
        
        d++;
        */
        
        ///* Code for sig 3 & 4
        // Every communication the Master starts, 
        // sets the SSP1IF flag
        if (PIR1bits.SSP1IF) {
            PIR1bits.SSP1IF = 0;
            i2c_secondary_poll(255, 0); //i2c_secondary_poll(85, 170); // i2c_secondary_poll(d1, d2)
        }
        //*/
    }
    
    return;
}
