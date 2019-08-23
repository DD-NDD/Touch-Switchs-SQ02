/*
 * File:   led.c
 * Author: Yobikid
 *
 * Created on May 30, 2019, 8:17 PM
 */
#include "led.h"
#include"mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/pin_manager.h"
#include <stdbool.h>
// fix l?i m?ch m?i
void LED_1_SetHigh()
{
    LED_1_B_SetHigh();
    LED_1_R_SetLow();
}
void LED_1_SetLow()
{
    LED_1_R_SetHigh();
    LED_1_B_SetLow();
}

void LED_0_SetHigh()
{
    LED_0_B_SetHigh();
    LED_0_R_SetLow();
}
void LED_0_SetLow()
{
    LED_0_R_SetHigh();
    LED_0_B_SetLow();
}

void LED_PROCESS(uint8_t x)
{
    switch(x)
    {
        case 0:
        {
            LED_1_SetHigh();LED_0_SetHigh();
            break;
        }
        case 1:
        {
            LED_1_SetHigh();LED_0_SetLow();
            break;
        }
        case 2:
        {
            LED_1_SetLow();LED_0_SetHigh();
            break;
        }
        case 3:
        {
            LED_1_SetLow();LED_0_SetLow();
            break;
        }       
    }
}