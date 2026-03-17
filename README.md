# Smart Lamp IoT Control

> Projeto de controle de LED RGB via MQTT com ESP32, integrado ao FIWARE.

## Descrição

Sistema IoT para acionamento e controle de cor de um LED difuso RGB através de mensagens MQTT. O projeto faz parte de uma integração com a plataforma FIWARE, permitindo o monitoramento e controle remoto de dispositivos embarcados.

## Funcionalidades

- **Controle ON/OFF** - Liga e desliga o LED onboard do ESP32
- **Controle de Cor RGB** - Envie códigos hexadecimais (ex: `#FF0000`) ou nomes de cores em português (ex: `vermelho`, `azul`)
- **Leitura de Luminosidade** - Publica valores de luminosidade em tópico MQTT separado
- **Integração FIWARE** - Comunicacao via broker MQTT com a plataforma FIWARE

## Hardware

- ESP32
- LED RGB Diffuso (ânodo comum)
- Resistor 330Ω (para cada canal RGB)
- Potenciômetro (para sensor de luminosidade)

## Configuração

### Broker MQTT
```cpp
const char* BROKER_MQTT = "136.115.197.23"; // IP do Broker
const int BROKER_PORT = 1883;
```

### Tópicos MQTT
| Tópico | Função |
|--------|--------|
| `/TEF/lamp001/cmd` | Recebe comandos on/off |
| `/TEF/lamp001/cmd/#` | Recebe mudança de cor (hex) |
| `/TEF/lamp001/attrs` | Publica status do LED |
| `/TEF/lamp001/attrs/l` | Publica luminosidade |

## Controle de Cor

O sistema aceita comandos de cor de duas formas:

### Via Hexadecimal
Envie um código hexadecimal para alterar a cor:
```
#FF0000 - Vermelho
#00FF00 - Verde
#0000FF - Azul
#FFFFFF - Branco
#FFA500 - Laranja
```

### Via Nome da Cor
Envie o nome da cor em português (minúsculas ou maiúsculas):
| Nome | Cor |
|------|-----|
| branco | #FFFFFF |
| preto | #000000 |
| vermelho | #FF0000 |
| azul | #0000FF |
| verde | #00FF00 |
| amarelo | #FFFF00 |

### Formato MQTT
A mensagem pode ser enviada de duas formas:
- Apenas o valor: `vermelho` ou `#FF0000`
- Com pipe: `lamp001@cor|vermelho` (ignora tudo antes do pipe)

## Autores

- **Professor Fábio Henrique Cabrini** - Autor original do projeto
- **Felipe Menezes** - Adaptacao para LED RGB difuso e lógica de conversão hex para RGB

## Créditos

Baseado no projeto original do professor Fábio Henrique Cabrini (Revisões: Lucas Demetrius Augusto).

---

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![MQTT](https://img.shields.io/badge/MQTT-3F4E5F?style=for-the-badge&logo=mqtt&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)

</div>

## Simulação Wokwi

[![Wokwi](https://img.shields.io/badge/Simula%C3%A7%C3%A3o-Wokwi-FFFFFF?style=for-the-badge&logo=wokwi)](https://wokwi.com/projects/458334989531892737)

> Clique no badge acima ou [aqui](https://wokwi.com/projects/458334989531892737) para acessar a simulação do projeto.