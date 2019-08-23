#include "mcc_generated_files/mcc.h"
#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include"define.h"
#include"led.h"
#include"relay.h"
/*
                         Main application
 */
void DATA_PROCESS(char x);
void check_message(uint8_t x);
bool  check_data(uint8_t x);
void update_state(void);
void myButtonPressedCallback(enum mtouch_button_names button);
void myButtonReleasedCallback(enum mtouch_button_names button);
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    MTOUCH_Button_SetPressedCallback(myButtonPressedCallback);
    MTOUCH_Button_SetNotPressedCallback(myButtonReleasedCallback);    
    LED_EN_SetLow();
    config_status = false;  
    last_touch_status.full_status = 0x00;
    LED_PROCESS(last_touch_status.full_status);
    RELAY_PROCESS(last_touch_status.full_status);     
    timeout_count = 0;
    while (1)
    {
        if(update_state_flag == true)
        {
            update_state_flag = false;
            update_state();           
        }
        if(EUSART_is_rx_ready())
        {
            rxData = EUSART_Read();
            if(check_data(rxData))
            {
                if(check_request == false)// khong co gi de check
                {
                    prcData = rxData;
                    check_message(rxData);                  
                }
                else// da gui lenh check, xac nhan
                {
                    if(config_status == true)
                    {
                        if(rxData == 'T')
                        {
                            LED_PROCESS(7);
                            __delay_ms(500);
                            __delay_ms(500);
                            __delay_ms(500);
                            __delay_ms(500);
                            __delay_ms(500);
                            __delay_ms(500);
                            config_status = false;
                            check_request = false;
                            timeout_count = 0;
                            LED_PROCESS(last_touch_status.full_status);
                        }
                        if(rxData == 'F')
                        {           
                            LED_PROCESS(0);
                            __delay_ms(500);
                            __delay_ms(500);
                            __delay_ms(500);
                            __delay_ms(500);
                            __delay_ms(500);
                            __delay_ms(500);
                            config_status = false;
                            check_request = false;
                            timeout_count = 0;
                            LED_PROCESS(last_touch_status.full_status);
                        }                        
                    }
                    else
                    {
                        if(rxData == 'T')
                        {
                            DATA_PROCESS(prcData);
                        }
                        if(rxData == 'F')
                        {                        
                            check_request = false;// xoa request
                        }
                    }
                }
            }
        }
        if(MTOUCH_Service_Mainloop()) // 1 tick = 0.5
        {
            if(config_status == false)
            {
                if(check_request == true)
                {
                    timeout_count++;
                    if(timeout_count == 30) // 10 ms
                    {
                        timeout_count = 0;
                        check_request = false;
                    }
                }
                if(MTOUCH_Button_isPressed(0) == 1 && MTOUCH_Button_isPressed(1) == 1)
                {                    
                    count_conf++;
                    if(count_conf == 1800)
                    {         
                        count_conf = 0;
                        printf("PICSMART");
                        __delay_ms(500);
                        __delay_ms(500);
                        __delay_ms(500);
                        __delay_ms(500);
                        __delay_ms(500);
                        __delay_ms(500);
                        __delay_ms(500);
                        __delay_ms(500);
                        printf("RUN\r\n");
                        config_status = true;
                        check_request = true;
                        timeout_count = 0;
                        LED_PROCESS(0);
                        blink = false;
                    }
                }
                else
                {
                    count_conf = 0;
                }
            }
            else // dang smart configure
            {
                if(check_request == true)
                {
                    timeout_count++;
                    if(timeout_count >= 30000)
                    {
                        
                        LED_PROCESS(0);
                        //timeout_count++;
                        if(timeout_count >= 32000)
                        {
                            timeout_count = 0;
                            LED_PROCESS(last_touch_status.full_status);
                            check_request = false;
                            config_status = false;                          
                        }                        
                    }
                    else
                    {
                        blink_count++;
                        if(blink == false)
                        {
                            LED_PROCESS(0);
                            if(blink_count == 200)
                            {
                                blink_count = 0;
                                blink = true;
                            }                                      
                        }
                        else
                        {
                            LED_PROCESS(3);
                            if(blink_count == 200)
                            {
                                blink_count = 0;
                                blink = false;
                            } 
                        }  
                    }
                }                                    
            }            
        }
    }
}
/**
 End of File
*/
void myButtonPressedCallback(enum mtouch_button_names button)
{
    count_conf = 0;
    if(config_status == false)
    {
        if(button == 0) last_touch_status.full_status ^= 0b00000001;
        if(button == 1) last_touch_status.full_status ^= 0b00000010;
        LED_PROCESS(last_touch_status.full_status);
        RELAY_PROCESS(last_touch_status.full_status);
    }
}
void myButtonReleasedCallback(enum mtouch_button_names button)
{
    count_conf = 0;
    if(config_status == false)
    {
        
        if(MTOUCH_Button_isPressed(0)==0&&MTOUCH_Button_isPressed(1)==0)
        {           
            update_state_flag = true;
        }
    }
}
void update_state(void)
{
    switch(last_touch_status.full_status)
    {
        case 0: printf("PIC0"); break;
        case 1: printf("PIC1"); break;
        case 2: printf("PIC2"); break;
        case 3: printf("PIC3"); break;
    }
}
void DATA_PROCESS(char x)
{
    timeout_count = 0;
    switch(x)
    {
        case '0':
        {
            LED_PROCESS(0);            
            RELAY_PROCESS(0);           
            check_request = false;
            last_touch_status.full_status = 0;
            break;
        }
        case '1':
        {
            LED_PROCESS(1);
            RELAY_PROCESS(1);
            check_request = false;
            last_touch_status.full_status = 1;
            break;
        }
        case '2':
        {
            LED_PROCESS(2);
            RELAY_PROCESS(2);
            check_request = false;
            last_touch_status.full_status = 2;
            break;
        }
        case '3':
        {
            LED_PROCESS(3);
            RELAY_PROCESS(3);
            check_request = false;
            last_touch_status.full_status = 3;
            break;
        }                       
        case 'U':
        {
            check_request = false;
            update_state_flag = true;
            break;
        }
        default:
        {
            check_request = false;
            break;
        }
    }
}
void check_message(uint8_t x)
{
    switch (x)
    {
        case 'T': 
        {
            check_request = false;
            break;
        }
        case 'F': 
        {
            check_request = false;
            break;
        }
        default:
        {
            printf("%c",x);            
            check_request = true;
            break;
        }
    }
}
bool  check_data(uint8_t x)
{
    switch(x)
    {
        case '0': return true;
        case '1': return true;
        case '2': return true;
        case '3': return true;
        case 'T': return true;
        case 'F': return true;
        case 'U': return true;        
        default : return false;
    }
}