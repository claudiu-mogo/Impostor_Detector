#ifndef DISTANCE_H
#define DISTANCE_H
#include <Arduino.h>

// DISTANCE SENSOR
// defines pins numbers
const int trigPin = 11;
const int echoPin = 12;

long read_distance();
long get_average_distance();

#endif
