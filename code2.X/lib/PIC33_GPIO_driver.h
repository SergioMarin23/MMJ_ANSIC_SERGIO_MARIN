/* 
 * File:   PIC33_GPIO_driver.h
 * Author: Sergio Andres Marin Patiño
 *
 * Created on 9 de agosto de 2024, 10:04 PM
 */

#ifndef PIC33_GPIO_DRIVER_H
#define	PIC33_GPIO_DRIVER_H

#include <stdint.h>

//Constantes con define
//Direccion de memoria para registros GPIO
#define TRISA_REG    0x02C0
#define TRISB_REG    0x02C8
#define LATA_REG     0x02C4
#define LATB_REG     0x02CC
#define PORTA_REG    0x02C2
#define PORTB_REG    0x02CA
#define OSCCON_REG   0x0742

//Tipos de datos
//Los registro para el puerto A
typedef struct {
    uint8_t PIN0   :1;
    uint8_t PIN1   :1;
    uint8_t PIN2   :1;
    uint8_t PIN3   :1;
    uint8_t PIN4   :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
}TRISA_bits_t;

typedef struct {
    uint8_t PIN0   :1;
    uint8_t PIN1   :1;
    uint8_t PIN2   :1;
    uint8_t PIN3   :1;
    uint8_t PIN4   :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
    uint8_t        :1;
}PORTA_bits_t;

typedef struct {
    uint8_t PIN0  :1;
    uint8_t PIN1  :1;
    uint8_t PIN2  :1;
    uint8_t PIN3  :1;
    uint8_t PIN4  :1;
    uint8_t       :1;
    uint8_t       :1;
    uint8_t       :1;
    uint8_t       :1;
    uint8_t       :1;
    uint8_t       :1;
    uint8_t       :1;
    uint8_t       :1;
    uint8_t       :1;
    uint8_t       :1;
    uint8_t       :1;
}LATA_bits_t;

//Los registro para el puerto B
typedef struct {
    uint8_t PIN0   :1;
    uint8_t PIN1   :1;
    uint8_t PIN2   :1;
    uint8_t PIN3   :1;
    uint8_t PIN4   :1;
    uint8_t PIN5   :1;
    uint8_t PIN6   :1;
    uint8_t PIN7   :1;
    uint8_t PIN8   :1;
    uint8_t PIN9   :1;
    uint8_t PIN10  :1;
    uint8_t PIN11  :1;
    uint8_t PIN12  :1;
    uint8_t PIN13  :1;
    uint8_t PIN14  :1;
    uint8_t PIN15  :1;
}TRISB_bits_t;

typedef struct {
    uint8_t PIN0   :1;
    uint8_t PIN1   :1;
    uint8_t PIN2   :1;
    uint8_t PIN3   :1;
    uint8_t PIN4   :1;
    uint8_t PIN5   :1;
    uint8_t PIN6   :1;
    uint8_t PIN7   :1;
    uint8_t PIN8   :1;
    uint8_t PIN9   :1;
    uint8_t PIN10  :1;
    uint8_t PIN11  :1;
    uint8_t PIN12  :1;
    uint8_t PIN13  :1;
    uint8_t PIN14  :1;
    uint8_t PIN15  :1;
}PORTB_bits_t;

typedef struct {
    uint8_t PIN0   :1;
    uint8_t PIN1   :1;
    uint8_t PIN2   :1;
    uint8_t PIN3   :1;
    uint8_t PIN4   :1;
    uint8_t PIN5   :1;
    uint8_t PIN6   :1;
    uint8_t PIN7   :1;
    uint8_t PIN8   :1;
    uint8_t PIN9   :1;
    uint8_t PIN10  :1;
    uint8_t PIN11  :1;
    uint8_t PIN12  :1;
    uint8_t PIN13  :1;
    uint8_t PIN14  :1;
    uint8_t PIN15  :1;
}LATB_bits_t;

//Los registro para oscilador
typedef struct {
    uint8_t OSWEN_CONF    :1;
    uint8_t LPOSCEN_CONF  :1;
    uint8_t               :1;
    uint8_t CF_CONF       :1;
    uint8_t               :1;
    uint8_t LOCK_CONF     :1;
    uint8_t IOLOCK_CONF   :1;
    uint8_t CLKLOCK_CONF  :1;
    uint8_t NOSC_CONF     :3;
    uint8_t               :1;
    uint8_t COSC_CONF     :3;
    uint8_t               :1;
}OSCCON_bits_t;

//Declarar el prototipo de las variables externas
extern volatile TRISA_bits_t  *const TRISA_bits;
extern volatile TRISB_bits_t  *const TRISB_bits;
extern volatile LATA_bits_t   *const LATA_bits;
extern volatile LATB_bits_t   *const LATB_bits;
extern volatile PORTA_bits_t  *const PORTA_bits;
extern volatile PORTB_bits_t  *const PORTB_bits;
extern volatile OSCCON_bits_t *const OSCCON_bits;

#endif	/* PIC33_GPIO_DRIVER_H */

