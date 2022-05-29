//Librarys
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"
/*/#include "DHT.h"/*/

//files
#include "Icon.h"

int x = 0;

//DHT
const int DHT_PIN = 15;
DHTesp dhtSensor;

/*/
#define DHT11Pin 2
#define DHTType DHT22
/*/


//display define
#define SCREEN_WIDTH 128 // Sirina
#define SCREEN_HEIGHT 64 // Visina
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//////////////////////////////////////////////////////////////////////////////////////////////////////

//definiranje default spremenjlivk v DisplayCenter
void DisplayCenter(String text = "Test" ,int size = 2, String center = "C", String Symbol = "", int x = 0 , int y = 0);



//LED
int Z = 12;
int R = 13;
int M = 14;



//Spremenjlivke za prebrane vrednsoti
float humi;
float tempC;

//Spremenjlivke za povprecne vrednsoti
float humi_Av;
float tempC_Av;

int  i = 0;


String H = "";
String C = "";


char Terminal [50];


//Timer
unsigned long starttime = 0;
unsigned long endtime = 0;

TaskHandle_t DisplayCore;

void setup() {

  //definira na katerme procesorju se izvaja Task
  xTaskCreatePinnedToCore(DisplayCorecode,"Display",10000,NULL,1,&DisplayCore,0);                         
  delay(500); 



  Serial.begin(9600);
  
  //For DHT11
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  /*/
  HT.begin();
  /*/
  
  //LED 
  pinMode(Z, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(M, OUTPUT);

}


void loop() {

  humi_Av = 0;
  tempC_Av = 0;

  i = 0;

  starttime = millis();
  endtime = starttime;

  

  while ((endtime - starttime) <=5000) { // 5 selunden timer ki racuna povprecje temp/humi
    
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();

    humi = (data.humidity);
    tempC = (data.temperature);

    /*/
    humi = HT.readHumidity();
    tempC = HT.readTemperature();
    /*/
   
    i += 1;

    humi_Av += humi;
    tempC_Av += tempC;
    delay(100);

    endtime = millis();
  }



  humi_Av  /= i;
  tempC_Av /= i;

  H = String(humi_Av);
  C = String(tempC_Av);

  sprintf(Terminal,"vlaga: %s % Temperatura: %s C ",H,C);
  Serial.println(Terminal);


  //priziga LED-ice glede na temperaturo
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

void DisplayCorecode( void * parameter ){ // isto kot void setup samo da deluje na procesorju 0
  Serial.begin(9600);
  Serial.print("core ");
  Serial.println(xPortGetCoreID());
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  //narise Icono na zagono
  display.clearDisplay();
  display.setRotation(2);
  display.drawBitmap(0, 0, WatherIcon, 128, 64, WHITE);
  display.invertDisplay(true);
  display.display();

  delay(5000);
  display.invertDisplay(false);


  for(;;){    // isto kot void loop samo da deluje na procesorju 0  

      display.clearDisplay();
      DisplayCenter(C, 2, "C", "C", 0 , -10);
      DisplayCenter("Temp", 2, "CT");


      delay(2500);
      display.clearDisplay();
      DisplayCenter(H, 2, "C", "%", 0 , -10);
      DisplayCenter("Humidity", 2, "CT");
      delay(2500);

     
       
    }
  } 



//funkcija za centriranje teksta/stevilke in izpisevanje na display
void DisplayCenter(String text ,int size, String center, String Symbol, int x , int y) {
  //velikost stevilke/teksta
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;
  //velikost symbola
  int16_t Sx1;
  int16_t Sy1;
  uint16_t Swidth;
  uint16_t Sheight;

  display.setTextColor(WHITE);
  display.setTextSize(size);
  //zabelezi veliksot symbola
  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);
  display.getTextBounds(Symbol, 0, 0, &Sx1, &Sy1, &Swidth, &Sheight);


  
  if (Symbol == "C"){
    if (center == "C"){//Center
      display.setCursor((SCREEN_WIDTH - width - Swidth-5) / 2 +  x , (SCREEN_HEIGHT -  height) / 2 -y);
    }
    else if(center == "CT"){//Spodnji center
      display.setCursor((SCREEN_WIDTH - width - Swidth-5) / 2 +  x, (SCREEN_HEIGHT - height) / 128 - y);
    }
    else if(center == "CB"){//zgornji senter
      display.setCursor((SCREEN_WIDTH - width - Swidth-5) / 2 +  x, (SCREEN_HEIGHT - height) / 1 - y);
    }


    display.println(text);
    //izrise znak za stopinje odvisno od velikosti teksta
    display.drawCircle((SCREEN_WIDTH + width - Swidth - 5) / 2 + 2 + x , (SCREEN_HEIGHT -  height) / 2 +2 - y , size, WHITE);  // print degree symbols (  )
    display.setCursor((SCREEN_WIDTH + width - Swidth) / 2 + (2+size) + x,(SCREEN_HEIGHT - height) / 2 - y );

  }
  else {

     if (Symbol == ""){
      Swidth  -= Swidth;
    }

    if (center == "C"){//Center
      display.setCursor((SCREEN_WIDTH - width - Swidth ) /2 +  x , (SCREEN_HEIGHT -  height) / 2 -y);
    }
    else if(center == "CT"){//Spodnji center
      display.setCursor((SCREEN_WIDTH - width - Swidth ) /2 +  x, (SCREEN_HEIGHT - height) / 128 - y);
    }
    else if(center == "CB"){//zgornji senter
      display.setCursor((SCREEN_WIDTH - width - Swidth ) /2 +  x, (SCREEN_HEIGHT - height) / 1 - y);
    }

    display.print(text);

  }  

  display.print(Symbol);
  display.display();

}
  
  



