#include "DHT.h"

#define DHTPIN 12
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


void setup() {
 Serial.begin(115200);
 dht.begin();

}

void loop() {
  float temperature = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);

  if(isnan(temperature)){
     Serial.println("Falha na captura");
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

    delay(3000);
}
