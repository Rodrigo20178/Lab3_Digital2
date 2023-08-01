/*
 * File:   Lab3_Slave.c
 * Author: rodri
 *
 * Created on 26 de julio de 2023, 05:20 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//LIBRERÍAS
#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#include "SPI.h"
#include "ADC.h"

//CONSTANTES
#define _XTAL_FREQ 8000000

//VARIABLES
uint8_t pot;
float voltaje;

//PROTOTIPO DE FUNCIONES
void setup(void);

//INTERRUPCIONES
void __interrupt() isr(void){
   if(SSPIF == 1){ // interrupción del SPI
        spiWrite(PORTD); // Escribir valor del puerto D en el SPI
        SSPIF = 0;
    }
}

//MAIN
void main(void) {
    setup();

    while(1){
        if(ADCON0bits.GO == 0)
        {
            PORTD = ADRESH; // El valor del ADC va al puerto D
            
            __delay_us(50);
            ADCON0bits.GO = 1;
        }
       
    }
    return;
}

//CONFIGURACIONES
void setup(void){
    ANSEL = 0x01;
    ANSELH = 0x00;
    
    TRISAbits.TRISA0 = 1;

    
    TRISB = 0;
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
    
    TRISE = 0;
    PORTE = 0;
    
    // Configuración del ADC
    adc_init(1);
    ADCON0bits.CHS = 0;     //canal 0
    __delay_us(100);
    
    ADCON0bits.GO = 1;
    
    // Confiiguración del oscilador
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
    // COnfiguración de interrupciones
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
   
    // Configuración del SPI
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}