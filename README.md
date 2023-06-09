# Senior Design 
## Team AquaCool 

### Background
The files located within this respository are the two code files that pertain to the project created by team AquaCool. One file is the main program that runs when the system is on and the other is the program that was used to test the performace of the system. The files will be described in slightly more detail in the bottom sections of this document. 

### Testing File
The testing file use two DS18B20 sensors to measure the temperature of the system. The readings are output through Serial and then put into a CSV using Putty. 

### Running Program
This program is the program that is constantly running that conrols the device. The program automatically cycles through a few different states such  as reading the temperature, outputting to the display, and turning the system on/off. While that is a more simpified approach, all the code within the file has been titled with states and a small explanation for each state. Three buttons control the operations of the system. Below are the functionalities for each button. 

Button 1: This button reduced the target temperature of the system by 1 degree fahrenheit when the temperature change state has beeen activated
Button 2: This button changes the system's auto-update if it is on the home menu. The home menu is the menu that displayes the temperature and water level. Once the system temperature change state has been activated, button 2 changes the temperature up by 1 degree fahrenheit. 
Button 3: This changes the system menu from the home to the temperature change menu.
