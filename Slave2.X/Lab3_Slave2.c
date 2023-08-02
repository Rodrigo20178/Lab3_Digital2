/*
 * File:   Lab3_Slave2.c
 * Author: rodri
 *
 * Created on 27 de julio de 2023, 04:35 PM
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
#include "IOCB.h"

//CONSTANTES
#define _XTAL_FREQ 8000000

//VARIABLES
uint8_t flag = 0;
uint8_t contador = 0;
uint16_t x = 0;

//PROTOTIPO DE FUNCIONES
void setup(void);

//INTERRUPCIONES
void __interrupt() isr(void){
   if(SSPIF == 1){                  // Interrupción de SPI
       if (flag == 0){
           ADCON0bits.GO = 1;       // Iniciar conversión ADC
           while(ADCON0bits.GO);    
           __delay_us(100);
           PORTD = ADRESH;          // Guardar valor del ADC al puerto D
           
           x = spiRead();           // Se almacena en la variable x lo que tenga el SPI
           spiWrite('c');           // Se manda un caracter 'c' en el SPI
           __delay_ms(1);
           x = spiRead();           // Se almacena de nuevo el valor del SPI
           spiWrite(PORTD);         // Se manda el valor del puerto D en el SPI
       }
       if (flag == 1){              // Si la bandera es igual a 1, se cumplen los siguientes puntos
           x = spiRead();           // Se almacena en la variable x lo que tenga el SPI
           spiWrite('d');           // Se manda un caracter 'd' en el SPI
            __delay_ms(1);
           x = spiRead();           // Se almacena de nuevo el valor del SPI
           spiWrite(contador);      // Se manda el valor del contador al SPI
           flag = 0;                // Apagamos bandera
       }
       
       if (INTCONbits.RBIF){        // Interrupcion IOCB
           if (PORTBbits.RB0 == 0){
               contador++;          // Si se presiona el primer botón, se incrementa el contador
               flag = 1;            // Se setea la bandera para que el valor del contador se vaya modificando en el SPI
           }
           else if (PORTBbits.RB1 == 0){
               contador--;          // Si se presiona el segundo botón, se decrementa el contador
               flag = 1;            // Se setea la bandera para que el valor del contador se vaya modificando en el SPI
           }
           INTCONbits.RBIF = 0;
       }
        
        SSPIF = 0;
    }
}

//MAIN
void main(void) {
    setup();

    while(1){
        
       
    }
    return;
}

//CONFIGURACIONES
void setup(void){
    ANSEL = 0x01;
    ANSELH = 0x00;
    
    TRISAbits.TRISA0 = 1;

    
    TRISB = 0b00000011;        // RB0 & RB1 como entradas
    PORTB = 0;                 // Se limpia el PORTB
    
    TRISD = 0;
    PORTD = 0;
    
    TRISE = 0;
    PORTE = 0;
    
    // Configuración IOCB
    OPTION_REGbits.nRBPU = 0;
    ioc_int(0);
    ioc_int(1);
    
    // COnfiguración ADC
    adc_init(1);
    ADCON0bits.CHS = 0;     //canal 0
    __delay_us(100);
    
    ADCON0bits.GO = 1;
    
    // Configuración del oscilador
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
    //Configuración de interrupciones
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupción de PORTB
   
    // Configuración SPI
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}