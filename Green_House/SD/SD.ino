#include <SD.h>
#include <SPI.h>


const int chipSelect = 9;


void escritura_SD(int dia, int hora, int temperatura, int humedad);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  escritura_SD("11-07-1991","16:44", 24.7 ,85);
  escritura_SD("12-07-1991","16:44", 24.7 ,85);
  

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
