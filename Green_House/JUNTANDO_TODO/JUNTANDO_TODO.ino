//////////////////////////////////////////////////////////////////
///////////////// Smart GreenHouse V1.0 //////////////////////////
//////////////////////////////////////////////////////////////////
/////// Notas: Conexion de sensores GND: Azul-BlancoAzul /////////
///////                        Dato: BlancoNaraja-BlancoMarron ///
///////                          VCC: Naranja-Marron /////////////
//////////////////////////////////////////////////////////////////

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include "DHT.h"
#include <SD.h>
#include <SPI.h>
#define DHTPIN1 0 //ACA EL PIN NO VA EL PIN (EJM  2 DEL D2), VA EL GPIO REAL DEL PUERTO DIGITAL
#define DHTPIN2 2 //
#define DHTTYPE DHT21
#define chipSelect 15
#define LED_R 3 //led rojo  RX
#define AUX 16 //SALIDA AUX


RtcDS3231<TwoWire> Rtc(Wire);
DHT dht1 (DHTPIN1, DHTTYPE);
DHT dht2 (DHTPIN2, DHTTYPE);

void RTCinit(void);
boolean escritura_SD(String dia, String hora, float temperatura, int humedad);

String Fecha_s;  //Fecha en formato string
String Hora_s;   //Hora en formato string
boolean flag=0;
boolean f=0;
File archivo;

void setup () {
    pinMode(LED_R,OUTPUT);
   // Serial.begin(9600);
       
    //////////////////
    //SETUP DE LA SD//
    //////////////////
    SD.begin(chipSelect);
    
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
  int segundos = Hora_Actual.Second();
  int dia= Hora_Actual.Day();
  int mes= Hora_Actual.Month();
  int ano= Hora_Actual.Year();
  
  Fecha_s= String(dia)+"-"+String(mes)+"-"+String(ano);
  Hora_s= String(hora)+":"+String(minutos);
  //flag = escritura_SD(Fecha_s,Hora_s,t,h);
  
  digitalWrite(LED_R,flag);
  if( minutos==0 && segundos<=1){
    flag = escritura_SD(Fecha_s,Hora_s,t,h);
    delay(1010);
    }
  if( minutos==15 && segundos<=1){
     flag = escritura_SD(Fecha_s,Hora_s,t,h);
     delay(1010);
    }
  if( minutos==30 && segundos<=1){
     flag = escritura_SD(Fecha_s,Hora_s,t,h);
     delay(1010);
    }
  if( minutos==45 && segundos<=1){
     flag = escritura_SD(Fecha_s,Hora_s,t,h);
     delay(1010);
    }
  while(flag==false){
    SD.begin(chipSelect);
    flag = escritura_SD(Fecha_s,Hora_s,t,h);
    digitalWrite(LED_R,1);
    delay(100);
    digitalWrite(LED_R,0);
    delay(100);
  }
 
}

 void RTCinit() {
    Rtc.Begin();
   // Rtc.SetDateTime(RtcDateTime(__DATE__,__TIME__)); //este se debe descomentar para cargar la hora desde la computadora.
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
  }

boolean escritura_SD(String dia, String hora, float temperatura, int humedad){
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
  archivo = SD.open(nombre, 0x04 | 0x02 | 0x10);  //se definen los permisos del archivo 0x04 APPEND, 0x02 WRITE, 0x10 CREATE
  cadena = hora + "," + String(temperatura) + "," + String(humedad) + "\r\n"; //aca van los datos armardos
  if(SD.exists(nombre)){
    if (archivo) {
      archivo.print(cadena);
      digitalWrite(LED_R,HIGH);
      return true;
    } 
  }
  else{
    return false;
  }
}
