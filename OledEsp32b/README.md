# 101devBoard Test Repo

# Install Prerequisites
- <b>use 64-bit Ubunto Linux OS</b> (if you prefer using 32-bit then replace xtensa folder with this one https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-80-g6c4433a-5.2.0.tar.gz
- run the following command:<br />
	sudo apt-get install git wget make libncurses-dev flex bison gperf python python-serial

# Build and Flash Instructions
1. clone this repo to home directory of your local host machine. use:<br /> git clone --recursive https://github.com/Avdalimov14/Testing.git
2. run the following commands to handle environment variables:<br />
	export GIT_PATH=$HOME/Testing<br />
	export IDF_PATH=$GIT_PATH/esp-idf<br />
	export PATH=$PATH:$GIT_PATH/xtensa-esp32-elf/bin<br />
	export COMMAND=xtensa-esp32-elf-gcc
3. nevigate to test project folder and build it:<br />
	cd ~/Testing/PcbVer3TestApp<br />
        make menuconfig   // it will open configuration menu, just exit it<br />
        make all
4. connect esp32 to PC and get into Download Mode:<br />
	Keep GPIO0 pressed while reseting the esp32
5. run the following commands to flash the test code:<br />
	make flash
5. reset the esp32 normally
6. run the following command to open serial port: make monitor (If you connected via UART be sure you connect the Uart-GND to the board, otherwise you won't see nothing, may be jibrish)
7. the tests will begin with instructions on serial monitor.


# Tests Insrtuction:
- The test includes NFC module connection, if it isn't connected correctly they program won't continue, you will see it on serial monitor. 
- the system will continue to next test by pressing GPIO0.
- First test (3 Clicks): RGB LED will light different single color for each press on GPIO0.
- Second test:  RGB LED will light white color.
- Third test: NFC Module Test --> if you have any NFC tag try to scan it, it should flick led's light after a scan.

In any case of errors and problems, contact me on email.

