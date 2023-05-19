#include <Wire.h>     // libreria para bus I2C
#include <Adafruit_GFX.h>   // libreria para pantallas graficas
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // crea objeto


#include "DHT.h"
#include <DHT.h>
#include <DHT_U.h>
float t;

#define DHTPIN 23     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

#define BTM 34 // minutos
#define BTH 35 // horas
int BTMlecture;
int BTHlecture;


int ms = 0;
int s = 0;
int m = 0;
int h = 0;




int state;
#define inicio 1
#define pantalla2 2


void setup() {
  Serial.begin(115200);
  Wire.begin();         // inicializa bus I2C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa pantalla con direccion 0x3C
  dht.begin();
  pinMode(BTM, INPUT_PULLUP);
  pinMode(BTH, INPUT_PULLUP);
  state = inicio;
}

void loop() {



  switch (state)
  {
    case inicio:
      delay(50);
      dhtFun();
      oledFunc();
      timer();
      lectureBtn();

      break;

    case pantalla2:

      delay(200);
      sumar();
      BTMlecture = digitalRead(BTM);
      BTHlecture = digitalRead(BTH);
      if ( BTHlecture == 0 && BTMlecture  == 0)
      {
        delay(200);
        Serial.print("prueba");
        BTMlecture = digitalRead(BTM);
        BTHlecture = digitalRead(BTH);

        state = inicio;


      }
      

      if (BTHlecture == 1 && BTMlecture  == 1)
      {
        Serial.print("PASO");
        delay(50);
        display.clearDisplay();      // limpia pantalla
        display.setCursor(0, 20);     // ubica cursor en inicio de coordenadas 0,0

        display.print("elige la hora: ");  // escribe en pantalla el texto
        display.setCursor(0, 45);
        display.print("Hora: ");
        display.print(h);
        display.print(":");
        display.print(m);
        display.print(":");
        display.print(s);
        display.display();
        BTMlecture = digitalRead(BTM);
        BTHlecture = digitalRead(BTH);







      }
      break;
  }
}

void dhtFun() {

  t = dht.readTemperature(); //Leemos la temperatura en grados Celsius
}
void oledFunc()
{


  display.clearDisplay();      // limpia pantalla
  display.setCursor(0, 20);     // ubica cursor en inicio de coordenadas 0,0
  display.setTextSize(1);
  display.print("Temp actual: ");  // escribe en pantalla el texto
  display.setTextColor(WHITE);
  display.setCursor(70, 20);     // ubica cursor en inicio de coordenadas 0,0
  display.setTextSize(1);
  display.print(t);  // escribe en pantalla el texto
  // display.display();     // muestra en pantalla todo lo establecido anteriormente


  display.setCursor(0, 45);
  display.print("Hora: ");
  display.print(h);
  display.print(":");
  display.print(m);
  display.print(":");
  display.print(s);
  display.display();
}



void timer()
{
  ms = ms + 1;
  delay(1);
  if (ms >= 30) {
    s++;
    ms = 0;
    // Serial.println(s);
  }
  if (s >= 60) {
    display.clearDisplay();
    m = m + 1;
    s = 0;
  }

  if (m >= 60) {
    display.clearDisplay();
    h = h + 1;
    m = 0;
  }

  if (h >= 24) {
    h = 0;
  }
}

void lectureBtn()
{
  BTMlecture = digitalRead(BTM);
  BTHlecture = digitalRead(BTH);


  Serial.print(BTMlecture);
  if ( BTHlecture == 0 && BTMlecture  == 0)
  {
    Serial.print("prueba");
    BTMlecture = digitalRead(BTM);
    BTHlecture = digitalRead(BTH);

    state = pantalla2;


  }
}

void sumar()
{
  if ( BTHlecture == 1 && BTMlecture  == 0)
  {
    // delay(10);
    h = h + 1;
  }

  if ( BTHlecture == 0 && BTMlecture  == 1)
  {
    //  delay(10);
    m = m + 1;
  }
  if (m >= 60) {
    h = h + 1;
    m = 0;
  }
  if (h >= 24) {
    h = 0;
  }



}
