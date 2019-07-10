
#include <Wire.h> //I2C library
#include <RtcDS3231.h> //RTC library
RtcDS3231<TwoWire> rtcObject(Wire);
void setup() {
     Serial.begin(115200);  //Starts serial connection
     rtcObject.Begin();     //Starts I2C
     RtcDateTime currentTime = RtcDateTime(__DATE__, __TIME__);
     rtcObject.SetDateTime(currentTime);

}

void loop() {
 RtcDateTime currentTime = rtcObject.GetDateTime();

 int hora = currentTime.Hour();
 int minutos = currentTime.Minute();

  Serial.print(hora);
  Serial.print(":");
  Serial.println(minutos);
}
