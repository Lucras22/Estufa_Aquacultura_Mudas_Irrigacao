/*
██╗     ██╗   ██╗ ██████╗ ██████╗  █████╗ ███████╗
██║     ██║   ██║██╔═════╗██╔══██╗██╔══██╗██╔════╝
██║     ██║   ██║██║     ║██████╔╝███████║███████╗
██║     ██║   ██║██║     ║██╔══██╗██╔══██║╚════██║
███████╗╚██████╔╝╚██████╔╝██║  ██║██║  ██║███████║
╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝
-----------------------------------------------------------
📌 Autor: Lucas Galindo
🔗 GitHub: https://github.com/Lucras22
📧 E-mail: devlucasgalindo@email.com
💼 LinkedIn: https://www.linkedin.com/in/lucasgalindoiot/
-----------------------------------------------------------
📜 Instruções de Uso:
- 
-----------------------------------------------------------
📂 Repositório do Projeto:
🔗 https://github.com/Lucras22/Estufa_Aquacultura_Mudas_Irrigacao
-----------------------------------------------------------
🛠️ Licença: ....
*/

// ##############  BIBLIOTECAS
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>

// ##############  CONFIGURAÇÃO DO WIFI
const char* ssid = "Lucas Galindo | POCO C65";       // Substitua pelo seu Wi-Fi
const char* password = "lucras22";  // Substitua pela senha

// ##############  CONFIGURAÇÃO DO TELEGRAM
const String botToken = "7751526303:AAEjh5i6E2B0uGwTbU3TGWbjhbvcdTEvFdg";  // Substitua pelo token do seu bot
const String chatId = "7003158288";     // Substitua pelo seu chat ID

WiFiClientSecure client;

// Função para enviar mensagem para o Telegram via POST
void sendMessage(String message) {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 10) { // Tenta conectar por no máximo 10 segundos
    delay(1000);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Montando os parâmetros da requisição POST
    String postData = "chat_id=" + chatId + "&text=" + message;
    
    // Enviando a requisição
    int httpResponseCode = http.POST(postData);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Código de resposta: " + String(httpResponseCode));
      Serial.println("Resposta: " + response);
    } else {
      Serial.print("Erro na requisição: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("\nFalha ao conectar ao Wi-Fi.");
  }

  WiFi.disconnect(true); // Desconecta e desliga o Wi-Fi
  Serial.println("Wi-Fi desligado!");
}

// ##############  PINOS
#define DHT1PIN 12  //Sensor DHT Interno

#define tdsPin1 33  //Sensor TDS 1
#define tdsPin2 32  //Sensor TDS 2
#define tdsPin3 34  //Sensor TDS 3
#define tdsPin4 35  //Sensor TDS 4

#define ONE_WIRE_BUS 4 //Sensor Temp Agua

#define PH_SENSOR_PIN1 25  // Sensor pH 1
#define PH_SENSOR_PIN2 26  // Sensor pH 2
#define PH_SENSOR_PIN3 27  // Sensor pH 3
#define PH_SENSOR_PIN4 14  // Sensor pH 4

// ##############  DHT GLOBAL
// Definindo o tipo de sensor DHT
#define DHTTYPE DHT22
// Iniciando o sensor DHT
DHT dht1(DHT1PIN, DHTTYPE);

// ##############  TEMP AGUA GLOBAL
// Criando uma instância da classe OneWire
OneWire oneWire(ONE_WIRE_BUS);
// Passando a instância OneWire para a classe DallasTemperature
DallasTemperature sensors(&oneWire);

// ##############  SENSOR PH AGUA

// Definições de calibração para as duas faixas
float calibracao_ph7 = 1.65;  // Tensão obtida na solução de calibração pH 7 (ajuste conforme a leitura real)
float calibracao_ph4 = 1.35;  // Tensão obtida na solução de calibração pH 4 (ajuste conforme a leitura real)
float calibracao_ph10 = 2.05; // Tensão obtida na solução de calibração pH 10 (ajuste conforme a leitura real)

float m_4_7, b_4_7;  // Variáveis para a equação da calibração pH 4 a 7
float m_7_10, b_7_10; // Variáveis para a equação da calibração pH 7 a 10

// ##############  

void setup() {
  Serial.begin(115200);

  pinMode(tdsPin1, INPUT);
  pinMode(tdsPin2, INPUT);
  pinMode(tdsPin3, INPUT);
  pinMode(tdsPin4, INPUT);

  dht1.begin();
  sensors.begin();

  // Calibração para pH 7 e 4 (faixa ácida)
  m_4_7 = (4.0 - 7.0) / (calibracao_ph4 - calibracao_ph7);
  b_4_7 = 7.0 - m_4_7 * calibracao_ph7;

  // Calibração para pH 7 e 10 (faixa básica)
  m_7_10 = (7.0 - 10.0) / (calibracao_ph7 - calibracao_ph10);
  b_7_10 = 10.0 - m_7_10 * calibracao_ph10;

  WiFi.mode(WIFI_OFF); // Garante que o Wi-Fi inicia desligado
}

void loop() {

// ##############  SENSOR PH 1
  float ph1 = lerPH(PH_SENSOR_PIN1);
// ##############  SENSOR PH 2
  float ph2 = lerPH(PH_SENSOR_PIN2);
// ##############  SENSOR PH 3
  float ph3 = lerPH(PH_SENSOR_PIN3);
// ##############  SENSOR PH 4
  float ph4 = lerPH(PH_SENSOR_PIN4);

// ##############  DHT22
  // Lendo a umidade relativa do ar do DHT1
  float humidity1 = dht1.readHumidity();

  // Lendo a temperatura em Celsius do DHT1
  float temperature1 = dht1.readTemperature();
  
  // Verifique se houve falha na leitura dos sensores
  if (isnan(humidity1) || isnan(temperature1)) {
    Serial.println("Falha ao ler do sensor DHT1!");
    return;
  }

// ##############  TDS 1
  int tdsValue1 = analogRead(tdsPin1);
  // ##############  TDS 2
  int tdsValue2 = analogRead(tdsPin2);
  // ##############  TDS 3
  int tdsValue3 = analogRead(tdsPin3);
  // ##############  TDS 4
  int tdsValue4 = analogRead(tdsPin4);

  // Converte o valor do sensor em milivolts (mV)
  float condutivy1 = tdsValue1 * 2;
  float condutivy2 = tdsValue2 * 2;
  float condutivy3 = tdsValue3 * 2;
  float condutivy4 = tdsValue4 * 2;

// ##############  TEMPERATURA DA AGUA
  sensors.requestTemperatures(); // Solicita a leitura de temperatura
  float temperatureWalter1 = sensors.getTempCByIndex(0); // Obtém a temperatura do primeiro sensor
  float temperatureWalter2 = sensors.getTempCByIndex(1); // Obtém a temperatura do segundo sensor
  float temperatureWalter3 = sensors.getTempCByIndex(2); // Obtém a temperatura do terceiro sensor
  float temperatureWalter4 = sensors.getTempCByIndex(3); // Obtém a temperatura do quarto sensor

// ##############  EXIBIR MONITOR SERIAL

  Serial.print("Umidade Interna: ");
  Serial.print(humidity1);
  Serial.print(" %\t");
  Serial.print("Temperatura Interna: ");
  Serial.print(temperature1);
  Serial.println(" °C");

  Serial.println("-----Tanque 1-----");

  Serial.print("Temperatura Agua 1: ");
  Serial.print(temperatureWalter1);
  Serial.println(" °C");
  Serial.print("TDS 1 Valor (PPM): ");
  Serial.println(tdsValue1);
  Serial.print("Condutividade Eletrica 1 (ECC): ");
  Serial.println(condutivy1);
  Serial.print("Tensão medida pH 1: ");
  Serial.print(ph1, 3);
  Serial.print(" V | pH 1: ");
  Serial.println(ph1, 2);
  
  Serial.println("-----Tanque 2-----");

  Serial.print("Temperatura Agua 2: ");
  Serial.print(temperatureWalter2);
  Serial.println(" °C");
  Serial.print("TDS 2 Valor (PPM): ");
  Serial.println(tdsValue2);
  Serial.print("Condutividade Eletrica 2 (ECC): ");
  Serial.println(condutivy2);
  Serial.print("Tensão medida pH 2: ");
  Serial.print(ph2, 3);
  Serial.print(" V | pH 2: ");
  Serial.println(ph2, 2);

  Serial.println("-----Tanque 3-----");

  Serial.print("Temperatura Agua 3: ");
  Serial.print(temperatureWalter3);
  Serial.println(" °C");
  Serial.print("TDS 3 Valor (PPM): ");
  Serial.println(tdsValue3);
  Serial.print("Condutividade Eletrica 3 (ECC): ");
  Serial.println(condutivy3);
  Serial.print("Tensão medida pH 3: ");
  Serial.print(ph3, 3);
  Serial.print(" V | pH 3: ");
  Serial.println(ph3, 2);

  Serial.println("-----Tanque 4-----");

  Serial.print("Temperatura Agua 4: ");
  Serial.print(temperatureWalter4);
  Serial.println(" °C");
  Serial.print("TDS 4 Valor (PPM): ");
  Serial.println(tdsValue4);
  Serial.print("Condutividade Eletrica 4 (ECC): ");
  Serial.println(condutivy4);
  Serial.print("Tensão medida pH 4: ");
  Serial.print(ph4, 3);
  Serial.print(" V | pH 4: ");
  Serial.println(ph4, 2);

  // Enviar dados para o Telegram
  String mensagem = "-----Dados do Ar-----\n";
  mensagem +="Umidade: " + String(humidity1) + " %\t\n";
  mensagem += "Temperatura: " + String(temperature1) + " °C\n\n";

  mensagem += "-----Tanque 1-----\n";
  mensagem += "Temperatura Agua 1: " + String(temperatureWalter1) + " °C\n";
  mensagem += "TDS 1 Valor (PPM): " + String(tdsValue1) + "\n";
  mensagem += "Condutividade Eletrica 1 (ECC): " + String(condutivy1) + "\n";
  mensagem += "Tensão medida pH 1: " + String(ph1, 3) + " V | pH 1: " + String(ph1, 2) + "\n\n";

  mensagem += "-----Tanque 2-----\n";
  mensagem += "Temperatura Agua 2: " + String(temperatureWalter2) + " °C\n";
  mensagem += "TDS 2 Valor (PPM): " + String(tdsValue2) + "\n";
  mensagem += "Condutividade Eletrica 2 (ECC): " + String(condutivy2) + "\n";
  mensagem += "Tensão medida pH 2: " + String(ph2, 3) + " V | pH 2: " + String(ph2, 2) + "\n\n";

  mensagem += "-----Tanque 3-----\n";
  mensagem += "Temperatura Agua 3: " + String(temperatureWalter3) + " °C\n";
  mensagem += "TDS 3 Valor (PPM): " + String(tdsValue3) + "\n";
  mensagem += "Condutividade Eletrica 3 (ECC): " + String(condutivy3) + "\n";
  mensagem += "Tensão medida pH 3: " + String(ph3, 3) + " V | pH 3: " + String(ph3, 2) + "\n\n";

  mensagem += "-----Tanque 4-----\n";
  mensagem += "Temperatura Agua 4: " + String(temperatureWalter4) + " °C\n";
  mensagem += "TDS 4 Valor (PPM): " + String(tdsValue4) + "\n";
  mensagem += "Condutividade Eletrica 4 (ECC): " + String(condutivy4) + "\n";
  mensagem += "Tensão medida pH 4: " + String(ph4, 3) + " V | pH 4: " + String(ph4, 2) + "\n";


  sendMessage(mensagem);  // Envia para o Telegram

  // Repetindo as leituras a cada 5 segundos
  delay(5000);
}

// Função para ler e calcular o pH de um sensor específico
float lerPH(int pin) {
  int buf[10];
  // Coleta de 10 amostras do sensor
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(pin);
    delay(10);
  }

  // Ordena os valores coletados em ordem crescente
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        int temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  int valorMedio = 0;
  for (int i = 2; i < 8; i++) {  // Média das 6 amostras centrais
    valorMedio += buf[i];
  }

  // Conversão do ADC para tensão (ESP32 usa ADC de 12 bits, valor máximo = 4095)
  float tensao = (valorMedio * 3.3) / 4095.0 / 6;

  // Calculo do pH com base nas duas faixas
  float ph;

  // Determina a faixa de pH com base na tensão lida
  if (tensao < calibracao_ph7) {
    // Faixa ácida (pH 4 a 7)
    ph = m_4_7 * tensao + b_4_7;
  } else if (tensao > calibracao_ph7) {
    // Faixa básica (pH 7 a 10)
    ph = m_7_10 * tensao + b_7_10;
  } else {
    // Faixa neutra (próxima ao pH 7)
    ph = 7.0;  // pH 7 como valor neutro
  }

  return ph;
}
