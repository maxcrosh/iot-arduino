#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TroykaGPS.h>
#include <SoftwareSerial.h>
#include <DHT.h>

// SD Card
int chipSelect = 10;
File file;

// GPS
SoftwareSerial mySerial(7, 6);
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
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int hum;
int temp;
//____________________________________

// CO2
#define pwmPin 5
int prevVal = LOW;
long th, tl, h, l, ppm;
//___________________________________


void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(pwmPin, INPUT);
  pinMode(chipSelect, OUTPUT);

//  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
//    Serial.println(F("SSD1306 failed"));
//    for(;;);
//  } else {
//    Serial.println(F("SSD1306 succeed"));
//  }
//  delay(2000);

  if (!SD.begin(chipSelect)) {
    Serial.println(F("Could not initialize SD card."));
  } else {
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
  
  GPS_SERIAL.begin(9600);
  Serial.print(F("GPS init 9600"));
  delay(1000);
}

void loop() {
  
}
