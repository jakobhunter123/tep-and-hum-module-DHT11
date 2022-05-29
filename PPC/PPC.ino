//Librarys
#include <Arduino.h>
#include "DHTesp.h"
/*/#include "DHT.h"/*/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//files
#include "Icon.h"



//DHT
const int DHT_PIN = 15;
DHTesp dhtSensor;

/*/
#define DHT11Pin 2
#define DHTType DHT22
/*/


//display define
#define SCREEN_WIDTH 128 // display display width, in pixels
#define SCREEN_HEIGHT 64 // display display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



//////////////////////////////////////////////////////////////////////////////////////////////////////


void DisplayCenter(String text = "Test" ,int size = 2, String center = "C", String Symbol = "", int x = 0 , int y = 0);



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


String H = "";
String C = "";


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

  //For display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  
  display.clearDisplay();
  display.setRotation(2);
  display.drawBitmap(0, 0, WatherIcon, 128, 64, WHITE);
  display.invertDisplay(true);
  display.setTextColor(WHITE);
  display.display();

  
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

  
  
  while ((endtime - starttime) <=1000) // do this loop for up to 1000mS
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

  display.invertDisplay(false);
  display.clearDisplay();

  H = String(humi_Av)/*/ + "%"/*/;
  C = String(tempC_Av)/*/ + "%"/*/;

  
  DisplayCenter(C, 2, "C", "C", 0 , -10);
  DisplayCenter("Temp", 2, "CT");


  delay(2500);
  display.clearDisplay();

  DisplayCenter(H, 2, "C", "%", 0 , -10);
  DisplayCenter("Humidity", 2, "CT");

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



  display.setTextSize(size);

  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);
  display.getTextBounds(Symbol, 0, 0, &Sx1, &Sy1, &Swidth, &Sheight);



  if (Symbol == "C"){
    if (center == "C"){//Center
      display.setCursor((SCREEN_WIDTH - width - Swidth-5) / 2 +  x , (SCREEN_HEIGHT -  height) / 2 -y);
      Serial.println("TestC0");
    }
    else if(center == "CT"){//Spodnji center
      display.setCursor((SCREEN_WIDTH - width - Swidth-5) / 2 +  x, (SCREEN_HEIGHT - height) / 128 - y);
    }
    else if(center == "CB"){//zgornji senter
      display.setCursor((SCREEN_WIDTH - width - Swidth-5) / 2 +  x, (SCREEN_HEIGHT - height) / 1 - y);
    }

    display.println(text);
    Serial.println("TestC1");
    display.drawCircle((SCREEN_WIDTH + width - Swidth - 5) / 2 + 2 + x , (SCREEN_HEIGHT -  height) / 2 +2 - y , size, WHITE);  // print degree symbols (  )
    display.setCursor((SCREEN_WIDTH + width - Swidth) / 2 + (2+size) + x,(SCREEN_HEIGHT - height) / 2 - y );

  }
  else {

     if (Symbol == ""){
          Serial.println(Swidth);
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
    Serial.println(Swidth);


  }  

  display.print(Symbol);
  display.display();

}
  
  
