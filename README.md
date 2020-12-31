# SAATunes for ESP8266 v 0.01

This project is a porting for ESP8266 of SAATunes for Arduino.
The library is only used for managing the communication with the saa1099.
It's an alpha version, there is still a lot of work to be done. 

Technical information:
- I used pcf8575 to write data in parallel to the chip (8 bit parallel bus)
- ESP8266 timer and interrupts to turn notes on/off (https://github.com/LucaCarrero/SAATunes/blob/master/SAATunes-Uno/SAATunes-Uno.ino for timer part).

Useful links:
- Original project: https://github.com/Bobcatmodder/SAATunes
- Note Duratioon calculation: https://rechneronline.de/musik/note-length.php
- Super Mario Bross 1987 music sheet: http://www.mariopiano.com/mario-sheet-music-overworld-main-theme.html
- Arduino music sheet convertion: https://github.com/nseidle/AxelF_DoorBell/wiki/How-to-convert-sheet-music-into-an-Arduino-Sketch
- Midi note value: https://github.com/LucaCarrero/SAATunes/blob/master/midi_note_values.jpg	 
