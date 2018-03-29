/* LEDC (LED Controller) fade example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

#define GPIO_INPUT_PIN_SEL 1ULL << GPIO_NUM_4
#define GPIO_OUTPUT_RRGB1   	GPIO_NUM_14//14
#define GPIO_OUTPUT_GRGB1   	GPIO_NUM_12//12
#define GPIO_OUTPUT_BRGB1   	GPIO_NUM_27//27
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_RRGB1) | (1ULL<<GPIO_OUTPUT_GRGB1) | (1ULL<<GPIO_OUTPUT_BRGB1))
/*
 * About this example
 *
 * 1. Start with initializing LEDC module:
 *    a. Set the timer of LEDC first, this determines the frequency
 *       and resolution of PWM.
 *    b. Then set the LEDC channel you want to use,
 *       and bind with one of the timers.
 *
 * 2. You need first to install a default fade function,
 *    then you can use fade APIs.
 *
 * 3. You can also set a target duty directly without fading.
 *
 * 4. This example uses GPIO18/19/4/5 as LEDC output,
 *    and it will change the duty repeatedly.
 *
 * 5. GPIO18/19 are from high speed channel group.
 *    GPIO4/5 are from low speed channel group.
 *
 */
#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO       (13)
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
/*
#define LEDC_HS_CH1_GPIO       (14)
#define LEDC_HS_CH1_CHANNEL    LEDC_CHANNEL_1

#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_LS_CH2_GPIO       (4)
#define LEDC_LS_CH2_CHANNEL    LEDC_CHANNEL_2
#define LEDC_LS_CH3_GPIO       (5)
#define LEDC_LS_CH3_CHANNEL    LEDC_CHANNEL_3
*/
#define LEDC_TEST_CH_NUM       (1)
#define LEDC_TEST_FULL_DUTY    (8191)
#define LEDC_TEST_FADE_TIME    (2000)

void waitUntilPress(gpio_num_t gpioNum)
{
	while (gpio_get_level(gpioNum));
}

void app_main()
{
    int ch;
    int state = 0;

    gpio_config_t io_conf;
    //bit mask of the pins
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	gpio_config(&io_conf);

	//bit mask of the pins
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//set as input mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//enable pull-up mode
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	gpio_config(&io_conf);
	gpio_set_level(GPIO_OUTPUT_RRGB1, 1ULL); // LED off
	gpio_set_level(GPIO_OUTPUT_GRGB1, 1ULL);
	gpio_set_level(GPIO_OUTPUT_BRGB1, 1ULL);
    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,           // timer mode
        .timer_num = LEDC_HS_TIMER            // timer index
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);
/*
    // Prepare and set configuration of timer1 for low speed channels
    ledc_timer.speed_mode = LEDC_LS_MODE;
    ledc_timer.timer_num = LEDC_LS_TIMER;
    ledc_timer_config(&ledc_timer);
*/
    /*
     * Prepare individual configuration
     * for each channel of LED Controller
     * by selecting:
     * - controller's channel number
     * - output duty cycle, set initially to 0
     * - GPIO number where LED is connected to
     * - speed mode, either high or low
     * - timer servicing selected channel
     *   Note: if different channels use one timer,
     *         then frequency and bit_num of these channels
     *         will be the same
     */
    ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM] = {
        {
            .channel    = LEDC_HS_CH0_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_HS_CH0_GPIO,
            .speed_mode = LEDC_HS_MODE,
            .timer_sel  = LEDC_HS_TIMER
        }/*,
        {
            .channel    = LEDC_HS_CH1_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_HS_CH1_GPIO,
            .speed_mode = LEDC_HS_MODE,
            .timer_sel  = LEDC_HS_TIMER
        },
        {
            .channel    = LEDC_LS_CH2_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH2_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .timer_sel  = LEDC_LS_TIMER
        },
        {
            .channel    = LEDC_LS_CH3_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH3_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .timer_sel  = LEDC_LS_TIMER
        },*/
    };

    // Set LED Controller with previously prepared configuration
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }

    // Initialize fade service.
    //ledc_fade_func_install(0);

    while (1) {
    	printf("state = %d, freq = %d\n", state, ledc_timer.freq_hz);
    	vTaskDelay(1000 / portTICK_PERIOD_MS);

    	if (gpio_get_level(GPIO_NUM_4) == 0) {


    		switch(ledc_timer.freq_hz)
    		{
    			case 1000:
    				ledc_timer.freq_hz = 2000;
    				break;
    			case 2000:
    				ledc_timer.freq_hz = 3000;
    				break;
    			case 3000:
    				ledc_timer.freq_hz = 4000;
    				break;
    			case 4000:
    				ledc_timer.freq_hz = 5000;
    				break;
    			case 5000:
    				ledc_timer.freq_hz = 1000;
    				break;
    			default:
    				break;
    		}
    		ledc_timer_config(&ledc_timer);
    		gpio_set_level(GPIO_NUM_14, 1); // Led Off
    	}

    	if (gpio_get_level(GPIO_NUM_0) == 0) {
    		state++;
    		if (state == 8) {
    			state = 0;
    			gpio_set_level(GPIO_NUM_14, NULL); // Led Off
    		}

			switch (state)
			{
				case 0:
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, 0);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					break;
				case 1:
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, LEDC_TEST_FULL_DUTY / 4);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					vTaskDelay(2000 / portTICK_PERIOD_MS);
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, 0);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					break;
				case 2:
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, LEDC_TEST_FULL_DUTY / 2);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					vTaskDelay(2000 / portTICK_PERIOD_MS);
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, 0);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					break;
				case 3:
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, LEDC_TEST_FULL_DUTY / 10);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					break;
				case 4:
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, LEDC_TEST_FULL_DUTY / 5);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					break;
				case 5:
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, (3 * LEDC_TEST_FULL_DUTY) / 10);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					break;
				case 6:
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, (2 * LEDC_TEST_FULL_DUTY) / 5);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					break;
				case 7:
					ledc_set_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel, (LEDC_TEST_FULL_DUTY) / 2);
					ledc_update_duty(ledc_channel[0].speed_mode, ledc_channel[0].channel);
					break;
				default:
					break;
			}
    	}
//        printf("1. LEDC fade up to duty = %d\n", LEDC_TEST_DUTY);
//        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
//            ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
//                    ledc_channel[ch].channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
//            ledc_fade_start(ledc_channel[ch].speed_mode,
//                    ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
//        }
//        vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);
//
//        printf("2. LEDC fade down to duty = 0\n");
//        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
//            ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
//                    ledc_channel[ch].channel, 0, LEDC_TEST_FADE_TIME);
//            ledc_fade_start(ledc_channel[ch].speed_mode,
//                    ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
//        }
//        vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);
//
//        printf("3. LEDC set duty = %d without fade\n", LEDC_TEST_DUTY);
//        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
//            ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_TEST_DUTY);
//            ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
//        }
//        vTaskDelay(1000 / portTICK_PERIOD_MS);
//
//        printf("4. LEDC set duty = 0 without fade\n");
//        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
//            ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0);
//            ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
//        }

    }
}
