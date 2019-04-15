#include <string.h>
//#include <iostream>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1331.h"
#include "SPI.h"
#include "PN532_SPI.h"
#include "PN532.h"
#include "NfcAdapter.h"
#include "Arduino.h"

extern "C" {
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_attr.h"
}

#define ESP32

#define MAC_SIZE 6
byte smartphoneMAC[MAC_SIZE];

// OLED Display
#define sclk	18
#define mosi	23
#define cs 		22
#define rst		32
#define dc		21

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

//Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);
extern "C" {
	int app_main(void);
// GPIO Defines

//#define GPIO_OUTPUT_BZZR		GPIO_NUM_13
#define GPIO_OUTPUT_RRGB1   	GPIO_NUM_25//14
#define GPIO_OUTPUT_GRGB1   	GPIO_NUM_26//12
#define GPIO_OUTPUT_BRGB1   	GPIO_NUM_27//27
#define GPIO_OUTPUT_PIN_SEL  (/*(1ULL<<GPIO_OUTPUT_BZZR) | */(1ULL<<GPIO_OUTPUT_RRGB1) | (1ULL<<GPIO_OUTPUT_GRGB1) | (1ULL<<GPIO_OUTPUT_BRGB1))
#define GPIO_INPUT_IO0     GPIO_NUM_0//0
//#define GPIO_INPUT_IO_1     35
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO0))// | (1ULL<<GPIO_INPUT_IO_1))
//#define ESP_INTR_FLAG_DEFAULT 0

// Declare of init functions
void init_gpio();
void initDisplay();
void flickLED();
#define FLICK flickLED()
void positiveDisplayFeedback();

// Declare general functions
void waitUntilPress(gpio_num_t gpioNum);
}


void positiveDisplayFeedback()
{
	display.fillScreen(BLACK);
	display.setCursor(0, 30);
	display.setTextColor(RED);
	display.setTextSize(2);
	display.println("  NFC");
	display.println(" SUCCESS");
	vTaskDelay(2000 / portTICK_RATE_MS);
}

void initDisplay()
{
	display.begin();
	display.printSpiStatus();

		ESP_LOGI("DEBUGGG", "why no print???");
	vTaskDelay(55 / portTICK_RATE_MS);
	display.fillScreen(BLACK);
	ESP_LOGI("DEBUGGG", "why no print???");
	display.printSpiStatus();

	ESP_LOGI("DEBUGGG", "why no print???");
	display.setCursor(0, 30);
    display.setTextColor(RED);
    display.setTextSize(2);
    display.print("Initializing");
    vTaskDelay(333 / portTICK_RATE_MS);
	display.print(".");
	vTaskDelay(333 / portTICK_RATE_MS);
	display.print(".");
	vTaskDelay(333 / portTICK_RATE_MS);
    display.print(".");
}



int app_main(void) {


	init_gpio();
	vTaskDelay(100 / portTICK_RATE_MS);
	Serial.begin(115200);
	initDisplay();

	printf("Welcome to 101DevBoard testing code.\n");
	while (1) {
		printf("Please press on GPIO 0 to light color\n");
#ifndef NOPRESS
		waitUntilPress(GPIO_INPUT_IO0);
#else
		vTaskDelay(1000 / portTICK_RATE_MS);
#endif
		// RGB IOs 14,12,27
		gpio_set_level(GPIO_OUTPUT_RRGB1, 1);
		vTaskDelay(250 / portTICK_RATE_MS);
		printf("Please press on GPIO 0 to o light another color\n");
#ifndef NOPRESS
		waitUntilPress(GPIO_INPUT_IO0);
#else
		vTaskDelay(750 / portTICK_RATE_MS);
#endif
		gpio_set_level(GPIO_OUTPUT_RRGB1, NULL);
		gpio_set_level(GPIO_OUTPUT_GRGB1, 1);
		vTaskDelay(250 / portTICK_RATE_MS);
		printf("Please press on GPIO 0 to o light another color\n");
#ifndef NOPRESS
		waitUntilPress(GPIO_INPUT_IO0);
#else
		vTaskDelay(750 / portTICK_RATE_MS);
#endif
		gpio_set_level(GPIO_OUTPUT_GRGB1, NULL);
		gpio_set_level(GPIO_OUTPUT_BRGB1, 1);
		vTaskDelay(250 / portTICK_RATE_MS);
		printf("Please press on GPIO 0 to o light white color\n");
#ifndef NOPRESS
		waitUntilPress(GPIO_INPUT_IO0);
#else
		vTaskDelay(750 / portTICK_RATE_MS);
#endif
		gpio_set_level(GPIO_OUTPUT_RRGB1, 1);
		gpio_set_level(GPIO_OUTPUT_GRGB1, 1);
		vTaskDelay(250 / portTICK_RATE_MS);
		// end of LEDs tests


		// NFC module tests
		printf("Please press on GPIO 0 to start NFC module test and turn off the LED\n");
#ifndef NOPRESS
		waitUntilPress(GPIO_INPUT_IO0);
#else
		vTaskDelay(750 / portTICK_RATE_MS);
#endif
		gpio_set_level(GPIO_OUTPUT_RRGB1, 1ULL);
		gpio_set_level(GPIO_OUTPUT_GRGB1, 1ULL);
		gpio_set_level(GPIO_OUTPUT_BRGB1, 1ULL);
		positiveDisplayFeedback();

		// end of NFC module tests

	}

		// need to xTaskCreate
	return 0;
	}

void waitUntilPress(gpio_num_t gpioNum)
{
	while (gpio_get_level(gpioNum))
		vTaskDelay(10);
}

void init_gpio()
{
	gpio_config_t io_conf;
	//disable interrupt
	io_conf.intr_type = GPIO_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	//disable pull-up mode
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	//configure GPIO with the given settings
	gpio_config(&io_conf);
	gpio_set_level(GPIO_OUTPUT_RRGB1, NULL);
	gpio_set_level(GPIO_OUTPUT_GRGB1, NULL);
	gpio_set_level(GPIO_OUTPUT_BRGB1, NULL);

	//bit mask of the pins
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	//io_conf.pull_up_en = 1;
	gpio_config(&io_conf);
}


void flickLED()
{
	gpio_set_level(GPIO_OUTPUT_RRGB1, 1);
	gpio_set_level(GPIO_OUTPUT_GRGB1, 1);
	gpio_set_level(GPIO_OUTPUT_BRGB1, 1);
	vTaskDelay(500 / portTICK_RATE_MS);
	gpio_set_level(GPIO_OUTPUT_RRGB1, NULL);
	gpio_set_level(GPIO_OUTPUT_GRGB1, NULL);
	gpio_set_level(GPIO_OUTPUT_BRGB1, NULL);
}
