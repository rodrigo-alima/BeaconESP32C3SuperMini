/*
 * Copyright 2024 Rodrigo <py4rdg@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <si5351.h>
#include <Preferences.h>

//Nome do namespace onde será armazenada a mensagem na memoria
const char *NAMESPACE = "beacon";
const char *chave = "mensagem";

// Definir o tamanho do display OLED
#define SCREEN_WIDTH 128  // Largura do display (em pixels)
#define SCREEN_HEIGHT 32  // Altura do display (em pixels)
#define OLED_RESET    -1

#define MAX_SIZE 50 //Tamanho maximo da mensagem

//Cria o objeto display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Cria o objeto si5351
Si5351 si5351;
//Frequencia de operação em Hetz
uint64_t freq_hz = 5000300000; 

// Configurações de rede Wi-Fi
const char* ssid = "BEACON";       // Substitua pelo nome da sua rede Wi-Fi
const char* password = "12345678"; // Substitua pela senha da sua rede Wi-Fi

// Configurações do servidor UDP
const unsigned int localUdpPort = 55000; // Porta local para o servidor UDP

// Array para armazenar a mensagem recebida
char mensagemRecebida[MAX_SIZE]; // Ajuste o tamanho conforme necessário
bool mensagemRecebidaFlag = false; // Flag para indicar quando a mensagem foi recebida

//Objeto UDP
WiFiUDP udp; 

bool conectado; 

//Deve-se manter a proporção 
//Ponto (.): 1 unidade de tempo.
//Traço (-): 3 unidades de tempo (três vezes mais longo que o ponto).
//Espaço entre símbolos (dentro da mesma letra): 1 unidade de tempo.
//Espaço entre letras (dentro da mesma palavra): 3 unidades de tempo.
//Espaço entre palavras: 7 unidades de tempo. No entanto deve ser considerado o tempo enter letras 
//no final de cada letra

//Em milisegundos
#define PONTO 150
#define TRACO 450
#define ENTRE_SIMBOLOS 150
#define ENTRE_LETRAS 450
#define SPACE 600

//Define o pino de saida
#define SAIDA 5

// Tipo de ponteiro para funções que não retornam nada e não recebem argumentos
typedef void (*func_ptr_t)();

// Criar um vetor de ponteiros para funções
func_ptr_t *vetorFuncoes = NULL;

void ponto(){
  digitalWrite(SAIDA, HIGH);  
  delay(PONTO);                      
  digitalWrite(SAIDA, LOW);
}

void traco(){
  digitalWrite(SAIDA, HIGH);
  delay(TRACO);
  digitalWrite(SAIDA, LOW);
}

// Funções que serão chamadas com base nos caracteres
void funcA() {
    Serial.println("Função A chamada");
    ponto();   
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void funcB() {
    Serial.println("Função B chamada");
    traco();   // turn the LED off by making the voltage LOW
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcC() {
    Serial.println("Função C chamada");
    traco();   // turn the LED off by making the voltage LOW
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcD() {
    Serial.println("Função D chamada");
    traco();   
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcE() {
    Serial.println("Função E chamada");
    ponto();   // turn the LED off by making the voltage LOW
    delay(ENTRE_LETRAS);
}

void funcF() {
    Serial.println("Função F chamada");
    ponto();  // turn the LED off by making the voltage LOW
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcG() {
    Serial.println("Função G chamada");
    traco();   
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcH() {
    Serial.println("Função H chamada");
    ponto();   // turn the LED off by making the voltage LOW
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcI() {
    Serial.println("Função I chamada");
    ponto();   
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcJ() {
    Serial.println("Função J chamada");
    ponto();  
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);

}

void funcK() {
    Serial.println("Função K chamada");
    traco();   
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void funcL() {
    Serial.println("Função L chamada");
    ponto();   
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);

}

void funcM() {
    Serial.println("Função M chamada");
    traco();   
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void funcN() {
    Serial.println("Função N chamada");
    traco();  
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcO() {
    Serial.println("Função O chamada");
    traco();   
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void funcP() {
    Serial.println("Função P chamada");
    ponto();   
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);

}

void funcQ() {
    Serial.println("Função Q chamada");
    traco();  
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);

}

void funcR() {
    Serial.println("Função R chamada");
    ponto();   
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcS() {
    Serial.println("Função S chamada");
    ponto();   
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcT() {
    Serial.println("Função T chamada");
    traco();
    delay(ENTRE_LETRAS);
}

void funcU() {
    Serial.println("Função U chamada");
    ponto();   
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void funcV() {
    Serial.println("Função V chamada");
    ponto();   
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);

}

void funcW() {
    Serial.println("Função W chamada");
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void funcX() {
    Serial.println("Função X chamada");
    traco();   
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void funcY() {
    Serial.println("Função Y chamada");
    traco();   
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void funcZ() {
    Serial.println("Função Z chamada");
    traco();  
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void func0() {
    Serial.println("Função 0 chamada");
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void func1() {
    Serial.println("Função 1 chamada");
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void func2() {
    Serial.println("Função 2 chamada");
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void func3() {
    Serial.println("Função 3 chamada");
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void func4() {
    Serial.println("Função 4 chamada");
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_LETRAS);
}

void func5() {
    Serial.println("Função 5 chamada");
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void func6() {
    Serial.println("Função 6 chamada");
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void func7() {
    Serial.println("Função 7 chamada");
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void func8() {
    Serial.println("Função 8 chamada");
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void func9() {
    Serial.println("Função 9 chamada");
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcBar() {
    Serial.println("Função / chamada");
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_SIMBOLOS);
    traco();
    delay(ENTRE_SIMBOLOS);
    ponto();
    delay(ENTRE_LETRAS);
}

void funcSpace() {
    Serial.println("Função space chamada");
    delay(SPACE);
}

// Função que inicializa o vetor de ponteiros com base no tamanho da mensagem
void inicializarVetorDeFuncoes(char *mensagem, func_ptr_t** vetorFuncoes, int tamanhoMensagem) {

    *vetorFuncoes = (func_ptr_t*)malloc(tamanhoMensagem * sizeof(func_ptr_t));

    if (*vetorFuncoes == NULL) {
        Serial.println("Falha ao alocar memória para o vetor de funções!");
        return;
    }

    // Percorrer a mensagem e associa os caracteres às funções e armazena no vetor de funções
    for (int i = 0; i < tamanhoMensagem; i++) {
        switch (mensagem[i]) {
            case 'A':
                (*vetorFuncoes)[i] = funcA;
                break;
            case 'B':
                (*vetorFuncoes)[i] = funcB;
                break;
            case 'C':
                (*vetorFuncoes)[i] = funcC;
                break;
            case 'D':
                (*vetorFuncoes)[i] = funcD;
                break;
            case 'E':
                (*vetorFuncoes)[i] = funcE;
                break;
            case 'F':
                (*vetorFuncoes)[i] = funcF;
                break;
            case 'G':
                (*vetorFuncoes)[i] = funcG;
                break;
            case 'H':
                (*vetorFuncoes)[i] = funcH;
                break;
            case 'I':
                (*vetorFuncoes)[i] = funcI;
                break;
            case 'J':
                (*vetorFuncoes)[i] = funcJ;
                break;
            case 'K':
                (*vetorFuncoes)[i] = funcK;
                break;
            case 'L':
                (*vetorFuncoes)[i] = funcL;
                break;
            case 'M':
                (*vetorFuncoes)[i] = funcM;
                break;
            case 'N':
                (*vetorFuncoes)[i] = funcN;
                break;
            case 'O':
                (*vetorFuncoes)[i] = funcO;
                break;
            case 'P':
                (*vetorFuncoes)[i] = funcP;
                break;
            case 'Q':
                (*vetorFuncoes)[i] = funcQ;
                break;
            case 'R':
                (*vetorFuncoes)[i] = funcR;
                break;
            case 'S':
                (*vetorFuncoes)[i] = funcS;
                break;
            case 'T':
                (*vetorFuncoes)[i] = funcT;
                break;
            case 'U':
                (*vetorFuncoes)[i] = funcU;
                break;
            case 'V':
                (*vetorFuncoes)[i] = funcV;
                break;
            case 'W':
                (*vetorFuncoes)[i] = funcW;
                break;
            case 'X':
                (*vetorFuncoes)[i] = funcX;
                break;
            case 'Y':
                (*vetorFuncoes)[i] = funcY;
                break;
            case 'Z':
                (*vetorFuncoes)[i] = funcZ;
                break;
            case '0':
                (*vetorFuncoes)[i] = func0;
                break;
            case '1':
                (*vetorFuncoes)[i] = func1;
                break;
            case '2':
                (*vetorFuncoes)[i] = func2;
                break;
            case '3':
                (*vetorFuncoes)[i] = func3;
                break;
            case '4':
                (*vetorFuncoes)[i] = func4;
                break;
            case '5':
                (*vetorFuncoes)[i] = func5;
                break;
            case '6':
                (*vetorFuncoes)[i] = func6;
                break;
            case '7':
                (*vetorFuncoes)[i] = func7;
                break;
            case '8':
                (*vetorFuncoes)[i] = func8;
                break;
            case '9':
                (*vetorFuncoes)[i] = func9;
                break;
            case ' ':
                (*vetorFuncoes)[i] = funcSpace;
                break;
            case '/':
                (*vetorFuncoes)[i] = funcBar;
                break;    
            default:
                (*vetorFuncoes)[i] = NULL; // Não faz nada se o caractere não for reconhecido
                break;
        }
    }
}

// Função que percorre o vetor e chama as funções apontadas pelos ponteiros
void chamarFuncoes(func_ptr_t* vetorFuncoes, int tamanhoMensagem) {
    for (int i = 0; i < tamanhoMensagem; i++) {
        if (vetorFuncoes[i] != NULL) {
            vetorFuncoes[i]();  // Chama a função usando o ponteiro
        }
    }
}

bool conectarWifi(){

  int contador = 0;

  // Conectando ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && contador < 4) {
    delay(1000);
    Serial.print(".");
    contador += 1;
  }
   
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("\nWi-Fi conectado.");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

    // Inicia o servidor UDP
    udp.begin(localUdpPort);
    Serial.printf("Servidor UDP iniciado na porta %d\n", localUdpPort);

    // Inicializa o display OLED
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setFont(&FreeMono9pt7b);
    display.setTextSize(1); 
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println(WiFi.localIP());
    display.display();
    return true;
  }
  //Desativando o modulo WiFi
  WiFi.mode(WIFI_OFF);
  Serial.println("Módulo Wifi desativado");
  return false;
}

bool lerMensagemUdp() {
  // Verifica se há dados disponíveis no servidor UDP
  int packetSize = udp.parsePacket();
  if (packetSize > 0) {
    Serial.printf("Pacote recebido, tamanho: %d\n", packetSize);

    // Lê o pacote recebido
    int len = udp.read(mensagemRecebida, sizeof(mensagemRecebida) - 1);
    if (len > 0) {
      mensagemRecebida[len] = '\0'; // Garante terminação da string

      Serial.print("Mensagem recebida: ");
      Serial.println(mensagemRecebida);

      // Opcional: Enviar uma resposta ao cliente
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.print("Mensagem recebida com sucesso.");
      udp.endPacket();

      mensagemRecebidaFlag = true; // Flag de mensagem recebida
      
      //Desativa o servidor udp após recepção da mensagem
      udp.stop();
      Serial.println("Servidor udp desativado");

      display.clearDisplay();
      display.display();
      return true;
    }
  }
  return false;
}


bool armazenarMensagem(const char *chave, const char *valor){
  
  //Inicia o objeto preferences
  Preferences preferences;  

  if(strlen(valor) >= MAX_SIZE){
    Serial.println("Valor excede o tamanho maximo");
    return false;
  }

  //Abre o namespace para o modo escrita
  preferences.begin(NAMESPACE, false);
  preferences.putString(chave, valor);
  preferences.end();

  return true;
  
}

bool lerMensagemGravada(const char *chave, char *buffer, size_t bufferSize){

  Preferences preferences;

  preferences.begin(NAMESPACE, true);
  String valor = preferences.getString(chave, "");
  preferences.end();
  if(valor.isEmpty()){
    Serial.println("Nenhum valor encontrado");
    return false;
  }
  if(valor.length() >= bufferSize){
    Serial.println("Buffer pequeno demais");
    return false;
  }
  valor.toCharArray(buffer, bufferSize);
}

void setup() {

  //Inicia comunicação serial
  Serial.begin(115200);

  //Inicia o si5351 com a frequencia inicial
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  si5351.set_freq(freq_hz, SI5351_CLK0);

  //Configura o pino de saida
  pinMode(SAIDA, OUTPUT);

  //Conecta o Wifi
  Serial.println("Inicializando Wi-Fi...");  
  conectado = conectarWifi();
}

void loop() {

  if(conectado == true){

    bool result = false;

    result = lerMensagemUdp();

    while(result == false) result = lerMensagemUdp();
   
  }

  //Caso a mensagem tenha sido recebida com sucesso
  //Inicia o vetor de funções caso contrario inicia com
  //a mensagem padrao "TEST"
  if (mensagemRecebidaFlag == true){
    
    //Grava a mensagem na memória
    bool result = armazenarMensagem(chave, mensagemRecebida);
    
    // Inicializar o vetor de ponteiros com as funções correspondentes aos caracteres da string
    inicializarVetorDeFuncoes(mensagemRecebida, &vetorFuncoes, strlen(mensagemRecebida));
    
  } else {

    bool result = lerMensagemGravada(chave, mensagemRecebida, sizeof(mensagemRecebida));

    mensagemRecebidaFlag = true;

    // Inicializar o vetor de ponteiros com as funções correspondentes aos caracteres da string
    inicializarVetorDeFuncoes(mensagemRecebida, &vetorFuncoes, strlen(mensagemRecebida));
  
  }

  while(mensagemRecebidaFlag){
    // Percorrer o vetor e chamar as funções
    chamarFuncoes(vetorFuncoes, strlen(mensagemRecebida));
    delay(5000);  // Espera 5 segundos antes de chamar as funções novamente
  }
}
