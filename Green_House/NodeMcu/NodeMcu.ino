#include "DHT.h"
#include <Wire.h> //I2C library
#include <RtcDS3231.h> //RTC library
#define DHTPIN 4 //ACA EL PIN NO VA EL PIN (EJM  2 DEL D2), VA EL GPIO REAL DEL PUERTO DIGITAL.
#define DHTTYPE DHT21
DHT dht(DHTPIN, DHTTYPE);


RtcDS3231<TwoWire> rtcObject(Wire);


void setup() {
     Serial.begin(115200);  
     rtcObject.Begin();    
     RtcDateTime currentTime = RtcDateTime(__DATE__, __TIME__);
     rtcObject.SetDateTime(currentTime);
   
     dht.begin();
     
}

void loop() {
  
 RtcDateTime currentTime = rtcObject.GetDateTime();

  int hora = currentTime.Hour();
  int minutos = currentTime.Minute();

  Serial.print(hora);
  Serial.print(":");
  Serial.println(minutos);

  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C ");

  
}
