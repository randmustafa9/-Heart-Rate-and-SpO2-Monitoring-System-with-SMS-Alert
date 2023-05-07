#include <Wire.h>
#include <Adafruit_SH1106.h>
#include <MAX30100_PulseOximeter.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>
SoftwareSerial mySerial(50, 52);
Adafruit_SH1106 display(-1);
PulseOximeter pox;

#define rp 1000
uint8_t tsLastReport = 0;

void (*resetFunc)(void) = 0;

void setup() {
Serial.begin(115200);
 mySerial.begin(9600);    
display.begin(SH1106_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextColor(WHITE);
display.setTextSize(1);
display.setCursor(0,0);

mySerial.println("AT");

  mySerial.println("AT+CMGF=1"); 
  
display.print(F("Heart Rate & SpO2"));
display.setCursor(0,50);
display.print(F("By: Rand Mustafa"));
display.display();
if (!pox.begin()) {
display.setCursor(0, 20);
display.print(F("MAX30100 not found!"));
display.display();



}
pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop() {
  byte heartRate = 0;
byte spo2 = 0;
pox.update();

if (millis() - tsLastReport > rp) {
heartRate = pox.getHeartRate();
spo2 = pox.getSpO2();
tsLastReport = millis();
display.clearDisplay();
display.setCursor(0, 10);
display.print(F("Heart rate: "));
display.print(heartRate);
display.print(F(" bpm"));
display.setCursor(0, 40);
display.print(F("SpO2: "));
display.print(spo2);
display.print(F(" %"));
display.display();

  

  //while (heartRate>180) {
   for (heartRate; heartRate>180;heartRate=0) {
//      x=x+1;
  send_sms();
   display.setCursor(0, 50);
display.print(F("message has been sent"));
display.display();
delay(500);
  Serial.println(heartRate);
   if (heartRate>180) {
    
     wdt_enable(WDTO_15MS); // Enable watchdog timer with 15ms timeout
    while (1) {}
  }
}
//}

}
}

void send_sms(){
 
    mySerial.println("AT+CMGS=\"+9647702618169\""); // 1)
  updateSerial();
  mySerial.print(".");
  updateSerial();
  mySerial.write(26); // 3)
  
}


void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
