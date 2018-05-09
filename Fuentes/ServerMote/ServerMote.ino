/*
  Arduino Webserver using ESP8266
  Displays temperature in a webpage

  Arduino Mega has three Serial communication ports,this code works well with 
  Arduino Mega.For UNO users,use Softserial library to connect ESP8266 with
  Arduino UNO 
  If you're unsure about your Arduino model or its pin cofigurations,please check
  the documentation at http://www.arduino.cc
 
  modified August 2016
  By Joyson Chinta and Gladson Kujur

  modificado en mayo de 2018
  por Juan Luis Rodríguez
  
 * Esp8266 | Arduino 
 * — — — — — — — — -
 *      RX | 11
 *      TX | 10
 *     GND | GND
 *     VCC | 3.3v
 *   CH_PD | 3.3v
 *  GPIO 0 | Nada
 *  GPIO 2 | Nada
 */
#include <SoftwareSerial.h>

#define DEBUG false
#define RX 10
#define TX 11
#define PIN 13
#define SERIAL_BAUDS 9600
#define ESP8266_BAUDS 115200

String AP = "ESP8266"; // SSID
String PASS = "1234567890"; // Clave del SSID
String CHANNEL = "6";
String ENCRYPTION = "3"; // 0: Abierto - 2: WPA_PSK - 3: WPA2_PSK - 4: WPA_WPA2_PSK
String PORT = "80";
int connectionId;

String cabecera = "<!DOCTYPE html><html><head></head><body><h1>Sensor Arduino</h1>";
String cola = "</body></html>";

SoftwareSerial esp8266(RX,TX);
 
void setup()
{
  Serial.begin(SERIAL_BAUDS); 
  esp8266.begin(ESP8266_BAUDS);
  pinMode(PIN,OUTPUT); // Configuramos el PIN para modificar
  digitalWrite(PIN,LOW);
 
  sendCommand("AT+RST",5); // Reseteamos el módulo para evitar conflictos
  sendCommand("AT",5); // Comprueba que el módulo se inicie correctamente.
  sendCommand("AT+CWMODE_CUR=2",1); // Configura el modo Wi-Fi - 1=Station 2=SoftAP 3=SoftAP+Station
  sendCommand("AT+CWSAP_CUR=\"" + AP + "\",\""+ PASS +"\"," + CHANNEL + "," + ENCRYPTION,1); // Configuramos SSID, clave, canal y modo de seguridad.
  sendCommand("AT+CIFSR",1); // Devuelve la IP local
  sendCommand("AT+CIPMUX=1",1); // Configuramos múltiples conexiones
  sendCommand("AT+CIPSERVER=1," + PORT, 1); // Activa un servidor en el puerto especificado.
}


float getSensorData() {
  int val = analogRead(A5);
  float mv = ( val/1024.0)*5000; 
  float celsius = mv/10;
  return(celsius);
}

void loop() {
  if(esp8266.available()) {
    if(esp8266.find("+IPD,")) { // +IPD es la señal que nos indica que hay una petición pendiente
      delay(300);
      connectionId = esp8266.read()-48; // Convierte ASCII a int y obtiene el identificador de conexión
      if (esp8266.find("GET")) { // Obtenemos una petición GET. Como sólo tenemos una página, ignoramos la URL pedida.
        Serial.println("Recibimos GET");
        String cuerpo;
        if (esp8266.find("pin=")) { // Obtenemos la petición de cambiar el pin
          Serial.println("Recibimos pin=");
          int pinNumber = (esp8266.read()-48)*10; // Convierte ASCII a int y usa la cifra para las decenas.
          pinNumber += (esp8266.read()-48); // Convierte ASCII a int y usa la cifra para las unidades.

          if (pinNumber == PIN) { // Nos aseguramos de que el pin es el que tenemos configurado para evitar errores
            Serial.println("Cambiamos el pin " + String(pinNumber));
            cuerpo = "<p>Cambiado el pin ";
            cuerpo += String(pinNumber);
            cuerpo += "</p>";
            digitalWrite(pinNumber, !digitalRead(pinNumber)); // Alternamos el valor del pin
          } else {
            Serial.println("No se puede cambiar el pin " + String(pinNumber));
            cuerpo = "<p>No se puede cambiar el pin solicitado</p>";
          }
        } else { // No obtuvimos la petición de cambiar el pin, luego sólo nos queda mostrar la temperatura.
          Serial.println("Mostramos temperatura");
          cuerpo = "<p>Temperatura=";
          String valorSensor =  String(getSensorData(), 3);
          cuerpo += valorSensor;
          cuerpo +="&#x2103"; // Código hexadecimal de grados Celsius;
          cuerpo +="</p>";
        }
        espsend(cabecera);
        espsend(cuerpo);
        espsend(cola);
        
      } else {
        Serial.println("No recibimos GET. Mostrando mensaje oculto");
        espsend("Has descubierto el secreto!");
        delay(1000); // Esperamos antes de cerrar la conexión para que haya tiempo de leerlo.
      }
      String closeCommand = "AT+CIPCLOSE="; //Cierra la conexión
      closeCommand+=connectionId; // Añadimos el ID de conexión
      closeCommand+="\r\n";
      sendCommand(closeCommand,3);
    }
  }
}

// Ejemplo de cómo podría empezar a construirse una librería para manejar el ESP8266
// encapsulando el comando AT+CIPSEND

void espsend(String d) { 
  String cipSend = " AT+CIPSEND=";
  cipSend += connectionId; 
  cipSend += ",";
  cipSend +=d.length();
  cipSend +="\r\n";
  sendCommand(cipSend,1);
  sendCommand(d,1); 
}
   
void sendCommand(String command, int maxTime) {
  Serial.print("AT command => ");
  Serial.print(command);
  Serial.print(" ");
  esp8266.println(command);
  
  // En este punto el comando ya está enviado. Ahora recibimos la respuesta
  
  String response = "";
  int timeout = maxTime*1000;
  long int time = millis();
  while((time+timeout) > millis()) { // Damos el tiempo de timeout para leer la respuesta del ESP8266
    while(esp8266.available()) {
      char c = esp8266.read();
      response+=c;
    }
  }
  if (response.indexOf("OK") != -1) {
    Serial.println("OK");
  } else {
    Serial.println("ERROR");
  }
  if (DEBUG) {
    Serial.println(response);
  }
}
