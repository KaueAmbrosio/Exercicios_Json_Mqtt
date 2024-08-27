#include <Arduino.h>
#include <ArduinoJson.h>
#include "iot.h"
#include "saidas.h"
#include "entradas.h"
#include "tempo.h"
#include "token.h"
#include <TimeLib.h>

// setTime(epochTime);
// Serial.printf("%02d/%02d/%04d", day(), month(), year());
// Serial.printf(" %02d:%02d:%02d \n", hour(), minute(), second());

#define mqtt_pub_topic1 "projetoKaue/publish"

// void acao_botao_boot();

unsigned long tempo_anterior = 0;
const unsigned long intervalo = 5000;

void setup()
{
    Serial.begin(115200);
    setup_wifi();
    setup_time();
    inicializa_entradas();
    inicializa_saidas();
    inicializa_mqtt();
    atualiza_time();
    Inicializa_senha();
}

void loop()
{
    atualiza_time();
    atualiza_saidas();
    atualiza_botoes();
    atualiza_mqtt();
    gera_senha();
    // acao_botao_boot();
    // Estado_botao();

    String json;
    JsonDocument doc;
    bool mensagemEmFila = false;
    if (millis() - tempo_anterior >= intervalo)
    {
        tempo_anterior = millis();
        doc["TimesTamp"] = timeStamp();
        mensagemEmFila = true;

        // doc["botao"] = estadoBotao;
        // doc["Led_Interna"] = LedBuiltInState;
        // Serial.print("Publiclando: ");
        // Serial.println(json);
    }

    if (botao_boot_pressionado())
    {
        LedBuiltInState = !LedBuiltInState;
        doc["LedState"] = LedBuiltInState;
        doc["BotaoState"] = true;
        doc["TimesTamp"] = timeStamp();
        mensagemEmFila = true;
    }
    else if (botao_boot_solto())
    {
        doc["BotaoState"] = false;
        doc["TimesTamp"] = timeStamp();
        mensagemEmFila = true;
    }
    if (mensagemEmFila)
    {
        serializeJson(doc, json);
        publica_mqtt(mqtt_pub_topic1, json);
        mensagemEmFila = false;
    }
}

// void acao_botao_boot()
//{
// }