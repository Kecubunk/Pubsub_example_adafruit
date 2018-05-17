#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ADAFRUIT_USERNAME            "mrkecubunk"
#define AIO_KEY                      "216495d375cc4d9fb3862cbb2ba8ec84"
#define FEED_PATH1                   "mrkecubunk/feeds/onoff"
#define AIO_CLIENT                   "WemosD1"
#define BUILTIN_LED  LED_BUILTIN

void callback (char* topic, byte* payload, unsigned int length) {
  Serial.print((char)payload[0]);
  if((char)payload[0] == '1' ){
    digitalWrite(BUILTIN_LED,LOW);
  }else{
     digitalWrite(BUILTIN_LED,HIGH); 
  }
  Serial.println("");
}


WiFiClient espClient;
PubSubClient mqttclient("io.adafruit.com", 1883, callback, espClient);

void reconnect() {


  mqttclient.connect(ADAFRUIT_USERNAME, AIO_KEY,"");

  while (!mqttclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    Serial.print(mqttclient.connected());
    if (mqttclient.connect(ADAFRUIT_USERNAME, AIO_KEY, "")) {
      Serial.println("MQTT Connected Adafruit");
      mqttclient.subscribe(FEED_PATH1);
      Serial.print("Suscrito a ");
      Serial.println(FEED_PATH1);      
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttclient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }

   
}

void setup(void)
{
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Connecting to Wifi");
  
  WiFi.begin("ImperiumII", "merdeka123");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  if (mqttclient.connect(AIO_CLIENT, ADAFRUIT_USERNAME, AIO_KEY)) {
     Serial.println(F("MQTT Connected"));
     mqttclient.subscribe(FEED_PATH1);
  }
  
}

void loop(void) {
  if (!mqttclient.connected()) {
    reconnect();
  }
  mqttclient.loop();
  delay(250);
}
