#include "Buzzer_Lcd.h"

// LCD
static LiquidCrystal_I2C lcd(0x27, 16, 2);

void init_lcd()
{
    lcd.init();
    lcd.display();
    lcd.backlight();
}

// make it easier for me to write on lcd
void display_text_lcd(const char text[15])
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(text);
}

// user midgame checks
void small_buzz()
{
    tone(buzzerPin, 1500, 200);
    delay(500);
    noTone(buzzerPin);
}

// fail sound
void big_buzz()
{
    for (int i = 0; i < 7; i++)
    {
        tone(buzzerPin, 1500, 200);
        delay(500);
        noTone(buzzerPin);
    }
}
