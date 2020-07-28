// Creat by Giang Le 28/07/2020
/* Library: 

 https://pubsubclient.knolleary.net/
 https://github.com/knolleary/pubsubclient

*/


#include <WiFi.h>
#include <PubSubClient.h>
WiFiClient wifiClient;
PubSubClient client(wifiClient);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const char* ssid = "";
const char* password = "";
char deviceId[]  = "";
#define MQTT_SERVER ""
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_LED_TOPIC "/lamp/power"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int current_ledState = LOW;
int last_ledState = LOW;




void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connect_to_broker()
{
  while (!client.connected())
  {
      Serial.print("Attempting MQTT connection...");
      if (client.connect(deviceId, MQTT_USER, MQTT_PASSWORD))
      {
        Serial.println("connected");
        client.subscribe(MQTT_LED_TOPIC);
      }
      else 
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 2 seconds");
        delay(2000);
      }
  }
}

void callback(char* topic, byte *payload, unsigned int length) 
{
  Serial.println("-------new message from broker-----");
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.print("message: ");
  Serial.write(payload, length);
  Serial.println();
  if (*payload == '1') current_ledState = HIGH;
  if (*payload == '0') current_ledState = LOW;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT );
  client.setCallback(callback);
  connect_to_broker();
    pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, current_ledState);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() 
{
  client.loop();
  if (!client.connected())
  {
    connect_to_broker();
  }

  if (last_ledState != current_ledState)
  {
    last_ledState = current_ledState;
    digitalWrite(LED_BUILTIN, current_ledState);
    Serial.print("LED state is ");
    Serial.println(current_ledState);
  }


}

 
  
