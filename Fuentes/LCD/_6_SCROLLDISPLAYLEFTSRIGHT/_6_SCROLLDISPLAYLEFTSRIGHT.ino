#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// The additional white space is intentional to make things legible and look nice.
String message = "Bienvenidos al curso de Arduino :)   ";
int i = 0;
int strLength;

void setup() {
  
  strLength = message.length();
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Bienvenido!");
  delay(500);

  lcd.clear();
  lcd.home();
}

void loop() {
  lcd.home();

   // set the cursor to (16,1):
  lcd.setCursor(16, 1);
  
  lcd.print(message);
  
  for (int positionCounter = 0; positionCounter < strLength; positionCounter++){
    // scroll one position right:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(500);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < strLength+16; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(500);
  }
  
}
