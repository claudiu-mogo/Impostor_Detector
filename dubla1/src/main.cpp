#include "My_pins.h"
#include "States.h"
#include <stdlib.h>
#include <time.h>
#include "Distance_sensor.h"
#include "Buzzer_Lcd.h"
#include "song.h"

#include <Wire.h>
#include <Servo.h>

// SERVO
Servo myservo; // create Servo object to control a servo
int pos;

// instatiate button matrix
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// safety measure -- close all leds
void clear_leds()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

// generate a random sequence of length level
void random_led_sequence()
{
    display_text_lcd("WATCH!");
    int chosen_led;
    for (int i = 0; i < level; i++)
    {
        chosen_led = rand() % 16;
        current_leds[i] = chosen_led;
        leds[chosen_led] = CRGB::Blue;
        FastLED.show();
        delay(led_time);
        // Now turn the LED off, then pause
        leds[chosen_led] = CRGB::Black;
        FastLED.show();
    }
}

// interrupt for "increased difficulty"
void button1ISR()
{
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (current_state == CHECK_DISTANCE)
        {
            // update total number of levels
            max_level = min(7, max_level + 1);
            level_flag = true;
            lastDebounceTime = millis(); // Update last debounce time
            return;
        }

        // during the game, change up times for the leds
        led_time -= 200;
        led_time = max(100, led_time);
        Serial.println(led_time);
        lastDebounceTime = millis(); // Update last debounce time
    }
}

// interrupt for "decreased difficulty"
void button2ISR()
{
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (current_state == CHECK_DISTANCE)
        {
            // update total number of levels
            max_level = max(1, max_level - 1);
            level_flag = true;
            lastDebounceTime = millis(); // Update last debounce time
            return;
        }

        // during the game, change up times for the leds
        led_time += 200;
        led_time = min(2000, led_time);
        Serial.println(led_time);
        lastDebounceTime = millis(); // Update last debounce time
    }
}

// set interrupts on FALLING
void my_interrupt()
{
    sei();
    EICRA = 0;
    EICRA |= (1 << ISC11) | (1 << ISC01);
    EIMSK = 0;
    EIMSK |= (1 << INT1) | (1 << INT0);
}

ISR(INT0_vect)
{
    button2ISR();
}

ISR(INT1_vect)
{
    button1ISR();
}

void setup()
{
    // initialize sensor
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // initialize serial connection
    Serial.begin(9600);

    // initialize led matrix
    FastLED.addLeds<WS2812, led_pin, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(5);
    clear_leds();

    // initialize buzzer
    pinMode(buzzerPin, OUTPUT);

    // initialize lcd
    init_lcd();
    display_text_lcd("START GAME!");
    level_flag = false;

    // initialize servo
    myservo.attach(10);
    pos = 0;
    myservo.write(pos);

    // put time seed
    srand(time(0));

    my_interrupt();
    // add interrupts to difficulty buttons
    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
}

void loop()
{
    if (current_state == CHECK_DISTANCE)
    {
        // got an interrupt for changing max_level
        if (level_flag)
        {
            char current_str[20];
            sprintf(current_str, "LEVELS: %d", max_level);
            display_text_lcd(current_str);
            level_flag = false;
            delay(1000);
        }
        display_text_lcd("START GAME!");
        long dist = get_average_distance();

        // check threshold for distance
        if (dist <= 3.5)
        {
            debounceDelay = MIDGAME_DEBOUNCE_DELAY;
            current_state = BLINK_LEDS;
        }
    }
    else if (current_state == BLINK_LEDS)
    {
        // generate sequence and go to REPLICATE
        display_text_lcd("BEWARE!");
        delay(2000);
        random_led_sequence();
        current_state = REPLICATE;
        display_text_lcd("WAIT!");
        delay(500);
        display_text_lcd("REPLICATE!");
        array_index = 0;
        small_buzz();
    }
    else if (current_state == REPLICATE)
    {
        char customKey = customKeypad.getKey();
        if (customKey)
        {
            // compromise for the validity of the above condition
            customKey = max(0, customKey);
            if (customKey == current_leds[array_index])
            {
                // mark the led as correct
                leds[customKey] = CRGB::Red;
                FastLED.show();
                delay(pop_up_led_time);
                leds[customKey] = CRGB::Black;
                FastLED.show();

                // go to the next element
                array_index++;
                if (array_index == level)
                {
                    // finished level
                    array_index = 0;
                    current_state = BLINK_LEDS;
                    level++;
                    if (level > max_level)
                    {
                        // finished game
                        current_state = GG;
                        level = 1;
                    }
                }
                small_buzz();
            }
            else
            {
                // mark the led as wrong
                leds[customKey] = CRGB::Green;
                FastLED.show();
                delay(pop_up_led_time);
                leds[customKey] = CRGB::Black;
                FastLED.show();

                // reset the game
                array_index = 0;
                level = 1;
                big_buzz();
                debounceDelay = INITIAL_DEBOUNCE_DELAY;
                current_state = CHECK_DISTANCE;
            }
        }
        else
        {
            analogWrite(buzzerPin, 0);
        }
    }
    else if (current_state == GG)
    {
        // done game, open up the prize
        display_text_lcd("GG!");
        for (pos = 0; pos <= 180; pos += 1)
        { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            myservo.write(pos);
            delay(15);
        }
        for (pos = 180; pos >= 0; pos -= 1)
        { // goes from 180 degrees to 0 degrees
            myservo.write(pos);
            delay(15);
        }
        sing();

        // restart game if needed
        debounceDelay = INITIAL_DEBOUNCE_DELAY;
        current_state = CHECK_DISTANCE;
    }
}
