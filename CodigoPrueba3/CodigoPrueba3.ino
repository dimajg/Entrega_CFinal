#include "AsyncTaskLib.h"
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


const int potPin = 34;

// variable for storing the potentiometer value
int potValue = 0;
void leerluz (void);
void leertemperaturahumedad (void);
AsyncTask asyncTask1(1000, true, leerluz);
AsyncTask asyncTask2(2000, true, leertemperaturahumedad);

void setup()
{
   pinMode(LED_BUILTIN, OUTPUT);
   dht.begin();
    Serial.begin(115200);

  asyncTask1.Start();
}

void loop()
{
  asyncTask1.Update(asyncTask2);
  asyncTask2.Update(asyncTask1);
}

void leerluz (void) {
  potValue = analogRead(potPin);
  Serial.println(potValue);
}

void leertemperaturahumedad (void) {
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
}


