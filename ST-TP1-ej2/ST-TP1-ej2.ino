#include <Arduino.h>
#include <ESP32Time.h>
#include "time.h"
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define BOTON1 34
#define BOTON2 35



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//**********************************************************//


//* DECLARO DE FUN                                        **//


//**********************************************************//


void pedir_lahora(void);

void setup_rtc_ntp(void);

struct tm timeinfo;

ESP32Time rtc;


long unsigned int timestamp; // hora
const char *ntpServer = "south-america.pool.ntp.org";
long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

const char* ssid = "ORT-IoT";

const char* password = "OrtIOTnew22$2";

int funcionamiento;
int lecturaBoton1;
int lecturaBoton2;


int estadoBoton;
int estadoBoton1;
int estadoBoton2;
int estadoBoton3;
int estadoPantalla;


int hora;
int minutos;
int gmt = -3;


void setup() {
  
 pinMode(BOTON1, INPUT_PULLUP);
 pinMode(BOTON2, INPUT_PULLUP);
 Serial.begin(115200);
 Serial.println("Connecting to Wi-Fi...");
 initWiFi();

 setup_rtc_ntp();

 if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64

   Serial.println(F("SSD1306 allocation failed"));

   for (;;);

 }

 delay(2000);
 
 display.clearDisplay();
 display.setTextSize(1);
 display.setTextColor(WHITE);

}

void loop() {
  
 lecturaBoton1 = digitalRead(BOTON1);
 lecturaBoton2 = digitalRead(BOTON2);

 if (gmtOffset_sec > 43200) {

   gmtOffset_sec = 43200;


 }

 if (gmtOffset_sec < -43200) {

   gmtOffset_sec = -43200;


 }

 if (gmt < -12) {
  gmt = -12;


 }
 if (gmt > 12) {
   gmt = 12;


 }
 
 switch (funcionamiento) {

   case 0:
     pedir_lahora();

     delay(500);
     display.clearDisplay();

     if (lecturaBoton1 == LOW && lecturaBoton2 == LOW ) {
      
       estadoBoton3 = HIGH;

     }
     
     if (lecturaBoton1 == HIGH && lecturaBoton2 == HIGH && estadoBoton3 == HIGH) {

       estadoBoton3 = LOW;

       funcionamiento = 1;


     }


     break;

   case 1:

     estadoPantalla = 1;

     display.clearDisplay();
     display.setCursor(0, 25);
     display.println("GMT:");
     display.print(gmt);
     display.display();

     if (lecturaBoton1 == LOW && lecturaBoton2 == LOW ) {

       estadoBoton3 = HIGH;

     }

     if (lecturaBoton1 == HIGH && lecturaBoton2 == HIGH && estadoBoton3 == HIGH) {

       estadoBoton3 = LOW;
       funcionamiento = 0;
       display.clearDisplay();
       break;

     }

     if (lecturaBoton1 == LOW && lecturaBoton2 == HIGH && estadoBoton3 == LOW) {

       estadoBoton1 = HIGH;


     }
     
     if (lecturaBoton1 == HIGH && estadoBoton1 == HIGH && estadoBoton3 == LOW) {

       gmtOffset_sec -= 3600;
       gmt = gmt - 1;
       setup_rtc_ntp();
       estadoBoton1 = LOW;

     }
     if (lecturaBoton2 == LOW && lecturaBoton1 == HIGH && estadoBoton3 == LOW) {
       estadoBoton2 = HIGH;
     }

     if (lecturaBoton2 == HIGH && estadoBoton2 == HIGH && estadoBoton3 == LOW) {
       gmtOffset_sec += 3600;
       gmt = gmt + 1;
       setup_rtc_ntp();

       estadoBoton2 = LOW;
     }
     break;

 }


}


void initWiFi() {


 WiFi.begin(ssid , password );


 Serial.print("Connecting to WiFi ..");


 while (WiFi.status() != WL_CONNECTED) {

   Serial.print('.');

   delay(1000);

 }

 Serial.println(WiFi.localIP());
 Serial.println();


}

//**********************************************************//

//* SETUP  rtc                                           **//

//**********************************************************//


void setup_rtc_ntp(void)


{

 // init and get the time
 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

 timestamp = time(NULL);

 rtc.setTime(timestamp + gmtOffset_sec);


}

//**********************************************************//


//* PIDE LA HORA AL SERVER O AL ESP32                     **//


//**********************************************************//


void pedir_lahora(void)


{


 minutos = timeinfo.tm_min;


 if (!getLocalTime(&timeinfo))


 {
   display.setCursor(0, 25);
   display.println("veo la hora del rtc interno ");
   timestamp = rtc.getEpoch() - gmtOffset_sec;
   timeinfo = rtc.getTimeStruct();
   display.println(&timeinfo, "%H:%M:%S");
 }

 else
 {
   display.setCursor(0, 25);
   display.print("NTP Time:");
   timestamp = time(NULL);
   display.println(&timeinfo, "%H:%M:%S");
 }


 display.display();
 return;


}
