{
Serial.begin(9600);
Serial.println(F("DHTxx test!"));

dht.begin();

delay(1000);

Serial.println();
pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
delay(10);
digitalWrite(ledPin, HIGH); // initialize pin as off (active LOW)
// attempt to connect to Wifi network:
Serial.print("Connecting to Wifi SSID ");
Serial.print(WIFI_SSID);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}
Serial.print("\nWiFi connected. IP address: ");
Serial.println(WiFi.localIP());
Serial.print("Retrieving time: ");
configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
time_t now = time(nullptr);
while (now < 24 * 3600)
{
Serial.print(".");
delay(100);
now = time(nullptr);
}
Serial.println(now);
}
void loop()
{
potValue = analogRead(potPin);
Serial.println(potValue);
delay(500);

delay(2000);
   

// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
// Read temperature as Fahrenheit (isFahrenheit = true)
float f = dht.readTemperature(true);

// Check if any reads failed and exit early (to try again).
if (isnan(h) || isnan(t) || isnan(f)) {
Serial.println(F("Failed to read from DHT sensor!"));
return;
    
if (millis() - bot_lasttime > BOT_MTBS)
{
int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
while (numNewMessages)
{
Serial.println("got response");
handleNewMessages(numNewMessages);
numNewMessages = bot.getUpdates(bot.last_message_received + 1);
}
bot_lasttime = millis();
}
}

float hif = dht.computeHeatIndex(f, h);
// Compute heat index in Celsius (isFahreheit = false)
float hic = dht.computeHeatIndex(t, h, false);

Serial.print(F("Humidity: "));
Serial.print(h);
Serial.print(F("%  Temperature: "));
Serial.print(t);
Serial.print(F("°C "));
Serial.print(f);
Serial.print(F("°F  Heat index: "));
Serial.print(hic);
Serial.print(F("°C "));
Serial.print(hif);
Serial.println(F("°F"));

}
