# GPInstruments 720NV Autosampler Debug Controller
![screenshot.png](screenshot.png)

This application is used to help speed up the calibration of Model 720NV Autosamplers. It allows to connect over RS-232 and control movement of the needle carriage, poll and set variables in memory, and monitor status of the machine. It supports sending arbitrary commands over serial as well but most commonly used functions are supported. Spend less time looking through the manual to find the one command you need and more time doing other things. Written in C++11 with Qt and [@Jorgen-VikingGod's Qt Frameless Window DarkStyle](https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle) and available for Windows in 32 and 64bit flavors. 

### [Download from the release page.](https://github.com/amoose136/GPIController/releases)

## Usage Notes

Hover over the relevant button and wait a second for a tooltip to pop up explaining the function. In the move section only one command can be sent at a time and nothing happens if all values are zero. Individual axis homing is currently disabled as the implementation on the machine side is not done yet. Currently temperature reporting does not autoupdate and only reports temp to the nearest whole degree C.