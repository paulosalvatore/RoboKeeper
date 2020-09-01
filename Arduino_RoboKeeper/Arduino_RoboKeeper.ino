/*
 * RobôKeeper - FIAP
 * Desenvolvido por: Paulo Salvatore e Vinicius Oliveira
*/
int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;

void setup()
{
  // Define os pinos como saida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

int tempo = 50;

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

