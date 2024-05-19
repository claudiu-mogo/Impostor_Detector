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
    // Calculating the distance
    distance = duration * 0.034 / 2;

    // Return distance only if it's within a realistic range
    if (distance >= 1 && distance <= 400)
    {
        return distance;
    }
    // Return an invalid distance if the measurement is out of range
    return -1;
}

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
        return -1; // No valid readings
    }

    // Calculate the avg value
    return sum / validReadings;
}
