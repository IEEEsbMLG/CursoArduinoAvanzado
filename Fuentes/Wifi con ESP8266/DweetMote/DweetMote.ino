/*
 * Cliente Arduino usando el ESP8266
 * Recoge datos de un sensor y los envía a Dweet.io para su posterior uso.
 * 
 * Preparado para Arduino UNO.
 * 
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
#define DEBUG true
#define RX 10
#define TX 11
#define SENSOR A5
#define SERIAL_BAUDS 9600
#define ESP8266_BAUDS 115200

String AP = "SSID_NOMBRE"; // SSID - Cambiar
String PASS = "SSID_CLAVE"; // Clave del SSID - Cambiar

String HOST = "www.dweet.io";
String URL = "/dweet/for/";
String PORT = "80";
String sensorName = "sensor"; // Nombre del sensor - Opcional cambiar
String moteName = "MOTA_NOMBRE"; // Nombre de la mota - Cambiar

int valSensor = 0;

SoftwareSerial esp8266(RX,TX); // Configuramos un canal de comunicación serie UART en los pines establecidos.

// El código de SETUP se ejecuta sólo una vez, al principio.
void setup() {
  Serial.begin(SERIAL_BAUDS);
  esp8266.begin(ESP8266_BAUDS);
  sendCommand("AT+RST",5); // Reseteamos el módulo para evitar conflictos
  sendCommand("AT",5); // Comprueba que el módulo se inicie correctamente.
  sendCommand("AT+CWMODE_CUR=1",5); // Configura el modo Wi-Fi - 1=Station 2=SoftAP 3=SoftAP+Station
  sendCommand("AT+CWJAP_CUR=\""+ AP +"\",\""+ PASS +"\"",20); // // Configura el SSID y la clave del punto de acceso (AP) al que se va a conectar el módulo.

}

// El código de LOOP se ejecuta repetidamente en bucle después de SETUP.
void loop() {
  valSensor = getSensorData();
  String getData = "GET " + URL + moteName + "?" + sensorName + "=" + String(valSensor)+ " HTTP/1.1\r\n";
  sendCommand("AT+CIPMUX=0",5); // Enable single connection
  sendCommand("AT+CIPSTART=\"TCP\",\"" + HOST + "\"," + PORT, 15); // Establece la conexión TCP - single connection
  sendCommand("AT+CIPSEND=" + String(getData.length()+4), 15); // Envía los datos - single connection
  //sendCommand("AT+CIPMUX=1",5); // Enable multiple connections
  //sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15); // Establishes TCP connection - multiple connections
  //sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4); // Envía los datos - multiple connections
  esp8266.println(getData); // Escribe los datos 
  delay(1500);
  sendCommand("AT+CIPCLOSE",5); // Cierra la conexión - single connections
  //sendCommand("AT+CIPCLOSE=0",5); // Cierra la conexión - multiple connections
}

int getSensorData(){
  return analogRead(SENSOR);
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
