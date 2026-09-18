#include <avr/io.h>
#include "pwm.h"

void PWM_Init(void)
{
    // 1. Configurer PD6 (OC0A) en SORTIE
    DDRD |= (1 << PD6);

    /* 2. Configuration du Timer 0 (8 bits)
     * WGM00 & WGM01 = 1 : Mode Fast PWM
     * COM0A1 = 1    : Clear OC0A on Compare Match (Mode non-inversé)
     */
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);

    /* 3. Configuration de l'horloge (Prescaler)
     * CS01 & CS00 = 1 : Prescaler /64
     * Fréquence PWM = 16MHz / (64 * 256) ≈ 976 Hz (Parfait pour une LED)
     */
    TCCR0B = (1 << CS01) | (1 << CS00);
    
    // Initialisation à 0 (LED éteinte)
    OCR0A = 0;
}

void PWM_SetDuty(uint8_t duty)
{
    // Le registre OCR0A contrôle la largeur de l'impulsion sur PD6
    OCR0A = duty;
}