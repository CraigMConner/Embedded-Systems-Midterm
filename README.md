=== ECE 387 - Analog Temperature Sensor ===

Contributor: connerc4 (Craig Conner)

=== Description ===

Included in the repository is the main C file "analogTemp.c" along with the LCD library used in class and a modified version of the python compile script. When properly wired and uploaded to the Atmega, an LCD prompts a user to press either button 1 for temp in Celsius or button 2 for temp in Fahrenheit. It finds the temp using 3 TMP36 sensors connected to the analog input pins. For every reading ADC (Analog to digital) conversion is enabled, a value is read from the sensor, and it turns ADC back off. This is to improve the accuracy by cutting out interference from all of the sensors going at once. After a button is pressed the value found is averaged, converted into a temperature scale, and displayed on the LCD screen for 5 seconds. After that it returns back to the main menu.

=== Instructions ===

* Make sure that all files are in the SAME DIRECTORY before using compile_script.py. Also appropriatly modify the script so that it reflects your computer's setting such as the port number. The final command that uploads the code to the board is different on Mac then Windows. I use a Mac so that's what is in the current script. If you have Windows you have to replace the last command with: "avrdude.exe -patmega328p -P(Port Name) -carduino -D -U flash:w:analogTemp.hex:i"

  - Then just run the command: python compile_script.py 4

=== Video Demonstration ===

https://www.youtube.com/watch?v=Of2rQjPQiyQ
