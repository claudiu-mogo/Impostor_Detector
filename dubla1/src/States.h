#ifndef STATES_H
#define STATES_H

#define CHECK_DISTANCE 0
#define BLINK_LEDS 1
#define REPLICATE 2
#define GG 3

int current_state = CHECK_DISTANCE;
int led_time = 1000;
int pop_up_led_time = 300;
int current_leds[10];
int level = 1;
int array_index = 0;
int max_level = 3;
bool level_flag;

#endif
