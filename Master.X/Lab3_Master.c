/*
 * File:   Lab3_Master.c
 * Author: Rodrigo García
 *
 * Created on 26 de julio de 2023, 05:07 PM
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
#include "LCD8bits.h"

//CONSTANTES
#define _XTAL_FREQ 8000000
#define RS RE0
#define EN RE1

//VARIABLES
uint8_t pot1;
uint8_t pot2;
uint8_t contador;
float voltaje1;
float voltaje2;
unsigned char valor[20];
unsigned char valor2[20];
unsigned char valor3[20];
char centenas;
char decenas;
char unidad;
char selection;
int a;
int b;

//PROTOTIPO DE FUNCIONES
void setup(void);

//MAIN
void main(void) {
    setup();
   
    while(1){
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(a);             // Mandarle una variable al slave 1
       pot1 = spiRead();        // Recibir el valor del pot del slave 1
      
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect
       
       
       
       PORTCbits.RC1 = 0;
       __delay_ms(1);
       
       spiWrite(b);             // Mandarle una variable al slave 2
       selection = spiRead();   // Recibir caracter 'c' ó 'd' del salve 2
       
       if (selection == 'c'){   
           spiWrite(b);         
           pot2 = spiRead();   // Si se recibió el caracter 'c', leer el valor del pot del slave 2
       }
       
       
       else if (selection == 'd'){
           spiWrite(b);
           contador = spiRead(); // Si se recibió el caracter 'd', leer el valor del contador del slave 2
       }
       
        __delay_ms(1);
        PORTCbits.RC1 = 1;
       
      voltaje1 = (pot1*((float)5/255)); // Conversión del valor del pot del slave 1 a valor de voltaje
       
       sprintf(valor, "%.2f", voltaje1);
        
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Pot1");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(valor);
        Lcd_Set_Cursor(2, 5);
        Lcd_Write_String("V ");
        
        voltaje2 = (pot2*((float)5/255)); // Conversión del valor del pot del slave 2 a valor de voltaje
       
       sprintf(valor2, "%.2f", voltaje2);
        
        Lcd_Set_Cursor(1,7);
        Lcd_Write_String("Pot2");
        Lcd_Set_Cursor(2,7);
        Lcd_Write_String(valor2);
        Lcd_Set_Cursor(2, 11);
        Lcd_Write_String("V ");
        
        

        Lcd_Set_Cursor(1,13);
        Lcd_Write_String("Cont");
        
        centenas = (contador/100);    // Convertir el valor del contador en centenas, decenas y unidades
        decenas = (contador/10)%10;
        unidad = contador%10;

        Lcd_Set_Cursor(2,14);
        Lcd_Write_Char(centenas + 48);
        Lcd_Write_Char(decenas + 48);
        Lcd_Write_Char(unidad + 48);
        
      
    }
    return;
}

//CONFIGURACIONES
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    PORTA = 0;
    
    TRISE = 0;
    PORTE = 0;
    
    TRISC1 = 0;
    TRISC2 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    
    // Configuración del LCD
    Lcd_Init();
    
   
    // Configuración del SPI
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}