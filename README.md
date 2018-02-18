# roomlight-6
Purpose: A program to allow for easy control of LED strips under the control of WS2812 chipset with an arduino

Description:
- GUI- creates an easy to use interface for communicating with an arduino uno using python (not complete)
- .INI- The background code used to command an arduino uno (functional for non abstracted use)
        is based on the Adafruit Neopixel Library "strandtest.ini"

Installation:
1. Install dependencies
 - Download and install python from https://www.python.org/downloads/
 - Download and install arduino software from https://www.arduino.cc/en/Main/Software
 - Install python libraries:
  - (pyaudio) apt-get install python-pyaudio or python -m pip install pyaudio
  - (tkinter) apt-get install python-tk
  - (pyowm) apt-get install python-pyowm or python -m pip install pyowm
  - (numpy) apt-get install python-numpy or python -m pip install numpy
  - (pyserial) apt-get install python-pyserial or python -m pip install pyserial
2. Flash arduino:
 - Open arducode.ino in the arduino software
 - click the upload button

Far Goals:
 - enable on the fly settings changes, add strips through GUI, change number of lights possibly either through settings files, or rebasing entire thing with firmata and node.js
 - add graphical bar for displaying audio levels, Graphical equalizer
 - optimize certain functions on arduino, rainbow functions currently struggle

