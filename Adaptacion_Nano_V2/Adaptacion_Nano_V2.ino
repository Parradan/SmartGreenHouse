 #include <Wire.h> 
 #include "RTClib.h" 
 #include <Adafruit_Sensor.h>
 #include "DHT.h"
 #include <SD.h>
 #include <SPI.h>
 #include "MQ135.h" 
 #define DHTPIN1 4 // Pin D4 del arduino nano que va al D2 del Mcu
 #define DHTPIN2 3 // Pin D3 del arduino nano que va al D2 del Mcu
 #define DHTTYPE DHT21
 #define LED_R 2    //led rojo  D2 del arduino nano
 #define chipSelect 9 // Pin D9 del nano que va al D8 del Mcu
 #define CO2PIN A2 //Pin A1 del nano  
 #define COpin1 A0 //Pin A0 del nano
 #define COpin2 A3 //Pin A3 del nano

 DS1307 Rtc;
 DHT dht1 (DHTPIN1, DHTTYPE);
 DHT dht2 (DHTPIN2, DHTTYPE);
 void RTCinit(void);
 MQ135 gasSensor = MQ135(CO2PIN); 
 
 String Fecha_s;  //Fecha en formato string
 String Hora_s;   //Hora en formato string
 boolean flag=0;
 int val1=0;
 int val2=0;
 float val_prom=0;
 File archivo;
 

 
void setup() {
    pinMode(LED_R,OUTPUT);
    pinMode(CO2PIN, INPUT); 
    pinMode(COpin1, INPUT);
    pinMode(COpin2, INPUT);
    Serial.begin(9600);
       
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

void loop() {
  DateTime now = Rtc.now();

  int h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  int h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  
  float t=(t1+t2)/2;
  int h=(h1+h2)/2;
  int val=analogRead(CO2PIN);
  float zero = gasSensor.getRZero();
  float CO2ppm = gasSensor.getPPM();
  //Serial.println(zero);
  //Serial.println(CO2ppm);
  
  val1= analogRead(COpin1);
  val2= analogRead(COpin2);
  //Serial.println(val1);
  //Serial.println(val2);
  
  val_prom=(val1+val2)/2;
  //Serial.println(val_prom);
  float COppm= map (val_prom,235,1000,5,500);
  //Serial.println(COppm);
  

  
  int hora = now.hour();
  int minutos = now.minute();
  int segundos = now.second();
  int dia= now.day();
  int mes= now.month();
  int ano= now.year();
  
  Fecha_s= String(dia)+"-"+String(mes);
  Hora_s= String(hora)+":"+String(minutos);
  //escritura_SD(Fecha_s,Hora_s,t,h);
  digitalWrite(LED_R,flag);
  //Serial.print(Hora_s);
 // Serial.println(","+String(t)+","+String(h)+","+ String(CO2ppm)+","+ String(COppm));
  delay(2000);
  
  if( minutos==0&& segundos<=1){
    Serial.print(Hora_s);
    Serial.println(","+String(t)+","+String(h)+","+ String(CO2ppm)+","+ String(COppm));
    flag = escritura_SD(Fecha_s,Hora_s,t,h,CO2ppm,COppm);
    
    delay(2000);
    }
  if( minutos==15&& segundos<=1){
     flag = escritura_SD(Fecha_s,Hora_s,t,h,CO2ppm,COppm);
     Serial.print(Hora_s);
     Serial.println(","+String(t)+","+String(h)+","+ String(CO2ppm)+","+ String(COppm));
     delay(2000);
    }
  if( minutos==30 && segundos<=1){
     flag = escritura_SD(Fecha_s,Hora_s,t,h,CO2ppm,COppm);
     Serial.print(Hora_s);
     Serial.println(","+String(t)+","+String(h)+","+ String(CO2ppm)+","+ String(COppm));
     delay(2000);
    }
  if( minutos==45 && segundos<=1){
     flag = escritura_SD(Fecha_s,Hora_s,t,h,CO2ppm,COppm);
     Serial.print(Hora_s);
     Serial.println(","+String(t)+","+String(h)+","+ String(CO2ppm)+","+ String(COppm));
     delay(2000);
    }
  while(flag==false){
    SD.begin(chipSelect);
    //Serial.print("dentro del while");
    flag = escritura_SD(Fecha_s,Hora_s,t,h,CO2ppm,COppm);
    digitalWrite(LED_R,1);
    delay(100);
    digitalWrite(LED_R,0);
    delay(100);
  }
 
}

 void RTCinit() {
    Wire.begin();
    Rtc.begin();
    //Rtc.adjust(DateTime(__DATE__, __TIME__));
    //Rtc.Enable32kHzPin(false);
    // Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
  }

 boolean escritura_SD(String dia, String hora, float temperatura, int humedad, float CO2, float CO){
  /*Parametros: 
   *  día: String formateado previamente ej: dd-mm-aaaa
   *  hora: String formateado previamente ej: hh:mm
   *  temperatura
   *  humedad
   *  CO2
   *  CO
   *  Salida: archivo con nombre "dd-mm-aaaa.txt" y formado de datos: hh:mm,tt.t,hh
   */
  String cadena;
  String nombre = dia + ".txt";
  archivo = SD.open( nombre , 0x04 | 0x02 | 0x10);  //se definen los permisos del archivo 0x04 APPEND, 0x02 WRITE, 0x10 CREATE
  cadena = hora + "," + String(temperatura) + "," + String(humedad) + ","+ String(CO2) +","+ String(CO) + "\r\n"; //aca van los datos armardos
  if(SD.exists(nombre)){
    if (archivo) {
      archivo.print(cadena);
      archivo.close();
      //digitalWrite(LED_R,HIGH);
      return true;
    } 
  }
  else{
    return false;
  }
}
