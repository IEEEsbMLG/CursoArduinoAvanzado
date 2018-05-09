#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Creat a set of new characters
const uint8_t charBitmap[][8] = {
   { 0x00, 0x11, 0x00, 0x00, 0x11, 0x0E, 0x00}   
};

/*byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};*/


void setup()
{
   int charBitmapSize=(sizeof(charBitmap )/sizeof(charBitmap[0]));


   lcd.begin(16,2);               // initialize the lcd

   for ( int i = 0; i < charBitmapSize; i++ )
   {
      lcd.createChar ( i, (uint8_t *)charBitmap[i] );
   }
  
}

void loop()
{
  lcd.setCursor(0,0);
  lcd.write(byte(0));
}

