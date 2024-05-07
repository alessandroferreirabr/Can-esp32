/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "hal/twai_types.h"
#include "driver/twai.h"

#define pgnteste 0x123
void Can_Task (void *params);
void Led_Task (void *params);

twai_message_t message;
twai_handle_t canhandle;
void app_main(void)
{
 
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
       if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        printf("Driver installed\n");
    } else {
        printf("Failed to install driver\n");
        return;
    }
        if (twai_start() == ESP_OK) {
        printf("Driver started\n");
    } else {
        printf("Failed to start driver\n");
        return;
    }
       gpio_set_direction(2, GPIO_MODE_OUTPUT);

   xTaskCreate(&Can_Task,"Can_Task",2048,"Task1",0,NULL);
   xTaskCreate(&Led_Task,"Led_Task",1024,"Task2",2,NULL);
}

void Led_Task (void *params)
{
    while ((1))
    {
        gpio_set_level(GPIO_NUM_2,1);
        vTaskDelay(100/portTICK_PERIOD_MS);
        gpio_set_level(GPIO_NUM_2,0);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
    
}

void Can_Task (void *params)
{
            message.identifier = pgnteste;
            message.extd = 0;
            message.rtr = 0;
            message.ss = 1;
            message.data_length_code = 8;
            message.dlc_non_comp = 0;
            message.data[0] = 1;
            message.data[1] = 2;
            message.data[2] = 3;
            message.data[3] = 4;
            message.data[4] = 5;
            message.data[5] = 6;
            message.data[6] = 7;
            message.data[7] = 8;
            
   
    while ((1))
    {
           
        if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
            printf("Message queued for transmission\n");
        } else {
            printf("Failed to queue message for transmission\n");
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
    
}