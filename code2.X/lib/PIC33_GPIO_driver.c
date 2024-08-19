
#include "PIC33_GPIO_driver.h"

//Variables
volatile TRISA_bits_t  *const TRISA_bits  = ((volatile TRISA_bits_t *)TRISA_REG);
volatile TRISB_bits_t  *const TRISB_bits  = ((volatile TRISB_bits_t *)TRISB_REG);
volatile LATA_bits_t   *const LATA_bits   = ((volatile LATA_bits_t  *)LATA_REG );
volatile LATB_bits_t   *const LATB_bits   = ((volatile LATB_bits_t  *)LATB_REG );
volatile PORTA_bits_t  *const PORTA_bits  = ((volatile PORTA_bits_t *)PORTA_REG);
volatile PORTB_bits_t  *const PORTB_bits  = ((volatile PORTB_bits_t *)PORTB_REG);
volatile OSCCON_bits_t *const OSCCON_bits = ((volatile OSCCON_bits_t *)OSCCON_REG);
