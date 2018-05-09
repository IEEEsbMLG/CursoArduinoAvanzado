#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define BACKLIGHT_PIN     13 //que es esto

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// The additional white space is intentional to make things legible and look nice.
String message = "Vamos a ver si esto funciona :)   ";
int i = 0;
int strLength;
String toShow;

void setup() {

  pinMode ( BACKLIGHT_PIN, OUTPUT );
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  
  strLength = message.length();
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 1);
  // Print a message to the LCD.
  lcd.print("Bienvenido!");
  delay(500);

  lcd.clear();
  lcd.home();
}

void loop() {
  lcd.home();

   // set the cursor to (16,1):
  lcd.setCursor(0, 2);
  // set the display to automatically scroll:
  lcd.print(message);
  
// scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < strLength+16; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(500);
  }

  //turn off automatic scrolling
  //lcd.noAutoscroll();

  // clear screen for the next loop:
  //lcd.clear();
  
  
}
