
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Пины датчика Co2
#define pwmPin 5

// Обработка сигнала Co2
int prevValCo2 = LOW;
long th, tl, h, l, ppm;

void setup() {
  Serial.begin(115200);
  pinMode(pwmPin, INPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.println("Hello");
  display.display();
  delay(2000);
}

void loop() {
  
  // Co2
  long tt = millis();
  int co2 = digitalRead(pwmPin);
  
  if (co2 == HIGH) {
   
    if (co2 != prevValCo2) {
      h = tt;
      tl = h - l;
      prevValCo2 = co2;
    }
  }  else {
   
    if (co2 != prevValCo2) {
      l = tt;
      th = l - h;
      prevValCo2 = co2;
      ppm = 5000 * (th - 2) / (th + tl - 4);
      Serial.println("PPM: " + String(ppm));
    }
  }

  delay(100);
}
