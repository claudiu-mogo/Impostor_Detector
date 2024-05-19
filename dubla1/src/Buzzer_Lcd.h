#ifndef BUZZER_H
#define BUZZER_H

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// BUZZER
static int buzzerPin = 4;

void display_text_lcd(const char text[15]);
void small_buzz();
void big_buzz();
void init_lcd();

#endif