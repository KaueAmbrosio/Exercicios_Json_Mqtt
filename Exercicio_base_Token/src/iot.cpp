#include <WiFi.h>
#include <PubSubClient.h>
#include "tempo.h"
#include <ArduinoJson.h>
#include "iot.h"
#include "senhas.h"
#include "saidas.h"
#include "token.h"
#include <Arduino.h>

#define mqtt_topic1 "ProjetoKaue/receba"

const String cliente_id = "ESP32Client" + String(random(0xffff), HEX);

WiFiClient espClient;
PubSubClient client(espClient);

void tratar_msg(char *topic, String msg);
void callback(char *topic, byte *payload, unsigned int length);
void reconecta_mqtt();
void inscricao_topicos();

void setup_wifi()
{
    Serial.println();
    Serial.print("Conectando-se a Rede WiFi ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Conectado ao WiFi com sucesso com IP: ");
    Serial.println(WiFi.localIP());
}

void inicializa_mqtt()
{
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void atualiza_mqtt()
{
    client.loop();
    if (client.state() != MQTT_CONNECTED)
    {
        reconecta_mqtt();
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    // Serial.printf("Mensagem recebida [ %s ] \n\r", topic);
    String msg = "";
    for (int i = 0; i < length; i++)
    {
        // Serial.print((char)payload[i]);
        msg += (char)payload[i];
    }
    // Serial.println();
    tratar_msg(topic, msg);
}

void reconecta_mqtt()
{
    while (!client.connected())
    {
        Serial.print("Tentando se conectar ao Broker MQTT: ");
        Serial.println(mqtt_server);
        if (client.connect(cliente_id.c_str()))
        {
            Serial.println("Conectado ao Broker MQTT");
            inscricao_topicos();
        }
        else
        {
            Serial.println("Falha ao conectar ao Broker.");
            Serial.println("Havera nova tentativa de conexao em 2 segundos");
            delay(2000);
        }
    }
}

void publica_mqtt(String topico, String msg)
{
    client.publish(topico.c_str(), msg.c_str());
}

void inscricao_topicos()
{
    client.subscribe(mqtt_topic1); // LED 1
}

void tratar_msg(char *topic, String msg)
{
    if (strcmp(topic, mqtt_topic1) == 0)
    {
        int senha = randNumber;
        JsonDocument doc;
        deserializeJson(doc, msg);

        if (doc.containsKey("token"))
        {
            if (doc["token"] == senha)
            {
                if (doc.containsKey("LedState"))
                {
                    LedBuiltInState = doc["LedState"];
                }
            }
        }
        else
        {
            Serial.println("Nenhum token encontrado");
        }
    }
}
