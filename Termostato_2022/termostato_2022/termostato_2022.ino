#include <ESP8266WiFi.h>
#include "DHT.h"
#include <PubSubClient.h>

#define DHTPIN 12
#define DHTTYPE DHT11
#define waitTimeDHT 3000
#define waitTimeWI_FI 500

WiFiClient espClient;
PubSubClient MQTT(espClient);

const char* BROKER_MQTT = "broker.mqtt-dashboard.com"; 
int BROKER_PORT = 1883;

#define ID_MQTT "Term_01"
#define TOPIC_PUBLISH "DeviceTempUmid_01"


DHT dht(DHTPIN, DHTTYPE);

void setup() {
    
  Serial.begin(115200);  
  WiFi.begin("VIVO-8965", "C9D3C88965");  
  dht.begin();
  
  void conectMQTT(); 
  void payload();

  MQTT.setServer(BROKER_MQTT, BROKER_PORT);

 Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(waitTimeWI_FI);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

   if (!MQTT.connected()) {
       conectMQTT(); 
    }        
}

void loop() {
  float temperature = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);
  float humidity = dht.readHumidity();
  
  payload();
  MQTT.loop();

  if(isnan(temperature) || isnan(temperatureF)|| isnan(humidity)){
     Serial.println("Falha na captura sensor DHT!");
     delay(waitTimeDHT);
     return;
    }    
    
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.print("°C");
    Serial.println();
    
    Serial.print("Temperatura: ");
    Serial.print(temperatureF);
    Serial.print("°F");
    Serial.println();

    Serial.print("Umidade: ");
    Serial.print(humidity);
    Serial.print("%");
    Serial.println();

    delay(waitTimeDHT);
}

void conectMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
        } 
        else {
            Serial.println("Não foi possivel se conectar ao broker.");
            Serial.println("Nova tentativa de conexao em 10s");
            delay(10000);
        }
    }
}

void payload() {
//  MQTT.publish(TOPIC_PUBLISH, temperature);
    MQTT.publish(TOPIC_PUBLISH, "1");

}
