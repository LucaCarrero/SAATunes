#include <arduino.h>
#include <Wire.h>
#include "SAATunes.h"

//Volume (Velocity) variables
#define DO_VOLUME 1         // generate volume-modulating code? Needs -v on Miditones.
// pcf8575 i2c addr
#define PCF_7585_ADRESS 0x20
byte WR;
byte AO;

byte prevOctaves[] = {0, 0, 0, 0, 0, 0}; //Array for storing the last octave value for each channel
boolean SAATunes::channelActive[] = {0, 0, 0, 0, 0, 0}; //Array for storing whether a channel is active or not

void tune_playnote (byte chan, byte note, byte volume);
void tune_stopnote (byte chan);
void tune_stepscore (void);

// ******** Code Blocks ******** \\

//Pulses the WR pin connected to the SA1099 to load an address
void writeAddress() {
  digitalWrite(WR, LOW);
  delayMicroseconds(5);
  digitalWrite(WR, HIGH);
  
}

// function to write data on saa
void pf575_write(uint16_t data) 
{
  Wire.beginTransmission(0x20);
  Wire.write(lowByte(data));
  Wire.write(highByte(data));
  Wire.endTransmission();
}

void writeDataSAA(uint8_t data, uint8_t type){
  pf575_write(( type || 0 ), data);
  delayMicroseconds(5);
  pf575_write(( type || 1 ), data);
}

// class wrapper for play function
void SAATunes::musicNote(byte chan, byte note, byte volume){
	tune_playnote ( chan,  note,  volume);
}

// class wrapper for stop function
 void  SAATunes::stopNote(byte chan){
	tune_stopnote (chan);
}

//Initiate the WR and AO pins, as well as reset/enable all sound channels
void SAATunes::init_pins (byte AZ, byte WE) {
	Wire.begin();
	AO = AZ;
	WR = WE;

    pinMode(AO, OUTPUT);
    pinMode(WR, OUTPUT);
	digitalWrite(WR, HIGH);
	
	//Reset/Enable all the sound channels
	digitalWrite(AO, HIGH);
	pf575_write(word(0x00,0x1C));
    //PORTD = 0x1C;
    writeAddress();

    digitalWrite(AO, LOW);
	pf575_write(word(0x00,0x02));
    //PORTD = 0x02;
    writeAddress();

    digitalWrite(AO, LOW);
	pf575_write(word(0x00,0x00));
    //PORTD = 0x00;
    writeAddress();
	
	digitalWrite(AO, HIGH);
	pf575_write(word(0x00,0x1C));
    //PORTD = 0x1C;
    writeAddress();

    digitalWrite(AO, LOW);
	pf575_write(word(0x00,0x01));
    //PORTD = B00000001;
    writeAddress();
	
	//Disable the noise channels
	digitalWrite(AO, HIGH);
	pf575_write(word(0x00,0x15));
	//PORTD = 0x15;
	writeAddress();

	digitalWrite(AO, LOW);
	pf575_write(word(0x00,0x00));
	//PORTD = B00000000;
	writeAddress();
	
	//Disable envelopes on Channels 2 and 5
	digitalWrite(AO, HIGH);
	pf575_write(word(0x00,0x18));
	//PORTD = 0x18;
	writeAddress();

	digitalWrite(AO, LOW);
	pf575_write(word(0x00,0x00));
	//PORTD = B00000000;
	writeAddress();
	
	digitalWrite(AO, HIGH);
	pf575_write(word(0x00,0x19));
	//PORTD = 0x19;
	writeAddress();

	digitalWrite(AO, LOW);
	pf575_write(word(0x00,0x00));
	//PORTD = B00000000;
	writeAddress();
}


// Start playing a note on a particular channel

void tune_playnote (byte chan, byte note, byte volume) {
	
  //Percussion code, in this version we're ignoring percussion. 
  if (note > 127) { // Notes above 127 are percussion sounds.
	note = 60; //Set note to some random place
	volume = 0; //Then set it to 0 volume
  }
  
  byte noteAdr[] = {5, 32, 60, 85, 110, 132, 153, 173, 192, 210, 227, 243}; // The 12 note-within-an-octave values for the SAA1099, starting at B
  byte octaveAdr[] = {0x10, 0x11, 0x12}; //The 3 octave addresses (was 10, 11, 12)
  byte channelAdr[] = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D}; //Addresses for the channel frequencies

  //Shift the note down by 1, since MIDI octaves start at C, but octaves on the SAA1099 start at B
  note += 1; 

  byte octave = (note / 12) - 1; //Some fancy math to get the correct octave
  byte noteVal = note - ((octave + 1) * 12); //More fancy math to get the correct note

  prevOctaves[chan] = octave; //Set this variable so we can remember /next/ time what octave was /last/ played on this channel

  //Octave addressing and setting code:
  digitalWrite(AO, HIGH);
  pf575_write(word(0x00,octaveAdr[chan / 2]));
  //PORTD = octaveAdr[chan / 2];
  writeAddress();

  digitalWrite(AO, LOW);
  if (chan == 0 || chan == 2 || chan == 4) {
	  pf575_write(word(0x00,octave | (prevOctaves[chan + 1] << 4)));
    //PORTD = octave | (prevOctaves[chan + 1] << 4); //Do fancy math so that we don't overwrite what's already on the register, except in the area we want to.
  }
  
  if (chan == 1 || chan == 3 || chan == 5) {   
	pf575_write(word(0x00,(octave << 4) | prevOctaves[chan - 1]));
    //PORTD = (octave << 4) | prevOctaves[chan - 1]; //Do fancy math so that we don't overwrite what's already on the register, except in the area we want to.
  }
  
  writeAddress();
  
  //Note addressing and playing code
  //Set address to the channel's address
  digitalWrite(AO, HIGH);
  pf575_write(word(0x00,channelAdr[chan]));
  //PORTD = channelAdr[chan];
  writeAddress();

  //EXPERIEMNTAL WARBLE CODE
  noteAdr[noteVal] += random(-2, 2); //a plus/minus value of 15 gives a really out of tune version
  
  
  //Write actual note data
  digitalWrite(AO, LOW);
  pf575_write(word(0x00,noteAdr[noteVal]));
  //PORTD = noteAdr[noteVal];
  writeAddress();

  //Volume updating
  //Set the Address to the volume channel
  byte volAddress[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
  
	digitalWrite(AO, HIGH);
 	pf575_write(word(0x00, volAddress[byte(chan)]));
	writeAddress();

  #if DO_VOLUME
	//Velocity is a value from 0-127, the SAA1099 only has 16 levels, so divide by 8.
     byte vol = volume / 8;

	digitalWrite(AO, LOW);
	pf575_write(word(0x00,(vol << 4) | vol));
	//PORTD = (vol << 4) | vol;
	writeAddress();
  #else
		
	//If we're not doing velocity, then just set it to max.
	digitalWrite(AO, LOW);
	pf575_write(word(0x00,0xFF));
	//PORTD = B11111111;
	writeAddress();
  
  #endif
}


//-----------------------------------------------
// Stop playing a note on a particular channel
//-----------------------------------------------

void tune_stopnote (byte chan) {
  	byte volAddress[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
  
	digitalWrite(AO, HIGH);
	//PORTD = volAddress[byte(chan)];
 	pf575_write(word(0x00, volAddress[byte(chan)]));
	writeAddress();

	digitalWrite(AO, LOW);
	//PORTD = 0x00;
  	pf575_write(word(0x00,0x00));
	writeAddress();
}

