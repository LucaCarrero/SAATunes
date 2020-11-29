/*********************************************************************************************

     SAATunes v 1.04
	 
	 A library to play MIDI tunes on the SAA1099 using an arduino. 
	 For documentation, see SAATunes.cpp
	 
   Copyright (c) 2011, 2016, Len Shustek
   SAA1099 version Copyright (c) 2018, Jacob Field
   
**************************************************************************************/
/*
  Change log
   25 September 2017, Jacob Field
     - Original (release) library version, v1.03
	 16 August 2018, Jacob Field
	- Fixed reported bug: first letter of <arduino.h> in the #include line not being capitalized was breaking library on linux
	- Released as v1.04
  -----------------------------------------------------------------------------------------*/

#ifndef SAATunes_h
#define SAATunes_h

#include <Arduino.h>

class SAATunes
{
public:
 void init_pins(byte AZ, byte WE); 				// Define the pins that WE and AO are connected to (Naming conflict with AO, which is why it's AZ)
 static unsigned int decayRate;					// 1/16 of the rate in Milliseconds in which to decay notes
 static boolean channelActive[];        		// An array which stores which channels are active and which are no
 void musicNote(byte chan, byte note, byte volume);
 void stopNote(byte chan);
};

#endif