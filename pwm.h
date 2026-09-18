#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

// Initialise le Timer0 pour faire du PWM sur la pin PD6 (OC0A)
void PWM_Init(void);

// Change le rapport cyclique (0 à 255)
// 0 = 0%, 127 = 50%, 255 = 100%
void PWM_SetDuty(uint8_t duty);

#endif /* PWM_H_ */