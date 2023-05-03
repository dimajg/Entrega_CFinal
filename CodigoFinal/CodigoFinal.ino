#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include <DHT.h>

const char* ssid = "Galaxy A21s8396"; // conectarase a wifi 
const char* password = "fvoj0760";
#define BOTtoken "6241917181:AAFy8lbZGsEpGiwiH0j6eBgDytocAAQ4_9g"  // conectarse al Bot 
#define CHAT_ID "6280159573" //limitar la comunicacion con el tel 
#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

#define DHTPIN 13 //pin al que esta conectado el DTH1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 
LiquidCrystal lcd(22,23,5,18,19,21); //indica los pines de conexion

int hum = 0;; // variables que se encarga  de guaradar la informacion
int tem = 0;
int suelo = 0;
int luz=0;

int ledr=27; // leds y pines de salida
int ledv=12;
int leda=14;

unsigned long tim=0,timf=0; // actualizacion del tiempo a 1s
int motBomba=15;
int mot_Ven=4;
int bombillo=2;

bool ban_ven=false; // false=automatico 
bool ban_bom=false;
bool ban_led=false;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){ //comprueba el mensaje 
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/state") {
      String welcome = "Bienvenido, " + from_name + ".\n";
      welcome += "Escoja Opcion valida\n\n";
      welcome += "/0. Mostrar Informacion Sensores \n";
      welcome += "/1. Mostrar Informacion Implementos \n";
      welcome += "/2. Mostrar Informacion de Humedad \n";
      welcome += "/3. Mostrar Informacion de Temperatura \n";
      welcome += "/4. Mostrar Informacion de Suelo \n";
      welcome += "/5. Mostrar Informacion de Luz \n";
      welcome += "/6. ON/OFF ventilador de Temperatura \n";
      welcome += "/7. ON/OFF Bomba de Suelo \n";
      welcome += "/8. ON/OFF Led de Luz \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/0") {//Informacion Sensores
       String welcome = "Informacion Sensores, " + from_name + ".\n";
      welcome += "1. Humedad: "+String(hum)+"% \n";
      welcome += "2. Temperatura: "+String(tem)+"°C \n";
      welcome += "3. Suelo: "+String(suelo)+"% \n";
      welcome += "4. Luz: "+String(luz)+"% \n";
      bot.sendMessage(chat_id, welcome, "");
    }
     if (text == "/1") {//Informacion Sensores
       String welcome = "Informacion Implementos, 0= OFF, 1=ON " + from_name + ".\n";
      welcome += "1. Ventilador: "+String(ban_ven)+"\n";
      welcome += "3. Bomba: "+String(ban_bom)+" \n";
      welcome += "4. Luz Artificial: "+String(ban_led)+" \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/2") {//Informacion de Humedad
      bot.sendMessage(chat_id, "1. Humedad: "+String(hum)+"%", "");
    }
    
    if (text == "/3") {//Informacion de Temperatura
      bot.sendMessage(chat_id, "2. Temperatura: "+String(tem)+"°C", "");
    }
    
    if (text == "/4") {//Informacion de Suelo
      bot.sendMessage(chat_id, "Suelo: "+String(suelo)+"%", "");
    }
    if (text == "/5") {//Informacion de Luz
      bot.sendMessage(chat_id, "4. Luz: "+String(luz)+"%", "");
    }
    
    if (text == "/6") {//ventilador de tem
      if(ban_ven==false){
        bot.sendMessage(chat_id, "Ventilador Encendido", "");
        ban_ven=true;
        digitalWrite(mot_Ven,HIGH);
      }else{
        bot.sendMessage(chat_id, "Ventilador Apagado", "");
        ban_ven=false;
        digitalWrite(mot_Ven,LOW);
      }
    }
    
    if (text == "/7") {//bomba de hum
      if(ban_bom==false){
        bot.sendMessage(chat_id, "Bomba Encendida", "");
        ban_bom=true;
        digitalWrite(motBomba,HIGH);
      }else{
        bot.sendMessage(chat_id, "Bomba Apagada", "");
        ban_bom=false;
        digitalWrite(motBomba,LOW);
      }
    }
    if (text == "/8") {//led de luz
      if(ban_led==false){
        bot.sendMessage(chat_id, "Luz Artificial Encendida", "");
        ban_led=true;
        digitalWrite(bombillo,HIGH);
      }else{
        bot.sendMessage(chat_id, "Luz Artificial Apagada", "");
        ban_led=false;
        digitalWrite(bombillo,LOW);
      }
    }
  }
}

void setup() {
 inizializar();
}

void loop() {
  timf=millis();
  if (timf-tim>1000)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lcd.clear();
    temperatura();
    hum_suelo();
    luminidad();
    tim=timf; 
  }
}
void luminidad(){
    luz=analogRead(39);
    luz=map(luz,0,4096,0,100);
    imprimirL(8,1,"Luz:"); 
    imprimir(12,1,luz);
    if(luz<10){
        digitalWrite(ledv,HIGH);
    }else{
        digitalWrite(ledv,LOW);
    }
    if(ban_led==false){
      if(luz<10){
        digitalWrite(bombillo,HIGH);
      }else{
        digitalWrite(bombillo,LOW);
      }
    }
}
void hum_suelo(){
    suelo=analogRead(33);
    suelo=map(suelo,0,3500,0,120);
    imprimirL(0,1,"Sul:");
    imprimir(4,1,suelo);
    if(suelo>=100){
        digitalWrite(leda,HIGH);
      }else{
        digitalWrite(leda,LOW);
      }
    if(ban_bom==false){
      if(suelo>=100){
        digitalWrite(leda,HIGH);
        digitalWrite(motBomba,HIGH);
      }else{
        digitalWrite(leda,LOW);
        digitalWrite(motBomba,LOW);
      }
    }
}
void temperatura(){
    hum = dht.readHumidity(); // lee humedad
    tem = dht.readTemperature(); // lee temperatura
    imprimirL(0,0,"Hum:");
    imprimir(4,0,hum);
    imprimirL(8,0,"Tem:");
    imprimir(12,0,tem);
    if(tem>=20){
        digitalWrite(ledr,HIGH);
      }else{
        digitalWrite(ledr,LOW);   
      }
    if(ban_ven==false){
      if(tem>=20){
        digitalWrite(mot_Ven,HIGH);
      }else{
        digitalWrite(mot_Ven,LOW);      
      }
    }
}
void inizializar(){
  Serial.begin(115200);
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi.."); // se matiene en while mientra no se pueda conectar a una red
  }
  lcd.begin(16, 2);
  pinMode(ledr,OUTPUT);
  pinMode(ledv,OUTPUT);
  pinMode(leda,OUTPUT);
  pinMode(33,INPUT);
  pinMode(39,INPUT);
  pinMode(motBomba,OUTPUT);
  pinMode(mot_Ven,OUTPUT);
  pinMode(bombillo,OUTPUT);
  digitalWrite(motBomba,LOW);
  digitalWrite(mot_Ven,LOW);;
  dht.begin();
  lcd.setCursor(0,0);
  lcd.print("BIENVENIDO");
  hum = dht.readHumidity();
  tem = dht.readTemperature();
  Serial.println(WiFi.localIP());
  delay(1000);
  lcd.clear();
}
void imprimir(int a,int b,int c){
  lcd.setCursor(a, b);
  lcd.print(c);
}
void imprimirL(int a,int b,String c){
  lcd.setCursor(a, b);
  lcd.print(c);
}

