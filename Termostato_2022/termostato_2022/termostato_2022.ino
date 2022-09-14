#include <ESP8266WiFi.h>
#include "DHT.h"


#define DHTPIN 12
#define DHTTYPE DHT11
#define waitTimeDHT 3000
#define waitTimeWI_FI 500

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(115200);  
  WiFi.begin("VIVO-8965", "C9D3C88965");  
  dht.begin();



 Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(waitTimeWI_FI);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temperature = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);
  float humidity = dht.readHumidity();

  if(isnan(temperature) || isnan(temperatureF)|| isnan(humidity)){
     Serial.println("Falha na captura sensor DHT!");
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
