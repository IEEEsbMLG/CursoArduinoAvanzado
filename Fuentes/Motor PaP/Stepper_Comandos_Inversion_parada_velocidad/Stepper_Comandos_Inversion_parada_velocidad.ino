#include <avr/io.h>
#include<avr/interrupt.h>


// Aqui definimos los pines que vamos a usar para controlar el motor
#define IN_1 7
#define IN_2 6
#define IN_3 5
#define IN_4 4

#define NORMAL 1
#define INVERSA 0
#define ACTIVO 1
#define STOP 0

int direccion = NORMAL;
int activo = ACTIVO;
int velocidad = 50;

int paso [4][4] =
{
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1},
  {1, 0, 0, 1}
};

void setup() 
{
  cli();

  Serial.begin(9600);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  
  // Configuracion del timer TC0 (8-BITS)
  TIMSK1 |= (1<<OCIE1A); //Mascara de interrupciones del timer 1 para hacer saltar interrupcion una vez alcanzado OCR1A;
  TCCR1B |= 0b00000010  ;//Preescaler timer 1 puesto a 8 --- 1/16.000.000 Mhz = 0,0000000625s * 8prescaler = 0,0000005s por cada pulso de reloj --- Valor de OCR1A para que sea 1rpm --- 60s/4096pasos.vuelta = 0,0146484375s por pulso ---- valor OCR1A = 0,0146484375s /0,0000005s = 29296,875 ~ 29297.
  //OCR1A = 256;//29297; //Valor del contador que hará saltar una interrupcion.
  TCCR1A = 0; // HAY QUE PONERLO A 0 Arduino lo preconfigura como si fuera de 8 bits.
  OCR1A = 8000;
  TCNT1 = 0; //Ponemos el contador a 0

  sei();

}

ISR(TIMER1_COMPA_vect)
{
  static int paso_actual = 0; // Declarado static para que quede guardado en memoria entre llamadas a la rutina de interrupcion.
  // Va de 0 a 3 y es para alternar entre los valores que mueven el motor.
  cli();
    if(activo)
    {
        digitalWrite(IN_1,paso[paso_actual][0]);
        digitalWrite(IN_2,paso[paso_actual][1]);
        digitalWrite(IN_3,paso[paso_actual][2]);
        digitalWrite(IN_4,paso[paso_actual][3]);

        if(direccion)
        {
          paso_actual++;
        }
        else
        {
          paso_actual--;
        }
        

        if(paso_actual>3)
        {
          paso_actual=0;
        }
        if(paso_actual<0)
        {
          paso_actual=3;
        }

        TCNT1 = 0; //Ponemos el contador a 0
    }


  sei();
}


void loop() 
{
   if(Serial.available())
    {
      int comando = Serial.read();
      Serial.flush();

      switch(comando)
      {
        case 'n': cli(); direccion = NORMAL; sei(); break;
        case 'i': cli(); direccion = INVERSA; sei(); break;
        case 'a': cli(); activo = ACTIVO; sei(); break;
        case 's': cli(); activo = STOP; sei(); break;
        case '+': cli(); 
                  velocidad+=10;
                  if(velocidad>100)
                  {
                    velocidad=100;
                  }
                  OCR1A = map(velocidad, 0, 100, 10000, 500);
                  sei();
                  break;
        
        case '-': cli(); 
                  velocidad-=10;
                  if(velocidad<0)
                  {
                    velocidad=0;
                  }
                  OCR1A = map(velocidad, 0, 100, 10000, 500);
                  sei();
                  break;
                  
      }
    }
    delay(100);

}


void depuracion()
{
  static int paso_actual = 0;
        digitalWrite(IN_1,paso[paso_actual][0]);
        digitalWrite(IN_2,paso[paso_actual][1]);
        digitalWrite(IN_3,paso[paso_actual][2]);
        digitalWrite(IN_4,paso[paso_actual][3]);

        paso_actual++;

        if(paso_actual>=4)
        {
          paso_actual=0;
        }
        delay(10);
}


