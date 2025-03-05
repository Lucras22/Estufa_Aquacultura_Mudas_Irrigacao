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

// Definição das variáveis de calibração (substitua pelos valores obtidos nos testes)
int seco = 4095;    // Valor do sensor quando o solo estiver completamente seco (0%)
int medio = 2500;   // Valor do sensor para umidade intermediária (50%)
int molhado = 1000; // Valor do sensor quando o solo estiver totalmente úmido (100%)

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
}

int calcularUmidade(int valor) {
  if (valor <= molhado) return 100;   // Se for menor ou igual ao valor seco, retorna 0%
  if (valor >= seco) return 0; // Se for maior ou igual ao valor molhado, retorna 100%

  if (valor <= medio) {
    // Mapear entre seco (0%) e médio (50%)
    return map(valor, seco, medio, 0, 50);
  }

  // Mapear entre médio (50%) e molhado (100%)
  return map(valor, medio, molhado, 50, 100);
}

void loop() {
    // Leitura dos sensores
    int valor1 = analogRead(SOLO1);
    int valor2 = analogRead(SOLO2);
    int valor3 = analogRead(SOLO3);
    int valor4 = analogRead(SOLO4);
    int valor5 = analogRead(SOLO5);
    int valor6 = analogRead(SOLO6);
    int valor7 = analogRead(SOLO7);
    int valor8 = analogRead(SOLO8);
    int valor9 = analogRead(SOLO9);
    int valor10 = analogRead(SOLO10);
    int valor11 = analogRead(SOLO11);

    /*Serial.print("Valor ADC 1: "); Serial.println(valor1);
    Serial.print("Valor ADC 2: "); Serial.println(valor2);
    Serial.print("Valor ADC 3: "); Serial.println(valor3);
    Serial.print("Valor ADC 4: "); Serial.println(valor4);
    Serial.print("Valor ADC 5: "); Serial.println(valor5);
    Serial.print("Valor ADC 6: "); Serial.println(valor6);
    Serial.print("Valor ADC 7: "); Serial.println(valor7);
    Serial.print("Valor ADC 8: "); Serial.println(valor8);
    Serial.print("Valor ADC 9: "); Serial.println(valor9);
    Serial.print("Valor ADC 10: "); Serial.println(valor10);
    Serial.print("Valor ADC 11: "); Serial.println(valor11);*/

    // Conversão para porcentagem corretamente calibrada
    int umidade1 = calcularUmidade(valor1);
    int umidade2 = calcularUmidade(valor2);
    int umidade3 = calcularUmidade(valor3);
    int umidade4 = calcularUmidade(valor4);
    int umidade5 = calcularUmidade(valor5);
    int umidade6 = calcularUmidade(valor6);
    int umidade7 = calcularUmidade(valor7);
    int umidade8 = calcularUmidade(valor8);
    int umidade9 = calcularUmidade(valor9);
    int umidade10 = calcularUmidade(valor10);
    int umidade11 = calcularUmidade(valor11);

    //Serial.println("SENSOR - SOLO - BASICO");
    Serial.println("SENSOR - SOLO - CAPACITATIVO");

    // Exibir valores no Monitor Serial
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
    
    delay(2000); // Espera 2 segundos antes da próxima leitura
}
