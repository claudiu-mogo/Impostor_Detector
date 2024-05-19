#ifndef MYPINS_H
#define MYPINS_H

#include <Arduino.h>
#include <FastLED.h>
#include <Keypad.h> //use the Keypad libraries

// LED MATRIX
#define NUM_LEDS 16
#define led_pin 13

CRGB leds[NUM_LEDS];

// BUTTON MATRIX
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
// define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {-1, 4, 8, 12},
    {1, 5, 9, 13},
    {2, 6, 10, 14},
    {3, 7, 11, 15}};

// r1 r2 r3 r4
// c1 c2 c3 c4
byte rowPins[ROWS] = {6, 7, 8, 9};     // connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A2, A3}; // connect to the column pinouts of the keypad

#endif // MYPINS_H