#include <ESP8266WiFi.h>
#include "DHT.h"
#include <PubSubClient.h>

#define DHTTYPE DHT11
#define waitTimeDHT 3000
#define waitTimeWI_FI 500

const char* BROKER_MQTT = "broker.mqtt-dashboard.com"; 
int BROKER_PORT = 1883;

#define ID_MQTT "Term_01"
#define TOPIC_PUBLISH "DeviceTempUmid_01"

#define DHTPIN 12
int statusCooler = 4; // (D2 = 4)

const byte qtdePinosDigitais = 5;
                          //Node MCU ((2=d4),          (5 = d1),  (13=D7), (14=D5 ), (15=D8))
byte pinosDigitais[qtdePinosDigitais] = {2,                5,      13,     14,        15  };
byte modoPinos[qtdePinosDigitais]     = {INPUT_PULLUP,  OUTPUT,  OUTPUT,  OUTPUT,   OUTPUT};

const byte qtdePinosAnalogicos = 1;
byte pinosAnalogicos[qtdePinosAnalogicos] = {A0};

float limitT = 90;

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient MQTT(espClient);
WiFiServer server(80);

void setup() {
    
  Serial.begin(115200); 
  
  IPAddress ip(192, 168, 15, 155);     
  IPAddress gateway(192, 168, 15, 1);
  IPAddress subnet(255, 255, 255, 0);   
  IPAddress dns(192, 168, 15, 1); 
  //WiFi.config(ip, dns, gateway, subnet); 
  
  WiFi.begin("AndroidAP6A7B", "zyvs3949"); 
  //WiFi.begin("VIVO-8965", "C9D3C88965");  
  dht.begin();
  
  server.begin();
  Serial.println("Server iniciado");
  
  void conectMQTT(); 
  void payload();
  pinMode(statusCooler, OUTPUT);
  
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
     for (int nP=0; nP < qtdePinosDigitais; nP++) {
        pinMode(pinosDigitais[nP], modoPinos[nP]);
    }     
}

void loop() {
  float temperature = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);
  float humidity = dht.readHumidity();

  WiFiClient  client = server.available();
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

    cooler(temperatureF, limitT);
/*
    if(temperatureF >= 90){      
       digitalWrite(statusCooler,HIGH);
                           
      }else{                                        
          digitalWrite(statusCooler,LOW);
    }  */  
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
void cooler(float t, float l){
      if(t >= limitT){      
       digitalWrite(statusCooler,HIGH);
                           
      }else{                                        
          digitalWrite(statusCooler,LOW);
    } 
  }
