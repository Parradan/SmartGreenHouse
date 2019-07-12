#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include "DHT.h"
#include <SD.h>
#include <SPI.h>
#define DHTPIN1 0 //ACA EL PIN NO VA EL PIN (EJM  2 DEL D2), VA EL GPIO REAL DEL PUERTO DIGITAL.
#define DHTPIN2 2
#define DHTTYPE DHT21
#define chipSelect 15
#define LED_R 3 //led rojo
#define LED_G 1 //led verde


RtcDS3231<TwoWire> Rtc(Wire);
DHT dht1 (DHTPIN1, DHTTYPE);
DHT dht2 (DHTPIN1, DHTTYPE);

void RTCinit(void);

String Fecha_s;  //Fecha en formato string
String Hora_s;   //Hora en formato string

void setup () {
    Serial.begin(9600);
    pinMode(LED_R,OUTPUT);
    pinMode(LED_G,OUTPUT);
  
    
    
    
    //////////////////
    //SETUP DE LA SD//
    //////////////////
    Serial.print("Initializing SD card...");
     if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
        return;
      }
    Serial.println("card initialized.");
 
   
    /////////////////
    //SETUP DEL RTC//
    /////////////////
    RTCinit();
    
    /////////////////
    //SETUP DEL DHT//
    /////////////////
    dht1.begin();
    dht2.begin();
}

void loop () {
  
  RtcDateTime Hora_Actual = Rtc.GetDateTime();

  int h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  int h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  
  float t=(t1+t2)/2;
  int h=(h1+h2)/2;
  
  int hora = Hora_Actual.Hour();
  int minutos = Hora_Actual.Minute();
  int dia= Hora_Actual.Day();
  int mes= Hora_Actual.Month();
  int ano= Hora_Actual.Year();
  
  Fecha_s= String(dia)+"-"+String(mes)+"-"+String(ano);
  Hora_s= String(hora)+":"+String(minutos);

 

  escritura_SD(Fecha_s,Hora_s,t,h);
  
  

}

 void RTCinit() {
    Rtc.Begin();
    //Rtc.SetDateTime(__DATE__, __TIME__); //este se debe descomentar para cargar la hora desde la computadora.
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
  }

void escritura_SD(String dia, String hora, float temperatura, int humedad){
  /*Parametros: 
   *  día: String formateado previamente ej: dd-mm-aaaa
   *  hora: String formateado previamente ej: hh:mm
   *  temperatura
   *  humedad
   *  
   *  Salida: archivo con nombre "dd-mm-aaaa.txt" y formado de datos: hh:mm,tt.t,hh
   */
  String cadena;
  String nombre = dia + ".txt";
  File archivo=SD.open(nombre, 0x04 | 0x02 | 0x10);  //se definen los permisos del archivo 0x04 APPEND, 0x02 WRITE, 0x10 CREATE
  cadena = hora + "," + String(temperatura) + "," + String(humedad) + "\r\n"; //aca van los datos armardos
  if (archivo) {
    archivo.print(cadena);
    archivo.close();
  }
  else {
    Serial.println("error");
  }
  delay(1000);
}
