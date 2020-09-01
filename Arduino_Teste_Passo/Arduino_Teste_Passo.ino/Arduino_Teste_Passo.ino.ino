/*
 * RobôKeeper - FIAP
 * Desenvolvido por: Paulo Salvatore e Vinicius Oliveira
*/

#include <AccelStepper.h>

int velocidade_motor = 100;
int aceleracao_motor = 100;
int sentido_horario = 0;
int sentido_antihorario = 0;
char numero;

// Definicao pino ENABLE
int pino_enable = 8;

// Definicao pinos STEP e DIR
AccelStepper motor1(1, 4, 7);

int ms1 = 7;
int ms2 = 6;
int ms3 = 5;

void setup()
{
  pinMode(ms1, OUTPUT);
  pinMode(ms2, OUTPUT);
  pinMode(ms3, OUTPUT);

  Serial.begin(9600);
  pinMode(pino_enable, OUTPUT);
  // Configuracoes iniciais motor de passo
  motor1.setMaxSpeed(velocidade_motor);
  motor1.setSpeed(velocidade_motor);
  motor1.setAcceleration(aceleracao_motor);

  Serial.println("Digite 1, 2 ou 3 e clique em ENVIAR...");
}

void loop()
{
  digitalWrite(ms1, LOW);
  digitalWrite(ms2, LOW);
  digitalWrite(ms3, LOW);

  // Aguarda os caracteres no serial monitor
  if (Serial.available() > 0)
  {
    numero = Serial.read();

    Serial.println(numero);

    if (numero == '1')
    {
      Serial.println("Numero 1 recebido - Girando motor sentido horario.");
      digitalWrite(pino_enable, LOW);
      sentido_horario = 1;
      sentido_antihorario = 0;
    }
    else if (numero == '2')
    {
      Serial.println("Numero 2 recebido - Girando motor sentido anti-horario.");
      digitalWrite(pino_enable, LOW);
      sentido_horario = 0;
      sentido_antihorario = 1;
    }
    else if (numero == '3')
    {
      Serial.println("Numero 3 recebido - Parando motor...");
      sentido_horario = 0;
      sentido_antihorario = 0;
      motor1.moveTo(0);
      digitalWrite(pino_enable, HIGH);
    }
  }

  // Move o motor no sentido horario
  if (sentido_horario == 1)
  {
    motor1.moveTo(10000);
  }

  // Move o motor no sentido anti-horario
  if (sentido_antihorario == 1)
  {
    motor1.moveTo(-10000);
  }

  // Comando para acionar o motor no sentido especificado
  motor1.run();
}

