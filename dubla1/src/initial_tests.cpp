#include <Arduino.h>
#include <FastLED.h>
#include <Keypad.h> //use the Keypad libraries
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

// SEPARATE BUTTONS
const int button1 = 3;
volatile bool buttonPressed = false;
unsigned long lastDebounceTime = 0; // Variable to store the last time the output was toggled
unsigned long debounceDelay = 1000; // Delay time in milliseconds for debouncing

void buttonISR()
{
  buttonPressed = true; // Set the flag to indicate button press
}

// SERVO
Servo myservo; // create Servo object to control a servo
int pos = 0;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// BUZZER
int buzzerPin = 4;

// DISTANCE SENSOR
// defines pins numbers
const int trigPin = 11;
const int echoPin = 12;
// defines variables
long duration;
int distance;

// LED MATRIX
#define NUM_LEDS 16
#define DATA_PIN_MATRIX 13

CRGB leds[NUM_LEDS];

// BUTTON MATRIX
const int row1 = 2;
const int row2 = 3;
const int row3 = 4;
const int row4 = 5;
const int colA = 8;
const int colB = 9;
const int colC = 10;
const int colD = 11;

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
// define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {'0', '1', '2', '3'},
    {'4', '5', '6', '7'},
    {'8', '9', 'A', 'B'},
    {'C', 'D', 'E', 'F'}};

// r1 r2 r3 r4
// c1 c2c3c4
byte rowPins[ROWS] = {6, 7, 8, 9};     // connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A2, A3}; // connect to the column pinouts of the keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// put function declarations here:
int myFunction(int, int);

void setup()
{

  // initialize sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  // initialize serial connection
  Serial.begin(9600);

  // initialize led matrix
  FastLED.addLeds<WS2812, DATA_PIN_MATRIX, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(5);

  // initialize buzzer
  pinMode(buzzerPin, OUTPUT);

  // initialize lcd
  lcd.init();
  lcd.display();
  lcd.backlight();
  lcd.display();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Start game!");

  // initialize servo
  myservo.attach(10);

  pinMode(button1, INPUT);
  attachInterrupt(digitalPinToInterrupt(button1), buttonISR, FALLING);
}

void loop()
{
  // put your main code here, to run repeatedly:

  leds[10] = CRGB::Blue;
  FastLED.show();
  // delay(500);
  // // Now turn the LED off, then pause
  // leds[10] = CRGB::Black;
  // FastLED.show();

  // Clears the trigPin
  // digitalWrite(trigPin, LOW);
  // delayMicroseconds(2);
  // // Sets the trigPin on HIGH state for 10 micro seconds
  // digitalWrite(trigPin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigPin, LOW);
  // // Reads the echoPin, returns the sound wave travel time in microseconds
  // duration = pulseIn(echoPin, HIGH);
  // // Calculating the distance
  // distance = duration * 0.034 / 2;
  // // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.println(distance);

  // tone(buzzerPin, 1500, 200);
  // delay(500);
  // noTone(buzzerPin);

  // Print a message to the LCD.
  // lcd.display();
  // lcd.backlight();
  // lcd.setCursor(0, 0);
  // lcd.print("Start game!");
  // delay(500);
  // lcd.noDisplay();
  // delay(500);
  // lcd.noBacklight();
  // delay(500);
  // button matrix
  char customKey = customKeypad.getKey();

  if (customKey)
  {
    Serial.println(customKey);
    analogWrite(buzzerPin, 255);
    delay(1000);
    analogWrite(buzzerPin, 0);
  }
  else
  {
    analogWrite(buzzerPin, 0);
  }

  // servo
  /*
  myservo.write(0);
  delay(3000);*/
  for (pos = 0; pos <= 180; pos += 1)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);          // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1)
  {                     // goes from 180 degrees to 0 degrees
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);          // waits 15 ms for the servo to reach the position
  }

  // separate button
  if (buttonPressed)
  {
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
      Serial.println("Button pressed!");
      buttonPressed = false; // Reset button state
      // Add your button press handling code here
      lastDebounceTime = millis(); // Update last debounce time
    }
  }
}
