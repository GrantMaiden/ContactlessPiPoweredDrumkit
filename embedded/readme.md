# Directory for Embedded development  
  
!!!IMPORTANT! PLEASE READ ENTIRETY OF THIS README BEFORE PROCEEDING WITH RUNNING PROJECT!!!  
  
## Important Documents  
  
For Cmake build instructions, refer to [cmake_build_instructions.txt]("./embedded/cmake_build_instructions.txt")  
For Unit Test Parameters, refer to [unit_tests.txt]("./embedded/unit_tests.txt")  
Before proceeding, make sure to complete build setup, otherwise compilation will not be successful.  
  
### Running Instructions:  
   After building, preceed all program calls with _sudo_, as pigpio lib requires sudo access.  
  
## Project Setup  
Recommended Operating System for compilation: [Bookworm]("https://downloads.raspberrypi.com/raspios_full_armhf/images/raspios_full_armhf-2023-12-06/2023-12-05-raspios-bookworm-armhf-full.img.xz")  
  
### Project is managed using codeblocks.  
To install codeblocks, run the following from console:  
```
sudo apt-get install codeblocks  
```  
### Enable I2C/SPI communication:  
Pi Grenade Bomb -> Preferences -> Raspberry Pi Configuration -> Interfaces -> I2C (Toggle)  
Pi Grenade Bomb -> Preferences -> Raspberry Pi Configuration -> Interfaces -> SPI (Toggle)  
  
### Edit Config.txt:  
```
sudo nano /boot/firmware/config.txt
``` 
Add Lines:  
```
core_freq=250  
core_freq_min=250  
dtparam=i2c_baudrate=400000  
```  
### Setup Audio by changing user default audio device:  
```
   $sudo nano /usr/share/alsa/alsa.conf  
   defaults.pcm.card 1  
   defaults.ctl.card 1  
```  
### [pigpio download]("https://abyz.me.uk/rpi/pigpio/download.html")  
  
### Setup Codeblocks:  
Settings-> Compiler -> Linker Settings -> Other linker options -> add to field: -lpigpio -lrt -lpthread -lm -ldl -latomic  
  
### Add Tool Codeblocks:  
Tools -> Configure Tools -> Add -> Name field: Run as Sudo -> Executable Field: sudo $(TARGET_OUTPUT_FILE)   
**NOTE: MUST BUILD THEN TO RUN SELECT THIS TOOL FROM TOOL MENU**  
  
## Resources and Tools
  
Self Documenting Code Format: [DoxyGen]("https://www.doxygen.nl/manual/docblocks.html")    
  
[Kernel Documentation]("https://www.kernel.org/doc/Documentation/")  
  
Sys Libraries location:  
```
/usr/include/arm-linux-gnueabihf/sys
```
To close lingering pigpio processes:  
```
   $cat /var/run/pigpio.pid  
   $NumberReturned   
   $sudo kill -9 NumberReturned
```
