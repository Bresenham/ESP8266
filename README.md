# ESP8266
Figured out how to get SDK v3.1 working!

## Resources I used:

Install Espressif Toolchain: https://cutmywire.wordpress.com/2017/05/08/esp8266-windows-10-sdk-installation/comment-page-1/#comment-943

Overcome hurdles of Toolchain Installation: https://github.com/esp8266/esp8266-wiki/issues/74

Makefile source: https://github.com/esp8266/source-code-examples/blob/master/blinky/Makefile (adapted this one to SDK v3.0)

Latest esp_init_data_default.bin file: https://github.com/espressif/ESP8266_NONOS_SDK/tree/master/bin

Simple example from Espressif on how to write Code for SDK v3.0: https://github.com/espressif/ESP8266_NONOS_SDK/tree/master/examples/simple_pair

Fix partitioning error on startup for D1 mini (at least for me): https://blog.csdn.net/yannanxiu/article/details/83550183

How COM-Ports from Windows are mapped to linux subsystem: https://blogs.msdn.microsoft.com/wsl/2017/04/14/serial-support-on-the-windows-subsystem-for-linux/

How to fix permission error on dev/ttyS: https://github.com/Microsoft/WSL/issues/617

## General projectstructure to make things work with the Makefiles from espressif:

Projects<br/>
--bin\ (from [here](https://github.com/espressif/ESP8266_NONOS_SDK/tree/master/bin))<br/>
--ProjectDirectory<br/>
&nbsp;&nbsp;&nbsp;--user (contains your source-code)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--Makefile (from [here](https://github.com/espressif/ESP8266_NONOS_SDK/blob/master/examples/simple_pair/user/Makefile))<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--user_main.c<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--user_config.h<br/>
&nbsp;&nbsp;&nbsp;--Makefile (from [here](https://github.com/espressif/ESP8266_NONOS_SDK/blob/master/examples/simple_pair/Makefile))<br/>
--ld (exact same folder and content as [here](https://github.com/espressif/ESP8266_NONOS_SDK/tree/master/ld))<br/>
--lib (exact same folder and content as [here](https://github.com/espressif/ESP8266_NONOS_SDK/tree/master/lib))<br/>
--tools (exact same folder and content as [here](https://github.com/espressif/ESP8266_NONOS_SDK/tree/master/tools))<br/>
--gen_misc.sh (if you want)<br/>
--Makefile (from [here](https://github.com/espressif/ESP8266_NONOS_SDK/blob/master/Makefile))<br/>

You either call ./gen_misc.sh or Makefile from Projects-Folder.

I compile my project like this:<br/>

`$ make COMPILE=gcc BOOT=2 APP=0 SPI_SPEED=40 SPI_MODE=DIO SPI_SIZE_MAP=4`

If, for whatever reason, the compiler doesn't find the include path to the SDK where all the header files are, do this:<br/>
* Add the path to your include directory (e.g. `/opt/esp/sdk/include`) to the INCLUDE-statement at the very bottom of the outer-most Makefile.

You should also make sure that the path to the compiler is in your $PATH-Variable, in my case for example:

`$ PATH=$PATH:/opt/esp/crosstool-NG/builds/xtensa-lx106-elf/bin`

While you're at it, you can also add the SDK folder:

`$ PATH=$PATH:/opt/esp/sdk/include`

Unfortunately, the linux subsystem doesn't remember changes made to the $PATH-Variable. So after every reboot I have to type in again the two commands mentioned above.

I also added two more commands in the upper-most Makefile (which is not part of this repository) to make flashing and erasing easier:<br/>

`erase:`<br/>
&nbsp;&nbsp;&nbsp;`esptool.py --port $(PORT) erase_flash`
<br/>
`flash:`<br/>
&nbsp;&nbsp;&nbsp;`esptool.py -b 921600 --port $(PORT) write_flash 0x0 bin/eagle.flash.bin 0x10000 bin/eagle.irom0text.bin 0x3FC000 bin/esp_init_data_default_v08.bin`

Flashing the ESP8266 now works like this `make flash PORT=/dev/ttyS5`

If somehow esptool doesn't work or throws weird errors (like `rom doesn't support changing baud rate. keeping initial baud rate 115200`), remove the package from your linux distribution and instead clone the latest version from [github](https://github.com/espressif/esptool). As the last step, add the directory to your $PATH-Variable: `PATH=$PATH:/opt/esp/esptool/`.

The address `0x3FC000` is for the board I use at the moment (D1 mini), it may not work with yours. The `esp_init_data_default_v08.bin` is from [here](https://github.com/espressif/ESP8266_NONOS_SDK/tree/master/bin).

Check out the [Getting Started Guide](https://www.espressif.com/sites/default/files/documentation/2a-esp8266-sdk_getting_started_guide_en.pdf) on page 15 to get more information about the specific address for the `esp_init_data_default.bin` file.

## Documentation
BMP280 Documentation: https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf <br/>
ESP8266 reads ID from BMP280 via Soft-I2C (analyzed with Saleae Logic Analyzer):
![alt text](https://github.com/Bresenham/ESP8266/blob/master/resources/ESP8266_Read_BMP280_ID_via_I2C.png)

Library for SSD1306 OLED Display communication: https://www.mikrocontroller.net/topic/415651 <br/>
Basic Wifi Structure (`Sample Codes` -> `ESP8266 as TCP Client`): https://www.espressif.com/en/products/hardware/esp8266ex/resources <br/>

First breadboard construction: ![alt text](https://github.com/Bresenham/ESP8266/blob/master/resources/Breadboard.jpg)
