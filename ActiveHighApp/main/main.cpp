#include <string.h>
//#include <iostream>
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

extern "C" {
	int app_main(void);

	typedef struct TestStrtuct {
		uint32_t timePast = 0;
		uint32_t type = 3;
		uint32_t data = 1;
	    int fieldCounter = 0;
	    //void* pData;
	} TestStrtuct;
//}

// PWM Defines

#define LEDC_HS_TIMER         	LEDC_TIMER_0
#define LEDC_HS_MODE          	LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_RRGB2       (33)
#define LEDC_HS_CH0_CHANNEL   	LEDC_CHANNEL_0
#define LEDC_HS_CH1_GRGB2       (26)
#define LEDC_HS_CH1_CHANNEL		LEDC_CHANNEL_1
#define LEDC_HS_CH2_BRGB2 		(32)
#define LEDC_HS_CH2_CHANNEL		LEDC_CHANNEL_2

#define LEDC_TEST_CH_NUM       (3)
#define LEDC_INIT_DUTY			(0)
#define LEDC_TEST_DUTY         (8191)
#define LEDC_TEST_FADE_TIME    (2000)

// PWM global variables
ledc_timer_config_t ledc_timer;
ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM];
char LedColor[3][6] = {"Red", "Green", "Blue"};

// GPIO Defines

#define GPIO_OUTPUT_BZZR		GPIO_NUM_13
#define GPIO_OUTPUT_RRGB1   	GPIO_NUM_14//14
#define GPIO_OUTPUT_GRGB1   	GPIO_NUM_12//12
#define GPIO_OUTPUT_BRGB1   	GPIO_NUM_27//27
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_RRGB1) | (1ULL<<GPIO_OUTPUT_GRGB1) | (1ULL<<GPIO_OUTPUT_BRGB1))
#define GPIO_INPUT_IO0     GPIO_NUM_0//0
//#define GPIO_INPUT_IO_1     35
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO0))// | (1ULL<<GPIO_INPUT_IO_1))
//#define ESP_INTR_FLAG_DEFAULT 0

// Declare of init functions
void init_gpio();
void init_pwm();
void beep();
#define BEEP beep()

// Declare general functions
void waitUntilPress(gpio_num_t gpioNum);
}

void nfcHandle() {

	TestStrtuct test1;

    Serial.begin(115200);

    while (1) {
    	// ReadTag
    	printf("\nScan a NFC tag\n");
    	char desString[50];

    	  if (nfc.tagPresent(1000))
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
			  printf("For sending please keep the tag close");
			  vTaskDelay(500);
			  puts("...");

			  if (nfc.tagPresent()) {

				  uint64_t sendValue = 0;
				  //char* numnum;
				  //numnum = (char*)malloc(10*sizeof(char));
//				  strcpy(numnum, "Success!");
//				  test1.pData = numnum;
				  sendValue += (test1.timePast << (8*(3))) | (test1.type << (8*2)) | (test1.data & (65535));
				  printf ("0x%8X 0x%8X is sent!\n", (int)(sendValue >> 32), (int)sendValue);
			  }
			  BEEP;

    	  }

    	  vTaskDelay(2000);

    	}
}

int app_main(void) {

	int itCnt = 0;

	init_gpio();

	Serial.begin(115200);
	while(nfc.begin()) {
		printf("Connect NFC Module to continue!!!\n");
		vTaskDelay(300 / portTICK_RATE_MS);
	}

	printf("Welcome to 101 testing active low code.\n");
	while (1) {
		printf("Please press on GPIO 0 to start test\n");
		waitUntilPress(GPIO_INPUT_IO0);
		// buzzer tests
//		BEEP;
//		BEEP;
		// end of buzzer tests
		// turn on 1 LED
		gpio_set_level(GPIO_OUTPUT_RRGB1, 1);
		printf("Please press on GPIO 0 to continue test\n");
		waitUntilPress(GPIO_INPUT_IO0);
		// turn on 2 LEDs
		gpio_set_level(GPIO_OUTPUT_GRGB1, 1);
		printf("Please press on GPIO 0 to continue test\n");
		waitUntilPress(GPIO_INPUT_IO0);
		// turn on 3 LEDs
		gpio_set_level(GPIO_OUTPUT_BRGB1, 1);
		printf("Please press on GPIO 0 to continue test\n");
		waitUntilPress(GPIO_INPUT_IO0);
		// turn off all LEDs
		gpio_set_level(GPIO_OUTPUT_RRGB1, NULL);
		gpio_set_level(GPIO_OUTPUT_GRGB1, NULL);
		gpio_set_level(GPIO_OUTPUT_BRGB1, NULL);
		printf("Please press on GPIO 0 to continue test\n");
		waitUntilPress(GPIO_INPUT_IO0);
		// PWM tests
		init_pwm();
		// Fade in then out 1 LED
		printf("%s fade up to duty = %d\n", LedColor[0], LEDC_TEST_DUTY);
			ledc_set_fade_with_time(ledc_channel[0].speed_mode,
					ledc_channel[0].channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
			ledc_fade_start(ledc_channel[0].speed_mode,
					ledc_channel[0].channel, LEDC_FADE_NO_WAIT);
		vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

		printf("%s fade up to duty = %d\n", LedColor[0], LEDC_INIT_DUTY);
		ledc_set_fade_with_time(ledc_channel[0].speed_mode,
				ledc_channel[0].channel, LEDC_INIT_DUTY, LEDC_TEST_FADE_TIME);
		ledc_fade_start(ledc_channel[0].speed_mode,
				ledc_channel[0].channel, LEDC_FADE_NO_WAIT);
		vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

		printf("Please press on GPIO 0 to continue test\n");
		waitUntilPress(GPIO_INPUT_IO0);

		// Fade in then out 2 LEDs
		printf("all colors fade up to duty = %d\n", LEDC_TEST_DUTY);
		for (int ch = 0; ch < 1; ch++) {
			ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
					ledc_channel[ch].channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
			ledc_fade_start(ledc_channel[ch].speed_mode,
					ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
		}
		vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

		printf("all colors fade up to duty = %d\n", LEDC_INIT_DUTY);
		for (int ch = 0; ch < 1; ch++) {
			ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
					ledc_channel[ch].channel, LEDC_INIT_DUTY, LEDC_TEST_FADE_TIME);
			ledc_fade_start(ledc_channel[ch].speed_mode,
					ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
		}
		vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

		printf("Please press on GPIO 0 to continue test\n");
		waitUntilPress(GPIO_INPUT_IO0);

		// Fade in then out 3 LEDs
		printf("all colors fade up to duty = %d\n", LEDC_TEST_DUTY);
		for (int ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
			ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
					ledc_channel[ch].channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
			ledc_fade_start(ledc_channel[ch].speed_mode,
					ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
		}
		vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

		printf("all colors fade up to duty = %d\n", LEDC_INIT_DUTY);
		for (int ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
			ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
					ledc_channel[ch].channel, LEDC_INIT_DUTY, LEDC_TEST_FADE_TIME);
			ledc_fade_start(ledc_channel[ch].speed_mode,
					ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
		}
		vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

		// end of PWM tests

		printf("Please press on GPIO 0 to continue test\n");
		waitUntilPress(GPIO_INPUT_IO0);

		// NFC module tests
		nfcHandle();
		// end of NFC module tests

		itCnt++;
	}

		// need to xTaskCreate
	return 0;
	}

void waitUntilPress(gpio_num_t gpioNum)
{
	while (gpio_get_level(gpioNum));
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
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
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
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	gpio_config(&io_conf);
}

void init_pwm()
{
	/*
	 * Prepare and set configuration of timers
	 * that will be used by LED Controller
	 */
	ledc_timer.duty_resolution = LEDC_TIMER_13_BIT; // resolution of PWM duty
	ledc_timer.freq_hz = 5000;                      // frequency of PWM signal
	ledc_timer.speed_mode = LEDC_HS_MODE;           // timer mode
	ledc_timer.timer_num = LEDC_HS_TIMER;            // timer index

	// Set configuration of timer0 for high speed channels
	ledc_timer_config(&ledc_timer);

	ledc_channel[0].channel    = LEDC_HS_CH0_CHANNEL,
	ledc_channel[0].duty       = LEDC_INIT_DUTY,
	ledc_channel[0].gpio_num   = GPIO_OUTPUT_RRGB1,
	ledc_channel[0].speed_mode = LEDC_HS_MODE,
	ledc_channel[0].timer_sel  = LEDC_HS_TIMER;

	ledc_channel[1].channel    = LEDC_HS_CH1_CHANNEL;
	ledc_channel[1].duty       = LEDC_INIT_DUTY;
	ledc_channel[1].gpio_num   = GPIO_OUTPUT_GRGB1;
	ledc_channel[1].speed_mode = LEDC_HS_MODE;
	ledc_channel[1].timer_sel  = LEDC_HS_TIMER;

	ledc_channel[2].channel	= LEDC_HS_CH2_CHANNEL;
	ledc_channel[2].duty		= LEDC_INIT_DUTY;
	ledc_channel[2].gpio_num	= GPIO_OUTPUT_BRGB1;
	ledc_channel[2].speed_mode	= LEDC_HS_MODE;
	ledc_channel[2].timer_sel	= LEDC_HS_TIMER;

	// Set LED Controller with previously prepared configuration
	for (int ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
		ledc_channel_config(&ledc_channel[ch]);
	}

	// Initialize fade service.
	ledc_fade_func_install(0);
}

void beep()
{
	gpio_set_level(GPIO_OUTPUT_BZZR, 1ULL);
	vTaskDelay(100 / portTICK_RATE_MS);
	gpio_set_level(GPIO_OUTPUT_BZZR, NULL);
	vTaskDelay(100 / portTICK_RATE_MS);
}
