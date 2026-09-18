#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "io.h"    // fournis (pinMode, getState, etc.)
#include "pwm.h"   // fournis (PWM_Init, PWM_SetDuty)

// ---------- Configuration ----------
#define BUTTON_DEBOUNCE_MS 30
#define BUTTON_SAMPLE_MS   10

// Duty levels (0..255) : 0%, 25%, 50%, 75%, 100%
static const uint8_t duty_levels[] = { 0, 64, 128, 192, 255 };
#define DUTY_LEVELS_COUNT (sizeof(duty_levels) / sizeof(duty_levels[0]))

// ---------- Prototypes ----------
void Task_Button(void *pvParameters);
void Task_LED(void *pvParameters);

// ---------- Globals ----------
static QueueHandle_t xDutyQueue = NULL;


static const struct DIGITAL_IO button_pin = { 'D', 2 };  // PD2
static const struct DIGITAL_IO led_pin    = { 'D', 6 };  // PD6 (OC0A)

// ---------- main ----------
int main(void)
{

    pinMode(button_pin, input_pullup);

    
    pinMode(led_pin, output);

   
    PWM_Init();
    PWM_SetDuty(0); // LED éteinte au départ

    // Création de la queue : on stocke 1 octet (uint8_t duty)
    xDutyQueue = xQueueCreate(1, sizeof(uint8_t));
    if (xDutyQueue == NULL) {
        // Pas assez de mémoire : bloquer (ou gérer l'erreur)
        while (1) { ; }
    }

    // Création des tâches
    xTaskCreate(Task_Button, "Button", 128, NULL, 2, NULL);
    xTaskCreate(Task_LED,    "LED",    128, NULL, 1, NULL);

    // Démarrage du planificateur FreeRTOS
    vTaskStartScheduler();

    // Si on arrive ici, le scheduler a échoué ; boucle infinie
    for (;;) { }
    return 0;
}

// ---------- Task_Button ----------
// Surveille l'état du bouton, gère le rebond (simple), incrémente le niveau et envoie le duty dans la queue
void Task_Button(void *pvParameters)
{
    (void) pvParameters;

    char last_state = 1; // avec pull-up : repos = 1 (HIGH), appui = 0 (LOW)
    uint8_t current_level_index = 0;
    uint8_t duty_to_send;

    for (;;)
    {
        char state = getState(button_pin); // retourne 1 ou 0

        // Détection front descendant : 1 -> 0 (appui)
        if (state == 0 && last_state == 1)
        {
            // anti-rebond simple
            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS));
            if (getState(button_pin) == 0) // toujours appuyé
            {
                // incrémente le niveau
                current_level_index = (current_level_index + 1) % DUTY_LEVELS_COUNT;
                duty_to_send = duty_levels[current_level_index];

                // Envoi non-bloquant (timeout 0)
                xQueueSend(xDutyQueue, &duty_to_send, 0);
            }
        }

        last_state = state;
        vTaskDelay(pdMS_TO_TICKS(BUTTON_SAMPLE_MS));
    }
}

// ---------- Task_LED ----------
// Attend les messages sur la queue, applique le duty au PWM
void Task_LED(void *pvParameters)
{
    (void) pvParameters;

    uint8_t received_duty = 0;

    for (;;)
    {
        // Bloquant : attend indéfiniment un nouveau duty
        if (xQueueReceive(xDutyQueue, &received_duty, portMAX_DELAY) == pdTRUE)
        {
            // Applique le nouveau duty (0..255)
            PWM_SetDuty(received_duty);
        }
    }
}
void vApplicationIdleHook(void)
{
    // laisser vide ou mettre un petit code
}
