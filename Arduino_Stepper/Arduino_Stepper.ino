/*
 * RobôKeeper - FIAP
 * Desenvolvido por: Paulo Salvatore e Vinicius Oliveira
*/

#include <Servo.h>
#include <SimpleTimer.h>

SimpleTimer simpleTimer;

Servo servo;
int portaServo = 9;

unsigned long tempoAtual;

unsigned long tempoRecebimentoSerial;
float delayRecebimentoSerial = 200;
String valorRecebidoCompleto;
bool resetado;

void setup() {
  servo.attach(portaServo);
  servo.write(0);

  Serial.begin(9600);

  ResetarValorRecebido();
}

void ConstruirValorRecebido()
{
  if (Serial.available() > 0)
  {
    resetado = false;

    tempoRecebimentoSerial = tempoAtual;

    while (Serial.available() > 0)
    {
      char valorRecebido = Serial.read();

      valorRecebidoCompleto += valorRecebido;
    }
  }
  else if (!resetado && tempoAtual > tempoRecebimentoSerial + delayRecebimentoSerial)
  {
    ProcessarValorRecebido();

    ResetarValorRecebido();
  }
}

void ProcessarValorRecebido()
{
  int valor = valorRecebidoCompleto.toInt();

  valor = constrain(valor, 0, 180);

  Serial.println(valor);

  servo.write(valor);
}

void ResetarValorRecebido()
{
  valorRecebidoCompleto = "";

  resetado = true;
}

void loop()
{
  tempoAtual = millis();

  simpleTimer.run();

  ConstruirValorRecebido();
}

/*
#include <Stepper.h>

#define STEPS 100

Stepper stepper(STEPS, 8, 9, 10, 11);

void setup()
{
  stepper.setSpeed(30);
}
void loop()
{
  stepper.step(100);
  delay(100);
  stepper.step(-100);
  delay(100);
}
*/

/*
int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;

void setup()
{
  // Define os pinos como saída

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

int tempo = 1;

void loop()
{
  // Passo 1
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(tempo);

  // Passo 2
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(tempo);

  // Passo 3
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(tempo);

  // Passo 4
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(tempo);
}
*/

/*
int ponteH_BNegativo = 11;
int ponteH_BPositivo = 10;
int ponteH_ANegativo = 9;
int ponteH_APositivo = 8;

void setup() {
  pinMode(ponteH_BNegativo, OUTPUT);
  pinMode(ponteH_BPositivo, OUTPUT);
  pinMode(ponteH_ANegativo, OUTPUT);
  pinMode(ponteH_APositivo, OUTPUT);
}

void loop() {
}
*/

