#include <avr/io.h>
#include<avr/interrupt.h>


// Aqui definimos los pines que vamos a usar para controlar el motor
#define IN_1 7
#define IN_2 6
#define IN_3 5
#define IN_4 4


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
        digitalWrite(IN_1,paso[paso_actual][0]);
        digitalWrite(IN_2,paso[paso_actual][1]);
        digitalWrite(IN_3,paso[paso_actual][2]);
        digitalWrite(IN_4,paso[paso_actual][3]);

        paso_actual++;

        if(paso_actual>3)
        {
          paso_actual=0;
        }
     
        TCNT1 = 0; //Ponemos el contador a 0


  sei();
}


void loop() {
  
        delay(1000);

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


