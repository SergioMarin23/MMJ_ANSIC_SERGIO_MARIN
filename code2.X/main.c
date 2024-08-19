//Definir la frecuencia y tasa de baudios (Etiqueta de referencia)
#define FCY 7372800
#define FP  FCY/2
#define BAUDRATE 9600
#define BRGVAL ((FP/BAUDRATE)/16)-1 

// Configuración de bits (Fusibles)
#pragma config FNOSC = FRC  // Selecciona el oscilador interno r?pido (FRC)
#pragma config IESO = ON    // Habilita el cambio de oscilador interno/externo
#pragma config FWDTEN = OFF // Deshabilita el watchdog timer
#pragma config ICS = PGD3   // Selecciona el canal de comunicaci?n PGD3

//Librerias ANSI C
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//Librerias compilador
#include <libpic30.h>
#include "xc.h"
#include "p33FJ128MC802.h"

//Librerias propias
#include "lib/PIC33_GPIO_driver.h"

//Pines
#define Out_1   LATB_bits->PIN2
#define Out_2   LATB_bits->PIN3

// Definición de estados
typedef enum {
    ESTADO_INACTIVO,
    ESTADO_ENCENDIDO,
    ESTADO_CAMBIANDO_DIRECCION
} EstadoMotor;

// Variable global para almacenar el estado actual
EstadoMotor estadoActual = ESTADO_INACTIVO;

//Prototipo de Función
void HardwareInit(void);
void UART1Init(void);
char UART1ReceiveChar(void);
void ProcessCommand(char command);
void ShowMenu(void);
void UART1SendString(const char *str);
void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void);

// MAIN
void main(void) {
    HardwareInit();  // Inicializar el hardware
        
    while(1) {
        IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
        while(!U1STAbits.TRMT);
        ShowMenu();
        __delay_ms(1000);
        
    }
}

//Desarrollo de funciones
void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void) {
    while(!U1STAbits.URXDA);
    switch(U1RXREG){
        case 'S': // s ascii 115 
            ProcessCommand('S');
            break;
        case 'F': //a ascii 97 0 1000110101010
            ProcessCommand('F');
            break;
        case 'R': // d ascii 100
            ProcessCommand('R');
            break;
    }
    Out_1 = 1;
    __delay_ms(20);
    Out_1 = 0;
    IFS0bits.U1RXIF = 0;
}

//Configuración de hardware
void HardwareInit(void) {
    //Configuración del pin TX y RX
    RPINR18bits.U1RXR = 0b00001;  //Asigna pin RP1 como UART1 RX
    RPOR2bits.RP4R    = 0b00011;  //TX UART1 por RP4
    
    //Salidas apagadas inicialmente
    Out_1 = 0;
    Out_2 = 0;
    
    //Inicializar UART
    UART1Init();
}

//Función para configurar el UART1
void UART1Init(void) {
    U1MODEbits.STSEL = 0; // 1-Stop bit        
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode

    U1BRG = BRGVAL; // Baud Rate setting for monda

    U1STAbits.UTXISEL1 = 0; // Interrupt after one RX character is received   
    IEC0bits.U1RXIE = 1; // Enable UART RX interrupt

    //IPC3bits.U1TXIP = 5; // Set the transmition priority in 5 Lower than Reception 
    //IPC2bits.U1RXIP = 6; // Set the reception priority in 6 Higher than Transmition

    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable Transmition UART
}

//Función para mostrar el menú
void ShowMenu(void) {
    UART1SendString("\r\n--- Control de Motor DC ---\r\n");
    UART1SendString("Ingrese un comando:\r\n");
    UART1SendString("S - Detener el motor\r\n");
    UART1SendString("F - Avanzar el motor\r\n");
    UART1SendString("R - Revertir el motor\r\n");
    UART1SendString("Ingrese su eleccion: ");
}

//Función para enviar un string por UART
void UART1SendString(const char *str) {
    while(*str != '\0') {
        while(U1STAbits.UTXBF);  // Esperar a que el buffer de transmisión esté disponible
        U1TXREG = *str;          // Enviar carácter por UART
        str++;
    }
}

//Función para recibir un carácter por UART
/*
char UART1ReceiveChar(void) {
    while(!U1STAbits.URXDA);  // Esperar a que haya datos disponibles en el buffer de recepción
    return U1RXREG;           // Leer y devolver el carácter recibido
}
*/

//Función para procesar los comandos recibidos
void ProcessCommand(char command) {
    switch(command) {
        case 'S':   // Comando 'S' para detener el motor
            UART1SendString("\r\nMotor detenido.\r\n");
            LATBbits.LATB0 = 0;  // Ejemplo: apagar un pin (puedes adaptar esto a tu control de motor)
            break;
        
        case 'F':   // Comando 'F' para hacer girar el motor hacia adelante
            UART1SendString("\r\nMotor avanzando.\r\n");
            LATBbits.LATB0 = 1;  // Ejemplo: encender un pin (puedes adaptar esto a tu control de motor)
            break;

        case 'R':   // Comando 'R' para hacer girar el motor hacia atrás
            UART1SendString("\r\nMotor en reversa.\r\n");
            LATBbits.LATB1 = 1;  // Ejemplo: encender otro pin (puedes adaptar esto a tu control de motor)
            break;

        case 'P':   // Comando 'P' para cambiar la velocidad del motor
            UART1SendString("\r\nCambiar velocidad del motor.\r\n");
            // Aquí podrías agregar lógica adicional para cambiar la velocidad
            break;

        default:    // Comando no reconocido
            UART1SendString("\r\nComando no reconocido.\r\n");
            break;
    }
}