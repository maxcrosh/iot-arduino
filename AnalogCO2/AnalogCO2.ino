#include <TroykaGPS.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display
Adafruit_SSD1306 display(4);

#define pwmPin 8

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int hum;
int temp;

int prevVal = LOW;
long th, tl, h, l, ppm;

int chipSelect = 10;
File file;

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 failed"));
    for(;;);
  } else {
    Serial.println(F("SSD1306 succeed"));
  }
  delay(2000);
  
  pinMode(pwmPin, INPUT);
  pinMode(chipSelect, OUTPUT);
  
  dht.begin();
   
  if (SD.begin(chipSelect))
    Serial.println(1);
  
  if (SD.exists("file.txt")) 
    SD.remove("file.txt");

  delay(2000);
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  
  long tt = millis();
  int val = digitalRead(pwmPin);
  
  file = SD.open("file.txt", FILE_WRITE);
    
  hum = dht.readHumidity();
  temp= dht.readTemperature();

  if (val == HIGH) {
    if (val != prevVal) {
      h = tt;
      tl = h - l;
      prevVal = val;
    }
  }  else {
    if (val != prevVal) {
      l = tt;
      th = l - h;
      prevVal = val;
      ppm = 5000 * (th - 2) / (th + tl - 4);

      display.println(hum);
      display.println(temp);
      display.println(ppm);
      display.display();
      Serial.println(String(hum) + "," + String(temp) + "," + String(ppm));
      file.println(String(hum) + "," + String(temp) + "," + String(ppm));
    }
  }
  file.close();
}
