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

#define MAC_SIZE 6
PN532_SPI pn532spi(SPI, 5);
NfcAdapter nfc = NfcAdapter(pn532spi);

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

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);

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
//void init_pwm();
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
	delay(2000);
}

void initDisplay()
{
	display.begin();
	display.fillScreen(BLACK);
	display.setCursor(0, 30);
    display.setTextColor(RED);
    display.setTextSize(2);
    display.print("Initializing");
    delay(333);
	display.print(".");
	delay(333);
	display.print(".");
	delay(333);
    display.print(".");
}

void nfcHandle() {

    Serial.begin(115200);

    int i = 3;

    while (1) {
    	// ReadTag
    	printf("\nScan a NFC tag\n");
    	char desString[50];

    	  if (nfc.tagPresent())
    	  {
    		  // Extract MAC Address to use
    	    NfcTag tag = nfc.read();
    	    tag.getTagType().toCharArray(desString, 50);
    	    puts(desString);
    	    puts("UID: ");
    	    tag.getUidString().toCharArray(desString, 50);
    	    puts(desString);

    	    if (tag.hasNdefMessage()) // every tag won't have a message
    	    {

    	      NdefMessage message = tag.getNdefMessage();
    	      printf("\nThis NFC Tag contains an NDEF Message with ");
    	      printf("%d", message.getRecordCount());
    	      printf(" NDEF Record");
    	      if (message.getRecordCount() != 1) {
    	        printf("s");
    	      }
    	      puts(".");

    	      // cycle through the records, printing some info from each
    	      int recordCount = message.getRecordCount();
    	      for (int i = 0; i < recordCount; i++)
    	      {
    	    	  printf("NDEF Record ");printf("%d\n", i+1);
    	        NdefRecord record = message.getRecord(i);
    	        // NdefRecord record = message[i]; // alternate syntax

    	        printf("  TNF: ");printf("%d\n", record.getTnf());  // Tnf = Type Name Field. T - text, 2 - Bluetooth
    	        printf("  Type: ");
    	        record.getType().toCharArray(desString, 50);
				puts(desString); // will be "" for TNF_EMPTY

    	        // The TNF and Type should be used to determine how your application processes the payload
    	        // There's no generic processing for the payload, it's returned as a byte[]
    	        int payloadLength = record.getPayloadLength();
    	        byte payload[payloadLength];
    	        record.getPayload(payload);

    	        // check for TNF: if TNF = 2 so save to mac address field
				if (record.getTnf() == 2) {
					for (int i = 0; i < MAC_SIZE; i++) {
						smartphoneMAC[i] = payload[payloadLength - i - 1];
						printf("%2X:", smartphoneMAC[i]);
					}
					printf("\n");
    	        }

    	        // Print the Hex and Printable Characters
    	        puts("  Payload (HEX): ");
    	        PrintHexChar(payload, payloadLength);

    	        // Force the data into a String (might work depending on the content)
    	        // Real code should use smarter processing
    	        String payloadAsString = "";
    	        for (int c = 0; c < payloadLength; c++) {
    	          payloadAsString += (char)payload[c];
    	        }
    	        puts("  Payload (as String): ");
    	        payloadAsString.toCharArray(desString, 50);
				puts(desString);
    	        // id is probably blank and will return ""
    	        String uid = record.getId();
    	        if (uid != "") {
    	        	uid.toCharArray(desString, 50);
    	          puts("  ID: ");puts(desString);
    	        }
    	      }
    	    }
//			  printf("For sending please keep the tag close");
//			  vTaskDelay(500);
//			  puts("...");
//
//			  if (nfc.tagPresent()) {
//
//				  uint64_t sendValue = 0;
//				  //char* numnum;
//				  //numnum = (char*)malloc(10*sizeof(char));
////				  strcpy(numnum, "Success!");
////				  test1.pData = numnum;
//				  sendValue += (test1.timePast << (8*(3))) | (test1.type << (8*2)) | (test1.data & (65535));
//				  printf ("0x%8X 0x%8X is sent!\n", (int)(sendValue >> 32), (int)sendValue);
//			  }
//    	    FLICK;
    	    positiveDisplayFeedback();
    	  }

    	  vTaskDelay(250);
    	  i--;

    	}
}

int app_main(void) {


	init_gpio();
	vTaskDelay(100 / portTICK_RATE_MS);
//	init_pwm();
	Serial.begin(115200);
	while(nfc.begin()) {
		printf("Connect NFC Module to continue!!!\n");
		vTaskDelay(300 / portTICK_RATE_MS);
	}
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
		gpio_set_level(GPIO_OUTPUT_RRGB1, NULL);
		gpio_set_level(GPIO_OUTPUT_GRGB1, NULL);
		gpio_set_level(GPIO_OUTPUT_BRGB1, NULL);
		nfcHandle();
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

//void init_pwm()
//{
//	/*
//	 * Prepare and set configuration of timers
//	 * that will be used by LED Controller
//	 */
//	ledc_timer.duty_resolution = LEDC_TIMER_13_BIT; // resolution of PWM duty
//	ledc_timer.freq_hz = 4000;                      // frequency of PWM signal
//	ledc_timer.speed_mode = LEDC_HS_MODE;           // timer mode
//	ledc_timer.timer_num = LEDC_HS_TIMER;            // timer index
//
//	// Set configuration of timer0 for high speed channels
//	ledc_timer_config(&ledc_timer);
//
//	ledc_channel[0].channel    = LEDC_HS_CH3_CHANNEL,
//	ledc_channel[0].duty       = LEDC_ZERO_DUTY,
//	ledc_channel[0].gpio_num   = LEDC_HS_CH3_BZZR,
//	ledc_channel[0].speed_mode = LEDC_HS_MODE,
//	ledc_channel[0].timer_sel  = LEDC_HS_TIMER;
//
//	ledc_channel_config(&ledc_channel[0]);
//
//	ledc_channel[0].channel    = LEDC_HS_CH0_CHANNEL,
//	ledc_channel[0].duty       = LEDC_INIT_DUTY,
//	ledc_channel[0].gpio_num   = LEDC_HS_CH0_RRGB2,
//	ledc_channel[0].speed_mode = LEDC_HS_MODE,
//	ledc_channel[0].timer_sel  = LEDC_HS_TIMER;
//
//	ledc_channel[1].channel    = LEDC_HS_CH1_CHANNEL;
//	ledc_channel[1].duty       = LEDC_INIT_DUTY;
//	ledc_channel[1].gpio_num   = LEDC_HS_CH1_GRGB2;
//	ledc_channel[1].speed_mode = LEDC_HS_MODE;
//	ledc_channel[1].timer_sel  = LEDC_HS_TIMER;
//
//	ledc_channel[2].channel	= LEDC_HS_CH2_CHANNEL;
//	ledc_channel[2].duty		= LEDC_INIT_DUTY;
//	ledc_channel[2].gpio_num	= LEDC_HS_CH2_BRGB2;
//	ledc_channel[2].speed_mode	= LEDC_HS_MODE;
//	ledc_channel[2].timer_sel	= LEDC_HS_TIMER;
//
//	// Set LED Controller with previously prepared configuration
//	for (int ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
//		ledc_channel_config(&ledc_channel[ch]);
//	}
////
////	// Initialize fade service.
//	ledc_fade_func_install(0);
//}
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
