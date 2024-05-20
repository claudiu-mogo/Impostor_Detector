#ifndef STATES_H
#define STATES_H

// states macros
#define CHECK_DISTANCE 0
#define BLINK_LEDS 1
#define REPLICATE 2
#define GG 3

// initial state
int current_state = CHECK_DISTANCE;

// default period for a led to be lit
int led_time = 1000;

// period for a led to be lit to represent right or wrong
int pop_up_led_time = 300;

// array to memorise the current leds to guess
int current_leds[10];

// starting level
int level = 1;

// starting point for array
int array_index = 0;

// default max level
int max_level = 3;

// boolean flag set in interrupts to change display in LCD
bool level_flag;

#endif
