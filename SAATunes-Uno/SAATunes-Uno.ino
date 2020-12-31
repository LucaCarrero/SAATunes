#include "SAATunes.h"

//Include the file with the bytestream to interpret
#include "RagePenny.h"

SAATunes st;
int noteDuration = 150;

// [0]-ch1 ... [5]-ch6 [6]-duration
int noteList[][7] = {
 {78,88,60,0,0,0,150},
 {77,88,60,0,0,0,150},
 {0,0,0,0,0,0,150},
 {77,88,60,0,0,0,150},
 {0,0,0,0,0,0,150},
 {78,84,60,0,0,0,150},
 {77,88,60,0,0,0,150},
 {0,0,0,0,0,0,150},
 {79,83,91,0,0,0,150},
 {0,0,0,0,0,0,150},
 {0,0,0,0,0,0,150},
 {0,0,0,0,0,0,150},
 {79,64,0,0,0,0,150},
 {0,0,0,0,0,0,150},
 {0,0,0,0,0,0,150},
 {0,0,0,0,0,0,150},
};
int sequenzPosition = -1;
int delayCont = 0;
int sequenzLenght = 16;

void ICACHE_RAM_ATTR onTimerISR(){
 if(delayCont < noteDuration){
    delayCont++;
 }else{
    sequenzPosition++;
    delayCont = 0;
    noteDuration = noteList[sequenzPosition][6];
    if(sequenzPosition < sequenzLenght){
      for(int i =0;i<6;i++){
        if(noteList[sequenzPosition][i] == 0){
          st.stopNote(i);
        }else{
          st.musicNote(i, noteList[sequenzPosition][i] , 127);
        }
      }
    }else{
      sequenzPosition = 0;
    }
 }
}

void setup() {
  //Initialize the WE and A0 pins, on digital pins 9, and 8. (Change to whatever)
  Serial.begin(115200);
   
  st.init_pins(2, 0);
  st.stopNote(0);
  st.stopNote(1);
  st.stopNote(2);
  st.stopNote(3);
  st.stopNote(4);
  st.stopNote(5);
  
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(5000); //120000 us
}

void loop() {
 // delay(5024);
 // st.musicNote(4, 64, 127);
 // delay(5024);
 // st.stopNote(4);
}
