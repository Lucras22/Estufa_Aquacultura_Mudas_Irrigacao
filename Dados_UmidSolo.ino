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

#define SOLO1 32
#define SOLO2 33
#define SOLO3 34
#define SOLO4 35
#define SOLO5 25
#define SOLO6 26
#define SOLO7 27
#define SOLO8 14
#define SOLO9 4
#define SOLO10 13
#define SOLO11 15

// Definição das variáveis de calibração
int seco = 4095;    
int medio = 2500;   
int molhado = 1000; 

void setup() {
  Serial.begin(115200);
  
  pinMode(SOLO1, INPUT);
  pinMode(SOLO2, INPUT);
  pinMode(SOLO3, INPUT);
  pinMode(SOLO4, INPUT);
  pinMode(SOLO5, INPUT);
  pinMode(SOLO6, INPUT);
  pinMode(SOLO7, INPUT);
  pinMode(SOLO8, INPUT);
  pinMode(SOLO9, INPUT);
  pinMode(SOLO10, INPUT);
  pinMode(SOLO11, INPUT);

  WiFi.mode(WIFI_OFF); // Garante que o Wi-Fi inicia desligado
}

int calcularUmidade(int valor) {
  if (valor <= molhado) return 100;
  if (valor >= seco) return 0;

  if (valor <= medio) {
    return map(valor, seco, medio, 0, 50);
  }

  return map(valor, medio, molhado, 50, 100);
}

void loop() {
  // Coletar os dados dos sensores com Wi-Fi desligado
  int umidade1 = calcularUmidade(analogRead(SOLO1));
  int umidade2 = calcularUmidade(analogRead(SOLO2));
  int umidade3 = calcularUmidade(analogRead(SOLO3));
  int umidade4 = calcularUmidade(analogRead(SOLO4));
  int umidade5 = calcularUmidade(analogRead(SOLO5));
  int umidade6 = calcularUmidade(analogRead(SOLO6));
  int umidade7 = calcularUmidade(analogRead(SOLO7));
  int umidade8 = calcularUmidade(analogRead(SOLO8));
  int umidade9 = calcularUmidade(analogRead(SOLO9));
  int umidade10 = calcularUmidade(analogRead(SOLO10));
  int umidade11 = calcularUmidade(analogRead(SOLO11));

  //Serial.println("DADOS - SENSOR - SOLO - BÁSICO");
  Serial.println("DADOS - SENSOR - SOLO - CAPACITATIVO");
  
  Serial.print("Umidade 1: "); Serial.print(umidade1); Serial.println("%");
  Serial.print("Umidade 2: "); Serial.print(umidade2); Serial.println("%");
  Serial.print("Umidade 3: "); Serial.print(umidade3); Serial.println("%");
  Serial.print("Umidade 4: "); Serial.print(umidade4); Serial.println("%");
  Serial.print("Umidade 5: "); Serial.print(umidade5); Serial.println("%");
  Serial.print("Umidade 6: "); Serial.print(umidade6); Serial.println("%");
  Serial.print("Umidade 7: "); Serial.print(umidade7); Serial.println("%");
  Serial.print("Umidade 8: "); Serial.print(umidade8); Serial.println("%");
  Serial.print("Umidade 9: "); Serial.print(umidade9); Serial.println("%");
  Serial.print("Umidade 10: "); Serial.print(umidade10); Serial.println("%");
  Serial.print("Umidade 11: "); Serial.print(umidade11); Serial.println("%");

  Serial.println("----------------------");

  // Criar a mensagem com os dados coletados
  //String mensagem = "DADOS - SENSOR - SOLO - BÁSICO:\n\n";
  String mensagem = "DADOS - SENSOR - SOLO - CAPACITATIVO:\n\n";
  
  mensagem += "Umidade 1: " + String(umidade1) + "%\n";
  mensagem += "Umidade 2: " + String(umidade2) + "%\n";
  mensagem += "Umidade 3: " + String(umidade3) + "%\n";
  mensagem += "Umidade 4: " + String(umidade4) + "%\n";
  mensagem += "Umidade 5: " + String(umidade5) + "%\n";
  mensagem += "Umidade 6: " + String(umidade6) + "%\n";
  mensagem += "Umidade 7: " + String(umidade7) + "%\n";
  mensagem += "Umidade 8: " + String(umidade8) + "%\n";
  mensagem += "Umidade 9: " + String(umidade9) + "%\n";
  mensagem += "Umidade 10: " + String(umidade10) + "%\n";
  mensagem += "Umidade 11: " + String(umidade11) + "%\n";

  // Agora liga o Wi-Fi e envia os dados
  sendMessage(mensagem);

  delay(10000); // Espera 10 segundos antes da próxima coleta
}
