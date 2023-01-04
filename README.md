# SimpleFunctionGenerator
<br>**A simple function generator using STM32 and AD9833 (with an optional OLED screen displaying properties)**</br>
<br>Adding the files to an existing Keil STM32 project to use its functions.</br>
<br>**Target SystemBoard : STM32F103 Family (preferably STM32F103C8T6 with buttons) , AD9833 , IIRC OLED screen**</br>
<br>**Functions : Generate Square Wave , Triangle Wave , and Sinusoidal Wave From 0Hz to 12.5MHz and display current status through OLED screen **</br>
<br>**PIN CONFIGURATION :**</br>
<br>**1.OLED SCREEN:**</br>
<br>SCL -> PB6</br>
<br>SDA -> PB7</br>
<br>LED LIGHT -> PB1</br>
<br>**2.AD9833:**</br>
<br>SCLK -> PA6</br>
<br>SDATA -> PA5 </br>
<br>FSYNC -> PA7</br>
<br>**3.KEY CONTROL :**</br>
<br>KEY CHANGE FREQ -> PB10</br>
<br>KEY CHANGE WAVEFORM -> PB11</br>
