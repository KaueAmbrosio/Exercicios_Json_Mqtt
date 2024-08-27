#include <Arduino.h>
#include <Bounce2.h>
#include "entradas.h"
#include "saidas.h"

// Definição dos pinos dos botões
#define BOTAO_BOOT_PIN 0

//bool estadoBotao = false;

// Criação dos objetos para debouncing
Bounce botao_boot = Bounce();

// Inicializa as entradas digitais
void inicializa_entradas()
{
    botao_boot.attach(BOTAO_BOOT_PIN, INPUT_PULLUP);
    //botao_boot.interval(50);
}

// Atualiza o estado dos botões
void atualiza_botoes()
{
    botao_boot.update();
}

// Retorna se o botão boot foi pressionado
bool botao_boot_pressionado()
{
    return botao_boot.fell();
}

/*
bool Estado_botao()
{
    if (botao_boot.fell())
    {
        estadoBotao = true;
        Serial.println("Botao pressionado");
    }
    else if (botao_boot.rose())
    {
        estadoBotao = false;
        Serial.println("Butao solto");
    }
    return estadoBotao;
}
*/

bool botao_boot_solto()
{
    return botao_boot.rose();
}
