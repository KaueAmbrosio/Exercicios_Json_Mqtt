#include <Arduino.h>
#include "token.h"
#include "tempo.h"

long randNumber;

unsigned long tempo_anterior_token = 0;
const unsigned long troca_Token = 10000;
const unsigned long tempo_extra = 30000;

void Inicializa_senha()
{
    randomSeed(timeStamp());
}

void gera_senha()
{
    if (millis() - tempo_anterior_token >= troca_Token)
    {
        tempo_anterior_token = millis();
        randNumber = random(1000, 10000);
        Serial.println(randNumber);
    }
}