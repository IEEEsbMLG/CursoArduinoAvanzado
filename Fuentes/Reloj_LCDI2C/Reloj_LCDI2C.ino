#include <LiquidCrystal_I2C.h>
#include <avr/interrupt.h>
#include <TimerOne.h>
#include <Stepper.h>



const int stepsPerRev = 200;

//LiquidCrystal_I2C lcd(0x27, 3, POSITIVE);  // Set the LCD I2C address, BACKLIGHT pin and polarity
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Stepper motor(stepsPerRev, 7, 6, 5, 4);

uint8_t count = 0;

// Creat a set of new characters
const uint8_t charBitmap[][8] = {
   { 0x1f, 0x11, 0x15, 0x15, 0x17, 0x11, 0x1f}   
};

uint8_t m = 0;
uint8_t s = 0;
uint8_t h = 0;

uint8_t Refresh_flag = 1; //This flag indicates that the count has change and the LCD must be refreshed

void setup()
{
  //10ms timer setup. This timer will be used to perform the clock
   Timer1.initialize(10000);  // 10 ms
   Timer1.attachInterrupt(ISR_Clock);

  
  
   int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));


   lcd.begin(16,2);               // initialize the lcd

   for ( int i = 0; i < charBitmapSize; i++ )
   {
      lcd.createChar ( i, (uint8_t *)charBitmap[i] );
   }
  
  lcd.clear(); //Clear LCD and go home
  lcd.print("Curso avanzado");  
  //delay(500);
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("Arduino");
  //delay ( 1000 );
  lcd.home();
  lcd.clear();
  lcd.print ("IEEE Malaga");
}

void loop()
{
  if(Refresh_flag > 0){
      Timer1.detachInterrupt();
      lcd.setCursor(0, 1);
      lcd.print(char(0));
      if(h < 10)
          lcd.print("0");
      lcd.print(h);
      lcd.print(":");
      if(m < 10)
      lcd.print("0");
      lcd.print(m);
      lcd.print(":");
      if(s < 10)
          lcd.print("0");
      lcd.print(s);
      Refresh_flag = 0;
      Timer1.attachInterrupt(ISR_Clock);
  }
}


void ISR_Clock(){// This interrupt will be shoot every 10ms
  cli();
  count++;
  
  if(count == 100){
    s += 1; // Increase in one second
    if(s == 60){ 
        s=0; 
        m=m+1; 
    } 
    if(m == 60){ 
        m=0; 
        h=h+1; 
    } 
    if(h == 24)
      h=0;
    count = 0; // Restart the 10 cycle of 10ms each
    Refresh_flag = 1;
  }  
  sei();
}

