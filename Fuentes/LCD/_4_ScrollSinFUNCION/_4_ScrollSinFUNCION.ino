#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// The additional white space is intentional to make things legible and look nice.
String message = "       Bienvenidos al curso avanzado de Arduino :)              ";
int i = 0;
int strLength;
String toShow;

void setup() {
  strLength = message.length();
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Bienvenido!");
  delay(1000);

  lcd.clear();
  lcd.home();
}

void loop() {
  lcd.home();
  lcd.cursor();

  // Get 16 characters so that we can display on the LCD
  toShow = message.substring(i,i+16);
  
  // print the number of seconds since reset:
  lcd.print(toShow);

  i = i+1;

  // We have to reset i after there is less text displayed.
  if(i>(strLength-16)) {
    i = 0;
  }

  delay(500);
}
