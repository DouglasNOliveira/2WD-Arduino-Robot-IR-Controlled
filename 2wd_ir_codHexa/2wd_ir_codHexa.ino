/*

MIT License

Copyright (c) 2025 Douglas Nascimento de Oliveira

Para detralhes da licença, leia o campo apropriado no repositório original:
https://github.com/DouglasNOliveira/2WD-Arduino-Robot-IR-Controlled.git

*/

//------------------------------------------------------//
//    2WD com os códigos hexa para controle motor       //
//      Ponte H L298N com ENA / ENB ativos (PWM)        //
//      Controle via Receptor Infravermelho (IR)        //
//------------------------------------------------------//


#include <IRremote.h>
#include <L298NX2.h>

// -------------------- IR --------------------
#define PINO_RECEPTOR_IR 4

// -------------------- Ponte H (ordem correta) --------------------
#define ENA 10
#define IN1 9
#define IN2 8
#define ENB 5
#define IN3 7
#define IN4 6

L298NX2 motores(ENA, IN1, IN2, ENB, IN3, IN4);

// -------------------- LED RGB --------------------
#define LED_VERMELHO 13   // Ré
#define LED_VERDE    12   // Frente
#define LED_AZUL     11   // Esquerda / Direita

// -------------------- Códigos IR --------------------
const unsigned long TECLA_FRENTE   = 0xBF40FB04;
const unsigned long TECLA_RE       = 0xBE41FB04;
const unsigned long TECLA_ESQUERDA = 0xF807FB04;
const unsigned long TECLA_DIREITA  = 0xF906FB04;
const unsigned long TECLA_VEL_MED  = 0xEE11FB04;
const unsigned long TECLA_VEL_MAX  = 0xED12FB04;

// -------------------- Variáveis --------------------
int velocidade = 150;

// tecla ativa (mantém ação durante repeat)
unsigned long teclaAtiva = 0;

// watchdog
unsigned long ultimoTempoIR = 0;
const unsigned long TIMEOUT_PARADA = 200;

// -------------------- Funções auxiliares --------------------
void ApagarLEDs() {
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);
}

// -------------------- Movimentos --------------------
void Frente() {
  ApagarLEDs();
  digitalWrite(LED_VERDE, HIGH);

  motores.setSpeedA(velocidade);
  motores.setSpeedB(velocidade);
  motores.forward();
}

void Re() {
  ApagarLEDs();
  digitalWrite(LED_VERMELHO, HIGH);

  motores.setSpeedA(velocidade);
  motores.setSpeedB(velocidade);
  motores.backward();
}

void FrenteEsquerda() {
  ApagarLEDs();
  digitalWrite(LED_AZUL, HIGH);

  motores.setSpeedA(velocidade / 2);
  motores.setSpeedB(velocidade);
  motores.forward();
}

void FrenteDireita() {
  ApagarLEDs();
  digitalWrite(LED_AZUL, HIGH);

  motores.setSpeedA(velocidade);
  motores.setSpeedB(velocidade / 2);
  motores.forward();
}

void Pare() {
  motores.stop();
  ApagarLEDs();
}

// -------------------- Executa comando --------------------
void ExecutarTecla(unsigned long tecla) {
  switch (tecla) {
    case TECLA_FRENTE:   Frente(); break;
    case TECLA_RE:       Re(); break;
    case TECLA_ESQUERDA: FrenteEsquerda(); break;
    case TECLA_DIREITA:  FrenteDireita(); break;
    default:             Pare(); break;
  }
}

// -------------------- Setup --------------------
void setup() {
  Serial.begin(9600);

  IrReceiver.begin(PINO_RECEPTOR_IR);

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  Pare();

  Serial.println("Robo IR pronto com LED RGB");
}

// -------------------- Loop --------------------
void loop() {

  if (IrReceiver.decode()) {

    unsigned long codigo = IrReceiver.decodedIRData.decodedRawData;
    bool repeticao = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;

    // atualiza watchdog sempre que chegar algo
    ultimoTempoIR = millis();

    // se NÃO for repetição, atualiza estado
    if (!repeticao) {

      if (codigo == TECLA_VEL_MED) {
        velocidade = 150;
      }
      else if (codigo == TECLA_VEL_MAX) {
        velocidade = 255;
      }
      else {
        teclaAtiva = codigo;
      }
    }

    // mantém ação enquanto botão estiver pressionado
    ExecutarTecla(teclaAtiva);

    IrReceiver.resume();
  }

  // soltou o botão -> para
  if (millis() - ultimoTempoIR > TIMEOUT_PARADA) {
    teclaAtiva = 0;
    Pare();
  }
}