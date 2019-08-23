#include<xc.h>
#include<stdint.h>
#include"relay.h"
#include"mcc_generated_files/pin_manager.h"
void RELAY_PROCESS(uint8_t x)
{
    switch(x)
    {
        case 0:
        {
            RL1_SetHigh();RL0_SetHigh();
            break;
        }
        case 1:
        {
            RL1_SetHigh();RL0_SetLow();
            break;
        }
        case 2:
        {
            RL1_SetLow();RL0_SetHigh();
            break;
        }
        case 3:
        {
            RL1_SetLow();RL0_SetLow();
            break;
        }       
    }    
}
