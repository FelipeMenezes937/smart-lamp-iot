# Smart Lamp IoT

Projeto de uma lâmpada inteligente controlada via MQTT, desenvolvida para ESP32 com simulação no Wokwi.

## Funcionalidades

- **Controle via MQTT**: Ligue/desligue a lâmpada enviando comandos para o broker
- **LED RGB**: Mudança de cores usando códigos hexadecimais (ex: `#FF0000`) ou nomes de cores predefinidos
- **Cores predefinidas**: branco, preto, vermelho, azul, verde, amarelo
- **Cores dinâmicas**: Adicione novas cores em tempo real com o comando `add|nome|hexa`
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
| `/TEF/lamp001/cmd` | Comandos recebidos (on/off, cores) |
| `/TEF/lamp001/attrs` | Status do LED (s\|on / s\|off) |
| `/TEF/lamp001/attrs/l` | Leitura de luminosidade |

### Broker MQTT Padrão

- **Endereço**: `bore.pub`
- **Porta**: 7502

## Comandos

- `on` - Liga a lâmpada
- `off` - Desliga a lâmpada
- `#FFFFFF` - Define cor usando código hexadecimal
- `branco`, `vermelho`, `azul`, etc. - Define cor pelo nome
- `add|roxo|#800080` - Adiciona nova cor dinamicamente

## Compilação

```bash
./compilar.sh
```

Ou utilize a PlatformIO IDE.

## Simulação

Este projeto pode ser simulado no [Wokwi](https://wokwi.com) utilizando os arquivos `diagram.json` e `wokwi.toml`.
