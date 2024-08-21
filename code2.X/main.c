//Definir la frecuencia y tasa de baudios (Etiqueta de referencia)
#define FCY 7372800
#define FP  FCY/2
#define BAUDRATE 9600
#define BRGVAL ((FP/BAUDRATE)/16)-1 

// Configuraci�n de bits (Fusibles)
#pragma config FNOSC = FRC  // Selecciona el oscilador interno r�pido (FRC)
#pragma config IESO = ON    // Habilita el cambio de oscilador interno/externo
#pragma config FWDTEN = OFF // Deshabilita el watchdog timer
#pragma config ICS = PGD3   // Selecciona el canal de comunicaci�n PGD3

// Librer�as ANSI C
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Librer�as del compilador
#include <libpic30.h>
#include "xc.h"
#include "p33FJ128MC802.h"

// Librer�as propias
#include "lib/PIC33_GPIO_driver.h"

//Pines
#define Out_1   LATB_bits->PIN2
#define Out_2   LATB_bits->PIN3

// Definici�n de estados
typedef enum {
    ESTADO_INACTIVO,
    ESTADO_ENCENDIDO,
    ESTADO_CAMBIANDO_DIRECCION
} EstadoMotor;

// Variable global para almacenar el estado actual
EstadoMotor estadoActual = ESTADO_INACTIVO;

//Prototipo de Funci�n
void HardwareInit(void);
void UART1Init(void);
void ProcessCommand(char command);
void ShowMenu(void);
void UART1SendString(const char *str);
void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void);


// MAIN
void main(void) {
    HardwareInit();  // Inicializar el hardware
        
    while(1) {
        ShowMenu();         // Mostrar el men� en la consola
        __delay_ms(2000);   // Esperar un momento antes de recibir el siguiente comando
    }
}


//Desarrollo de funciones

void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void) {
    char ReceivedChar;

    // Verificar errores de recepci�n
    if (U1STAbits.FERR == 1) { // Error de framing
        IFS0bits.U1RXIF = 0;   // Limpiar la bandera de interrupci�n
        return;
    }
    
    if (U1STAbits.OERR == 1) { // Error de overrun
        U1STAbits.OERR = 0;    // Limpiar el error de overrun para continuar recibiendo
        IFS0bits.U1RXIF = 0;   // Limpiar la bandera de interrupci�n
        return;
    }

    // Verificar si hay datos recibidos
    if (U1STAbits.URXDA == 1) {
        Out_1 = 1;
        ReceivedChar = U1RXREG;   // Leer el car�cter recibido
        ProcessCommand(ReceivedChar); // Procesar el comando recibido
    }

    IFS0bits.U1RXIF = 0;  // Limpiar la bandera de interrupci�n de recepci�n UART
}

//Configuraci�n de hardware
void HardwareInit(void) {
    //Configuraci�n del pin TX y RX
    RPINR18bits.U1RXR = 0b00001;  // Asigna pin RP1 como UART1 RX
    RPOR2bits.RP4R    = 0b00011;  // TX UART1 por RP4
    
    // Salidas apagadas inicialmente
    Out_1 = 0;
    Out_2 = 0;
    
    // Inicializar UART
    UART1Init();
}

//Funci�n para configurar el UART1
void UART1Init(void) {
    U1MODEbits.STSEL = 0;  // 1-Stop bit        
    U1MODEbits.PDSEL = 0;  // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0;  // Auto-Baud disabled
    U1MODEbits.BRGH = 0;   // Standard-Speed mode
    U1MODEbits.UEN = 0;    // UxTX y UxRX habilitados y en uso

    U1BRG = (uint16_t)BRGVAL; // Configuraci�n de la tasa de baudios

    U1STAbits.URXISEL = 0;  // Interrupci�n despu�s de un car�cter RX recibido
    IEC0bits.U1RXIE = 1;    // Habilitar la interrupci�n de RX UART

    U1MODEbits.UARTEN = 1;  // Habilitar UART
    U1STAbits.UTXEN = 1;    // Habilitar transmisi�n UART
}

//Funci�n para mostrar el men�
void ShowMenu(void) {
    UART1SendString("\r\n--- Control de Motor DC ---\r\n");
    UART1SendString("Ingrese un comando:\r\n");
    UART1SendString("S - Detener el motor\r\n");
    UART1SendString("F - Avanzar el motor\r\n");
    UART1SendString("R - Revertir el motor\r\n");
    UART1SendString("Ingrese su eleccion: ");
}

//Funci�n para enviar un string por UART
void UART1SendString(const char *str) {
    while(*str != '\0') {
        while(U1STAbits.UTXBF);  // Esperar a que el buffer de transmisi�n est� disponible
        U1TXREG = *str;          // Enviar car�cter por UART
        str++;
    }
}

// Funci�n para procesar comandos
void ProcessCommand(char command) {
    switch (estadoActual) {
        case ESTADO_INACTIVO:
            if (command == 'F') {  // Comando 'F' para encender el motor hacia adelante
                UART1SendString("\r\nMotor avanzando.\r\n");
                Out_1 = 1;  // Encender el pin para girar hacia adelante
                estadoActual = ESTADO_ENCENDIDO;
            } else if (command == 'R') {  // Comando 'R' para encender el motor hacia atr�s
                UART1SendString("\r\nMotor en reversa.\r\n");
                Out_2 = 1;  // Encender el pin para girar hacia atr�s
                estadoActual = ESTADO_ENCENDIDO;
            } else {
                UART1SendString("\r\nComando no reconocido.\r\n");
            }
            break;

        case ESTADO_ENCENDIDO:
            if (command == 'S') {  // Comando 'S' para detener el motor
                UART1SendString("\r\nMotor detenido.\r\n");
                Out_1 = 0;  // Apagar el pin para detener el motor
                Out_2 = 0;  // Apagar el pin para detener el motor
                estadoActual = ESTADO_INACTIVO;
            } else if (command == 'F' || command == 'R') {  // Comandos para cambiar direcci�n
                UART1SendString("\r\nCambiando direcci�n.\r\n");
                estadoActual = ESTADO_CAMBIANDO_DIRECCION;
            } else {
                UART1SendString("\r\nComando no reconocido.\r\n");
            }
            break;

        case ESTADO_CAMBIANDO_DIRECCION:
            if (command == 'F') {  // Comando 'F' para girar hacia adelante
                UART1SendString("\r\nMotor avanzando.\r\n");
                Out_1 = 1;  // Encender el pin para girar hacia adelante
                Out_2 = 0;  // Apagar el pin para girar hacia atr�s
                estadoActual = ESTADO_ENCENDIDO;
            } else if (command == 'R') {  // Comando 'R' para girar hacia atr�s
                UART1SendString("\r\nMotor en reversa.\r\n");
                Out_1 = 0;  // Apagar el pin para girar hacia adelante
                Out_2 = 1;  // Encender el pin para girar hacia atr�s
                estadoActual = ESTADO_ENCENDIDO;
            } else if (command == 'S') {  // Comando 'S' para detener el motor
                UART1SendString("\r\nMotor detenido.\r\n");
                Out_1 = 0;  // Apagar el pin para detener el motor
                Out_2 = 0;  // Apagar el pin para detener el motor
                estadoActual = ESTADO_INACTIVO;
            } else {
                UART1SendString("\r\nComando no reconocido.\r\n");
            }
            break;

        default:
            UART1SendString("\r\nEstado desconocido.\r\n");
            break;
    }
}