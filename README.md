# ESP32-Oscilloscope-Arduino
A basic ESP32 dual-core based oscilloscope coded in Arduino IDE

## Note

This code was created under a tight deadline and requires significant improvement.  
The user setup file for the TFT library is not provided yet.

## Installation

The folder 'oscilloscope' is the Arduino project.  
Install the [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI "GitHub page") library in the Arduino IDE,
it should be available in the IDE's library manager.  
Make sure to configure the user setup file of the TFT_eSPI, the LCD used is the ST7796.  

For the pins, I have used the VSPI for touch and LCD, and:  
LCD:
- DC - G0
- RESET - EN
- CS - G5  

Touch:
- CS - G4

For the probe conection to the ADC inputs I did the following circuit:  
![Probe conection and scale](https://github.com/AdrianoPoier/ESP32-Oscilloscope-Arduino/blob/main/demo%20images/probe%20circuit.png)  
Where the probes positive connect to the respective CH labeled and their grounds connects all into the
probe's grounds line. The GPIO labels indicate to connect in the respective ADC pins of the ESP32.

## TODO

- Setup the hardware to make ADC readings through DMA
- Make the sampling rate configurable on the GUI
- Convert readings to voltage scale and change the GUI menus accordingly
- Add voltage calibration option on the GUI
- Improve GUI
- Improve graph rendering by drawing a column in a buffer and sending through DMA, with double buffering
