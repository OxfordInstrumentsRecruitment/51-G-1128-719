# 51-G-1128-719
STM32F103 breakout board

This breakout board for the STM32F103 processor provides the following features:
* Compatible with the STM32duino framework
* A USB bootloader, allowing code to be loaded from the Arduino IDE
* A position for a 240x240 RGB screen based on the ST7789 controller
* An SD card reader
* Battery or USB operation
* A three button user interface
* An RGB LED with PWM brightness control
* SPI, UART, I2C, SWD and GPIO external connectivity

# Assembly Instructions

If you were given a blank PCB business card by someone from OI at an event and wish to populate and use it, you will need to follow the instructions in "PCB Business Card Assembly Instructions.PDF", which is in the Documentation folder. If your PCB business card was given to you with components already in place, you can skip this step, and skip programming the USB bootloader.

The assembly instructions document contains a list of the tools and consumables you will need to build this board. For a shopping list of components, check out the file "51-G-1128-719_BOM.xlsx" in the Documentation folder.

The BOM also links each component value to the corresponding reference designator on the PCB.

# Programming The USB Bootloader

Once you have assembled your PCB business card, you must program the USB bootloader into the STM32F103 in order to use the Arduino IDE to program the device.

This is done using a USB to UART adapter such as the FTDI TTL-232R-3V3 or equivalent. Follow these steps in order to program the bootloader:

* There is a solderable link near the SD card socket labelled "BOOT 0" - apply a bead of solder bridging the center to the side marked "1"
* Solder the RX, TX and GND signals from the USB to UART adapter to PA9, PA10 and GND on the PCB business card. PA9 and PA10 can be accessed via the GPIO pins on the right hand side of the PCB. When using an FTDI USB to UART cable, the orange wire goes to PA10 and the yellow wire goes to PA9.
* The exact programming procedure is slightly different on Windows, Linux and Mac. A Windows utility is provided to program the PCB in the Firmware\Bootloader\stm32tools\2019.2.28\win\ folder. The utility is called stm32flash.exe and runs stand alone with no installation required. The bootloader binary is "generic_boot20_pb9.bin" and can be found under the folder Firmware\Bootloader\. To use the provided utility on widnows:
  * Open a CMD window and navigate to Firmware\Bootloader\
  * Connect the USB to UART adapter and apply power to the PCB business card.
  * Determine the COM port number of the USB to UART adapter (this can be done from the Arduino IDE, or by opening device manager)
  * Then run the command (replacing the 3 in "COM3" with the COM port number on your system):
  `stm32tools\2019.2.28\win\stm32flash.exe -w generic_boot20_pb9.bin -v -g 0x0 COM3`
* If you are running Linux or Mac, follow the instructions on [this page of the STM32duino wiki](http://wiki.stm32duino.com/index.php?title=Burning_the_bootloader)
* Once the bootloader is programmed, remove power from the PCB business card and solder the "BOOT 0" link into the "0" position

# Uploading Your First Sketch

The PCB business card USB bootloader uses the standard windows DFU (Device Firmware Upload) driver, however the USB VID and PID are not known to Windows and therefore Windows will say "USB device not recognised" when you connect it. This problem can be solved by running the script "install_drivers.bat" in the folder: Firmware\drivers\win\

Once this is done, Windows should enumerate the PCB business card as a DFU capable device. This step is not required on Mac or Linux as both operating systems recognize the device as DFU capable and do not care about the VID and PID.

To program the example sketch onto the PCB card, follow these steps:

* Install the Arduino IDE
* Follow the instructions on [this page](http://wiki.stm32duino.com/index.php?title=Installation) to install the STM32duino extensions for the Arduino IDE
* copy the folders from Firmware\Libs\ into your Arduino Libraries folder.
* start the Arduino IDE (note that the IDE must not be open when you copy the library folders)
* open Firmware\Test\graphicstest\graphicstest.ino
* Select the correct COM port from the tools menu in the Arduino IDE
* Press the upload button

This sketch will display some test graphics. If you have an SD card, copy the contents of Firmware\Test\SD_Graphics\ onto it and put the SD card in the PCD business card. The card will now display some images from the card. The name on the name tag display comes from the file name.txt on the SD card. The logo on the same page comes from logo.bmp. The name must not be more than 13 characters. The job title must not be more than 20 characters. the logo must be 240 pixels wide and in uncompressed 24-bit bitmap format.

Up to 255 images can be added to the SD card with number names from 1.bmp to 255.bmp. Each will be displayed for 2 seconds.
