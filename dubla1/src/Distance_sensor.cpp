#include "Distance_sensor.h"

// defines variables
long duration;
int distance;
int numReadings = 10;

long read_distance()
{
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    // compute the distance
    distance = duration * 0.034 / 2;

    // return distance only if it is within a realistic range
    if (distance >= 1 && distance <= 400)
    {
        return distance;
    }
    // return an invalid distance if the measurement is out of range
    return -1;
}

// compute the average of 10 consecutive measurements
long get_average_distance()
{
    long sum = 0;
    int validReadings = 0;

    for (int i = 0; i < numReadings; i++)
    {
        long dist = read_distance();
        if (dist != -1)
        {
            sum += dist;
            validReadings++;
        }
        delay(50); // delay between readings
    }

    if (validReadings == 0)
    {
        return 100; // no valid readings
    }

    // calculate the avg value
    return sum / validReadings;
}
