#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT11.h>
DHT11 dht11(23);

const char* ssid = "Nome da rede";
const char* password = "Senha da rede";
const char* mqtt_server = "192.168.0.4";

WifiClient ESP32_DHT;
PubSubClient client(ESP32_DHT);

void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

      if (client.connect("ESP32_DHT")) {
      Serial.print("connected");
      } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(500);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(2, OUTPUT);
}

void LED_Conexao() {
  if (WiFi.status() == WL_CONNECTED)
    digitalWrite(2, HIGH);
  else
    digitalWrite(2, LOW);
}

void Clima_interno()
{
  float temperature = dht11.readTemperature();
  client.publish("DHT_TEMP",String(temperature).c_str());
  delay(1000);

  float humidity = dht11.readHumidity();
  client.publish("DHT_UMI",String(humidity).c_str());
  delay(1000);
}

void loop() {
  reconnect();
  LED_Conexao();
  Clima_interno();
}
 