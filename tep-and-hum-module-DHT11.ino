//Librarys
#include <Arduino.h>
#include "DHTesp.h"
/*/#include "DHT.h"/*/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



//DHT
const int DHT_PIN = 15;
DHTesp dhtSensor;

/*/
#define DHT11Pin 2
#define DHTType DHT22
/*/


//OLED define
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



//////////////////////////////////////////////////////////////////////////////////////////////////////



//LED
int Z = 12;
int R = 13;
int M = 14;



//Spremenjlivke za prebrane vrednsoti
float humi;
float tempC;
float tempF;

//Spremenjlivke za povprecne vrednsoti
float humi_Av;
float tempC_Av;
float tempF_Av;

int  i = 0;



//Timer
unsigned long starttime = 0;
unsigned long endtime = 0;


void setup() {


  Serial.begin(9600);
  
  //For DHT11
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  /*/
  HT.begin();
  /*/

  //For OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setRotation(2);

  
  //LED 
  pinMode(Z, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(M, OUTPUT);

}


void loop() {

  humi_Av = 0;
  tempC_Av = 0;
  tempF_Av = 0;

  i = 0;

  starttime = millis();
  endtime = starttime;

  while ((endtime - starttime) <=5000) // do this loop for up to 1000mS
  {
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();


    humi = (data.humidity);
    tempC = (data.temperature);
    tempF = (data.temperature);

    /*/
    humi = HT.readHumidity();
    tempC = HT.readTemperature();
    tempF = HT.readTemperature(true);
    /*/
   

    i += 1;

    humi_Av += humi;
    tempC_Av += tempC;
    tempF_Av += tempF;
    delay(100);


    endtime = millis();
  }


  humi_Av  /= i;
  tempC_Av /= i;
  tempF_Av /= i;



  Serial.print("Vlaga:");
  Serial.print(humi_Av,1);
  Serial.print("%");
  Serial.print(" Temperatura:");
  Serial.print(tempC_Av,1);
  Serial.print("C ~ ");
  Serial.print(tempF_Av,1);
  Serial.println("F");







  display.clearDisplay();
  oledDisplayHeader();


  display.display(); 

  if (tempC_Av >= 30){
    digitalWrite(R,HIGH);
  }
  if (tempC_Av <= 10){
    digitalWrite (M,HIGH);
  }
  if (tempC_Av >= 10 && tempC_Av <= 30){
    digitalWrite (Z,HIGH);
  }

}



void oledDisplayHeader(){
 display.clearDisplay();
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.setCursor(42, 0);
 display.print("Temp");
 display.setCursor(15,35);
 display.setTextSize(3);
 display.print(String(tempC_Av)+String("C"));
 display.display();

 delay(1000);

 display.clearDisplay();
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.setCursor(30, 0);
 display.print("Humidety");
 display.setCursor(15,35);
 display.setTextSize(3);
 display.print(String(humi_Av)+String("%"));
 display.display();

}





