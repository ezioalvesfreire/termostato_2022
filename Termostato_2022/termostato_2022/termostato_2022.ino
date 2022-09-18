#include <PubSubClient.h>
#include <ESP8266WiFi.h>

//Informações do WiFi
#define WIFISSID "VIVO-8965"
#define WIDIPASS "C9D3C88965"

//Informações do Servidor MQTT
const char* mqttserver = "broker.mqtt-dashboard.com";
int mqttserverport = 1883;
//const char* mqttuser = "usuario do servidor MQTT";
//const char* mqttpass = "senha do servidor MQTT";

//Variáveis
WiFiClient wifiClient;
PubSubClient ConexaoMQTT(wifiClient);
int UltimoValor = 0;
int TimeCounter = 0;

//Pinos
const int PINbotao = 5;

void reconnect() {
  ConexaoMQTT.setServer(mqttserver, mqttserverport);
  while (!ConexaoMQTT.connected()) {
    Serial.println("Conectando ao Broker MQTT");
   // ConexaoMQTT.connect("esp8266",mqttuser,mqttpass);
    delay(3000);
  }
  Serial.println("MQTT conectado");
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  IPAddress ip(192, 168, 15, 155);
  IPAddress gateway(192, 168, 15, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(192, 168, 15, 1);
  WiFi.config(ip, dns, gateway, subnet);
  WiFi.begin(WIFISSID, WIDIPASS);
  delay(5000);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado! IP address: ");
  Serial.println(WiFi.localIP());

  if (!ConexaoMQTT.connected()) {
    reconnect();
  }
  
  pinMode(PINbotao, INPUT); 

  //Configura Sensores através do MQTT Discovery do Home Assistant
  String novosensor = "{\"expire_after\": \"600\", \"icon\": \"mdi:gesture-tap-button\", \"name\": \"Interruptor 1 ESP8266\", \"state_topic\": \"esp8266/interruptor1/state\"}";
  ConexaoMQTT.publish("homeassistant/binary_sensor/esp8266/interruptor1/config",novosensor.c_str(),false);

  //Primeira leitura
  int LeituraSwitchD1Pin = digitalRead(PINbotao);
  Serial.print("Primeira Leitura: ");
  Serial.println(LeituraSwitchD1Pin);
  if (LeituraSwitchD1Pin == 0) {
    UltimoValor = LeituraSwitchD1Pin;
    ConexaoMQTT.publish("esp8266/interruptor1/state","OFF");
  } else if (LeituraSwitchD1Pin == 1) {
    UltimoValor = LeituraSwitchD1Pin;
    ConexaoMQTT.publish("esp8266/interruptor1/state","ON");
  }
}

void loop() {
  if (!ConexaoMQTT.connected()) {
    reconnect();
  }
  
  int LeituraSwitchD1Pin = digitalRead(PINbotao);
  Serial.println(LeituraSwitchD1Pin);
  if (LeituraSwitchD1Pin != UltimoValor) {
    if (LeituraSwitchD1Pin == 0) {
      UltimoValor = LeituraSwitchD1Pin;
      ConexaoMQTT.publish("esp8266/interruptor1/state","OFF");
    } else if (LeituraSwitchD1Pin == 1) {
      UltimoValor = LeituraSwitchD1Pin;
      ConexaoMQTT.publish("esp8266/interruptor1/state","ON");
    }
  }
  if (TimeCounter < 600) {
    TimeCounter++;
  } else if (TimeCounter >= 600) {
    if (LeituraSwitchD1Pin == 0) {
      UltimoValor = LeituraSwitchD1Pin;
      ConexaoMQTT.publish("esp8266/interruptor1/state","OFF");
    } else if (LeituraSwitchD1Pin == 1) {
      UltimoValor = LeituraSwitchD1Pin;
      ConexaoMQTT.publish("esp8266/interruptor1/state","ON");
    }
    TimeCounter = 0;
  }
  delay(500);
}
