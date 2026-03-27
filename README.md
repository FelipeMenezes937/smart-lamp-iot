# wokwiSimulator

Projeto de uma lâmpada inteligente controlada via MQTT, desenvolvida para ESP32 com simulação no Wokwi.
## Arquitetura

![Arquitetura do Projeto](img/arquiteture.jpeg)

## Funcionalidades

- **Controle via MQTT**: Ligue/desligue a lâmpada enviando comandos para o broker
- **LED RGB**: Mudança de cores usando códigos hexadecimais (ex: `#FF0000`) ou nomes de cores predefinidos
- **Cores predefinidas**: ligar, desligar, vermelho, azul, verde, amarelo
- **Cores dinâmicas**: Adicione novas cores em tempo real com o comando `add|nom_da_cor|hexa`
- **Sensor de luminosidade**: Lê valores de um fotoresistor (LDR) e publica no tópico MQTT
- **Status em tempo real**: Envia o estado atual (ligado/desligado) para o broker

## Hardware

- ESP32 DevKit v1
- LED RGB (cátodo comum)
- Fotoresistor (LDR)
- Resistores 220Ω

## Configuração

### Pinos ESP32

| Pino | Função |
|------|--------|
| D2   | LED onboard |
| D25  | LED RGB Vermelho |
| D26  | LED RGB Verde |
| D27  | LED RGB Azul |
| D34  | Sensor LDR (leitura analógica) |

### Tópicos MQTT

| Tópico | Função |
|--------|--------|
| `/TEF/lamp002/cmd` | Comandos recebidos (ligar/desligar, cores) |
| `/TEF/lamp002/attrs` | Status do LED |
| `/TEF/lamp002/attrs/l` | Leitura de luminosidade |
| `/tef/lamp` | Tópico adicional de publicação |

### Broker MQTT Padrão

- **Endereço**: `34.39.201.158`
- **Porta**: 1883

## Comandos

- `ligar` - Liga a lâmpada
- `desligar` - Desliga a lâmpada
- `#FFFFFF` - Define cor usando código hexadecimal
- `vermelho`, `azul`, `verde`, `amarelo` - Define cor pelo nome
- `add|roxo|#800080` - Adiciona nova cor dinamicamente

Ou utilize a PlatformIO IDE.

## Simulação

Este projeto pode ser simulado no [Wokwi](https://wokwi.com/projects/458761019384626177) utilizando os arquivos `diagram.json` e `wokwi.toml`.

## Autores

| Nome | Função |
|------|--------|
| Prof. Fábio Henrique Cabrini | Código base |
| Gabriel Ardito | Aluno |
| Felipe Menezes | Aluno |
| João Sarracine | Aluno |
| João Gonzales | Aluno |

