//Autor: Fábio Henrique Cabrini
//Resumo: Esse programa possibilita ligar e desligar o led onboard, além de mandar o status para o Broker MQTT possibilitando o Helix saber
//se o led está ligado ou desligado.
//Revisões:
//Rev1: 26-08-2023 Código portado para o ESP32 e para realizar a leitura de luminosidade e publicar o valor em um tópico aprorpiado do broker
//Autor Rev1: Lucas Demetrius Augusto
//Rev2: 28-08-2023 Ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev2: Fábio Henrique Cabrini
//Rev3: 1-11-2023 Refinamento do código e ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev3: Fábio Henrique Cabrini

#include <WiFi.h>
#include <PubSubClient.h>
#include <vector>
// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = ""; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "bore.pub"; // IP do Broker MQTT
const int default_BROKER_PORT = 7502; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp001/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_SUBSCRIBE2 = "/TEF/lamp001/cmd/"; // Tópico MQTT de escuta // recebendo mudanca de cor
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp001/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp001/attrs/l"; // Tópico MQTT de envio de informações para Broker
const char* default_PICO_PUBLISH_3 = "/tef/lamp";
const char* default_ID_MQTT = "fiware_001"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard

const char* topicPrefix = "lamp001";
 
// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;
 
// mudancas feitas pelo felipe
//iniciando tratamento de portas do difusor, cada letra do rgb se comporta como um led independente, por isso 3 entradas
const int RED_PIN = 25;
const int GREEN_PIN = 26;
const int BLUE_PIN = 27;
 
WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0';

std::vector<String> nomesCores = {"branco", "preto", "vermelho", "azul", "verde", "amarelo"};
std::vector<String> hexaCores = {"#FFFFFF", "#000000", "#FF0000", "#0000FF", "#00FF00", "#FFFF00"};
void initSerial() {
    Serial.begin(115200);
}
 
void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}
 
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}
 
void setup() {
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
}
 
void loop() {
    VerificaConexoesWiFIEMQTT();
    EnviaEstadoOutputMQTT();
    handleLuminosity();
    MQTT.loop();
}
 
void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
 
    // Garantir que o LED inicie desligado
    digitalWrite(D4, LOW);
}
 
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg = "";// inicia variavel

    // converte payload(array de bytes) em string msg
    for (int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }
    // aprova de engracadinhos
    msg.trim();
    msg.toLowerCase();
    
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    if (msg.startsWith("add")) {
        adicionarNovaCor(msg);
        return; 
    }

    if (msg.indexOf('|') != -1) {
        msg = msg.substring(msg.indexOf('|') + 1);
    }


    if (msg == "on" || msg.indexOf("@on") != -1) { 
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
        Serial.println("LED LIGADO");
    } 
    else if (msg == "off" || msg.indexOf("@off") != -1) {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
        Serial.println("LED DESLIGADO");
    } 
    else if (msg.startsWith("#")) {
        setarUsandoHexa(msg);
    } 
    else if (msg.length() > 0) {
        setarUsandoNome(msg);
    }
}
 
void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}
 
void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- Led Ligado");
    }
 
    if (EstadoSaida == '0') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}
 
void InitOutput() {
    pinMode(D4, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    digitalWrite(D4, HIGH);
    boolean toggle = false;
 
    for (int i = 0; i <= 10; i++) {
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}
 
void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}
 
void handleLuminosity() {
    const int potPin = 34;
    int sensorValue = analogRead(potPin);
    int luminosity = map(sensorValue, 0, 4095, 0, 100);
    String mensagem = String(luminosity);
    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str());
    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}
 
// funcao que converte o hexadecimal em RGB
 
void setarCorPraHex(String hexColor){
  
  if (hexColor.startsWith("#")){
    hexColor.remove(0, 1);
  }
 
  // convertendo a string recebida em um numero inteiro que o esp consegue usar
  // esp normalmente tem 32 bits
  // exemplo: 0xFFFFFF = 16777215
  //strtol -> String To Long ou seja, texto pra numero
  //hexColor.c_str() --> conversão de tipo, temos String hexColor, mas o strtol() não aceita String, ele aceita chars (bizarro, mas descobri que string no C não é um array de Chars)
  // resumidamente, transformamos String "#FF0000" em "#FF0000" (formato C)
  // NULL indica onde a conversão deve parar, sem isso oq não é numero ficaria como "endptr", não queremos isso.
  // com o 16 estamos ditando a base numérica que queremos, Hex de 16, tamo quase lá
  //vai ficar mais ou menos assim:
  //  FF AA 33
  //  │  │  │
  //  R  G  B
  // IMPORTANTE --> O hexadecimal já é RGB compactado em 24 bits. Por isso separamos por >> (Bit Shifting)
  long number = strtol(hexColor.c_str(), NULL, 16);
 
  // feito isso, podemos só declarar:
  // 0xFF indica pro compilador que o valor tá em hexadecimal
  int r = number >> 16;
  int g = (number >> 8) & 0xFF;
  int b= number & 0xFF;
 
    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
 
    Serial.print("RGB -> ");
    Serial.print(r);
    Serial.print(",");
    Serial.print(g);
    Serial.print(",");
    Serial.println(b);
}

void setarUsandoNome(String msg) {
    msg.trim();
    msg.toLowerCase();

    // Com vector, usamos .size() para saber quantos itens existem
    int total = nomesCores.size(); 

    for (int i = 0; i < total; i++) {
        if (msg == nomesCores[i]) {
            Serial.print("Sucesso! Nome: "); Serial.print(nomesCores[i]);
            Serial.print(" -> Hex: "); Serial.println(hexaCores[i]);
            
            setarCorPraHex(hexaCores[i]);
            return; // Encontrou? Sai da função.
        }
    }
    Serial.println("Cor nao encontrada na lista.");
}

void setarUsandoHexa(String msg){
  setarCorPraHex(msg);
}
void adicionarNovaCor(String msg) {
  // Exemplo esperado: "add|verde|#00ff00"
  
  int primeiroPipe = msg.indexOf('|'); 
  int segundoPipe = msg.indexOf('|', primeiroPipe + 1);

  // Se encontrou os dois pipes
  if (primeiroPipe != -1 && segundoPipe != -1) {
    
    // Pega o que está entre o 1º e o 2º pipe
    String nomeCor = msg.substring(primeiroPipe + 1, segundoPipe); 
    
    // Pega tudo que está depois do 2º pipe
    String hexaCor = msg.substring(segundoPipe + 1);

    // REMOVE espaços, \n ou \r que estragam a comparação
    nomeCor.trim();
    hexaCor.trim();

    // seria o Append() do C++ em vetores
    nomesCores.push_back(nomeCor);
    hexaCores.push_back(hexaCor);

    Serial.println("--- NOVA COR CADASTRADA ---");
    Serial.print("Nome: ["); Serial.print(nomeCor); Serial.println("]");
    Serial.print("Hexa: ["); Serial.print(hexaCor); Serial.println("]");

    // teste de fogo
    setarUsandoNome(nomeCor); 
  } else {
    Serial.println("Erro! Formato aceito: comando|nome|hexa");
  }
}