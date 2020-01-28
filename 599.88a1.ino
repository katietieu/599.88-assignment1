/*
 * CPSC 599.88 Physical & Tangible HCI, Winter 2020
 * Assignment 1 - Arduino Hello World
 * A button which toggles an LED and simultaneously plays notes from "Running in the 90's"
 * Some code adapted from https://www.arduino.cc/en/Tutorial/ToneMelody and https://www.arduino.cc/en/Tutorial/StateChangeDetection
 *
 * Katie Tieu
 * 30018168
*/

#include "pitches.h"

const int buttonPin = 4;  // number of pushbutton pin
const int ledPin = 12 ;  // number of LED pin
const int buzzerPin = 7; // number of piezo buzzer pin

int buttonState = 0;  // variable for reading pushbutton status
int buttonPushCounter = 0;  // counts number of times button has been pressed
int lastButtonState = 0;  // previous state of the button

// notes in the melody:
int melody[] = {
  NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_D5, NOTE_CS5, NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_DS5, \
  NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_D5, NOTE_CS5, NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, \
  NOTE_A3, NOTE_E3, NOTE_A3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_A3, NOTE_E3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, \
  NOTE_C4, NOTE_B3, NOTE_G3, NOTE_C4, NOTE_B3, NOTE_G3, NOTE_C4, NOTE_B3, NOTE_G3, NOTE_D4, NOTE_B3, NOTE_G3, \
  NOTE_A3, NOTE_E3, NOTE_A3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_A3, NOTE_E3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, \
  NOTE_E5, NOTE_DS5, NOTE_D5, NOTE_CS5, NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5 /* 70 */ , \
  NOTE_A4, NOTE_E4, NOTE_A4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_E4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, \
  NOTE_C5, NOTE_B4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_D5, NOTE_B4, NOTE_G4, \
  NOTE_A4, NOTE_E4, NOTE_A4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_E4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, \
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_DS5, NOTE_DS5 /* 118 */, \
  NOTE_A4, NOTE_E4, NOTE_A4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_E4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, \
  NOTE_C5, NOTE_B4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_D5, NOTE_B4, NOTE_G4, \
  NOTE_A4, NOTE_E4, NOTE_A4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_E4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, \
  NOTE_E5, NOTE_DS5, NOTE_D5, NOTE_CS5, NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5 /* 164 */
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  8, 3, 8, 3, 8, 8, 8, 8, 8, 8, 8, 8, 8, 3, \
  8, 3, 8, 8, 8, 8, 8, 8, 8, 8, \
  16, 16, 16, 16, 16, 16, 8, 16, 16, 16, 16, 16, 16, \
  16, 16, 16, 8, 16, 8, 16, 16, 16, 8, 16, 8, \
  16, 16, 16, 16, 16, 16, 8, 16, 16, 16, 16, 16, 16, \
  8, 8, 8, 8, 8, 8, 8, 8, \
  16, 16, 16, 16, 16, 16, 8, 16, 16, 16, 16, 16, 16, \
  16, 16, 16, 8, 16, 8, 16, 16, 16, 8, 16, 8, \
  16, 16, 16, 16, 16, 16, 8, 16, 16, 16, 16, 16, 16, \
  16, 8, 16, 8, 8, 16, 8, 16, 16, 8, \
  16, 16, 16, 16, 16, 16, 8, 16, 16, 16, 16, 16, 16, \
  16, 16, 16, 8, 16, 8, 16, 16, 16, 8, 16, 8, \
  16, 16, 16, 16, 16, 16, 8, 16, 16, 16, 16, 16, 16, \
  8, 8, 8, 8, 8, 8, 8, 8
};


void setup() {
  pinMode(buttonPin, INPUT);  // button is input
  pinMode(ledPin, OUTPUT);  // light is output
  pinMode(buzzerPin, OUTPUT); // buzzer is output
  
  Serial.begin(9600); // set up Serial library at 9600 bits per second (to count button presses)
}

// main code which runs repeatedly
void loop() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);

      // one note per button press at the beginning of the song
      if ((buttonPushCounter >= 1) || (buttonPushCounter <= 12)) {
        tone(7, melody[buttonPushCounter - 1], (1000 / noteDurations[buttonPushCounter - 1]) );
      }

      if (buttonPushCounter >= 13) {
        // iterate over the notes of the melody:
        for (int thisNote = 12; thisNote < 164; thisNote++) {   // 70 / 118 / 164 for all notes
          
          // to calculate the note duration, take one second divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / noteDurations[thisNote];
          tone(7, melody[thisNote], noteDuration);
      
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          // stop the tone playing:
          noTone(7);
        }

        buttonPushCounter = 0;
      }
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;


  // turns on the LED every four button pushes by checking the modulo of the
  // button push counter. the modulo function gives you the remainder of the
  // division of two numbers:
  if (buttonPushCounter % 2 == 0) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }

}
