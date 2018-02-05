# 101devBoard Test Repo

1. clone this repo to home directory of your local host machine. use: "git clone --recursive https://github.com/Avdalimov14/Testing.git"
2. run the following commands to handle environment variables:
	export GIT_PATH=$HOME/Testing
	export IDF_PATH=$GIT_PATH/esp-idf
	export PATH=$PATH:$DEV_PATH/xtensa-esp32-elf/bin
	export COMMAND=xtensa-esp32-elf-gcc
3. run the following commands to build and flash the test code:
	cd ~/Testing/TestApp
	make menuconfig   // it will open configuration menu, just exit it
	make all
	make flash
4. run the following command to open serial port: make monitor
5. the tests will begin with instructions on serial monitor.


Tests Insrtuction:
- The test includes NFC module connection, if it isn't connected correctly you will see it on serial monitor. 
- Please connect the Red Led to GPIO14(RGB1 red pin).
- Please connect the Green Led to GPIO12(RGB1 green pin).
- the system will continue to next test by pressing GPIO0.
- First test: Buzzer beeps twice.
- Second test: RGB1(binded to GPIO14 & GPIO12)  will light by the following order: red,geen,blue.all-together.
- Attention on second test: the Red and Green Leds binded to RGB1 red pin & green pin, be sure they lighted corresponded.
- Third test: RGB2 will fade in then fade out by the following order: red,geen,blue.all-together.
- Fourth test: NFC Module Test --> if you have any NFC tag try to scan it, it should beep after a scan.

In any case of errors and problems, contact me on email.

