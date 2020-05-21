#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TroykaGPS.h>
#include <SoftwareSerial.h>
//#include <DHT.h>

// SD Card
int chipSelect = 4;
File file;

// GPS
SoftwareSerial mySerial(11, 10);
#define GPS_SERIAL mySerial
GPS gps(GPS_SERIAL);

#define MAX_SIZE_MASS 16
char latitudeBase60[MAX_SIZE_MASS];
char longitudeBase60[MAX_SIZE_MASS];
//____________________________________

// Display
Adafruit_SSD1306 display(4);
//____________________________________

// DHT22
//#define DHTPIN 2
//#define DHTTYPE DHT22
//DHT dht(DHTPIN, DHTTYPE);
//
//int chk;
//int hum;
//float temp;
//____________________________________

// CO2
#define pwmPin 5
int prevVal = LOW;
long th, tl, h, l, ppm;
//___________________________________


void setup(){
  Serial.begin(115200);
//  dht.begin();
  pinMode(pwmPin, INPUT);
  pinMode(chipSelect, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println(F("Could not initialize SD card."));
  }

  if (SD.exists("file.txt")) {
    Serial.println(F("File exists."));
    if (SD.remove("file.txt") == true) {
      Serial.println(F("Successfully removed file."));
    } else {
      Serial.println(F("Could not removed file."));
    }
  }
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 failed"));
    for(;;);
  } else {
    Serial.println(F("SSD1306 succeed"));
  }
  delay(2000);
  
  GPS_SERIAL.begin(9600);
  Serial.print(F("GPS init 9600"));
  delay(1000); 

}

void loop(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  file = SD.open("file.txt", FILE_WRITE);
  
//  hum = dht.readHumidity();
//  temp= dht.readTemperature();

  long tt = millis();
  int myVal = digitalRead(pwmPin);

  if (myVal == HIGH) {
    if (myVal != prevVal) {
      h = tt;
      tl = h - l;
      prevVal = myVal;
    }
  }  else {
    if (myVal != prevVal) {
      l = tt;
      th = l - h;
      prevVal = myVal;
      ppm = 5000 * (th - 2) / (th + tl - 4);
      
      display.println("ppm: " + String(ppm));
//      display.println("h: " + String(hum) + "%" + " t: " + String(temp) + " C");

      if (gps.available()) {
        gps.readParsing();
        switch(gps.getState()) {
          case GPS_OK:
            gps.getLatitudeBase60(latitudeBase60, MAX_SIZE_MASS);
            gps.getLongitudeBase60(longitudeBase60, MAX_SIZE_MASS);

            display.println(String(gps.getLatitudeBase10(), 6) + "; " + String(gps.getLongitudeBase10(), 6));  
            display.display();

            if (file) {
              file.println(String(ppm) + "," + String(gps.getLatitudeBase10(), 6) + ", " + String(gps.getLongitudeBase10(), 6));
            }
            break;
          
          case GPS_ERROR_DATA:
            display.println("lat:*; lng:*");
            display.display();
                    
            if (file) {
              file.println(String(ppm) + ",null,null");
            }
            break;
          case GPS_ERROR_SAT:
            display.println("lat:*; lng:*");
            display.display();
                    
            if (file) {
              file.println(String(ppm) + ",null,null");
            }
            break;
         }
      }
    }
  }

  file.close();
}
