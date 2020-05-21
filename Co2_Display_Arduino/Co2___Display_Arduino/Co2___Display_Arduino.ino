#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

#define SCREEN_WIDTH 128 // Ширина дисплея в пикселях
#define SCREEN_HEIGHT 64 // Высота дисплея в пикселях

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Пины датчика Co2
#define pwmPin 5
#define LedPin 13

int prevVal = LOW;
long th, tl, h, l, ppm;

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup() {
  Serial.begin(115200);
  pinMode(pwmPin, INPUT);
  pinMode(LedPin, OUTPUT);

  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
}

void loop() {

  hum = dht.readHumidity();
  temp= dht.readTemperature();

  long tt = millis();
  int myVal = digitalRead(pwmPin);

  //Если обнаружили изменение
  if (myVal == HIGH) {
    digitalWrite(LedPin, HIGH);
    if (myVal != prevVal) {
      h = tt;
      tl = h - l;
      prevVal = myVal;
    }
  }  else {
    digitalWrite(LedPin, LOW);
    if (myVal != prevVal) {
      l = tt;
      th = l - h;
      prevVal = myVal;
      ppm = 5000 * (th - 2) / (th + tl - 4);

      
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      // Display static text
      display.println("PPM = " + String(ppm));
      display.println("Humidity = " + String(hum) + "%");
      display.println("Temp = " + String(temp) + " Celsius");
      display.display();
      display.clearDisplay();
    }
  }
}
