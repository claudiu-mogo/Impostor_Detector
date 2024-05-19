#ifndef STATES_H
#define STATES_H

#define CHECK_DISTANCE 0
#define BLINK_LEDS 1
#define REPLICATE 2
#define GG 3

int current_state = CHECK_DISTANCE;
int led_time = 1000;
int current_leds[10];
int level = 1;
int array_index = 0;
int max_level = 4;

#endif
