#include "PololuDriver.h"
#include "Ramps.h"
#include "SimpleTimer.h"
#include "Servo.h"

Ramps ramps = Ramps();
Servo servo;
int portaServo = 9;

SimpleTimer simpleTimer;

int passoMax = 48;
int posicaoAtual = 0;
int posicaoMax = 180;
float grauPorPasso = (float)posicaoMax / passoMax;

int quantidadePassos = 0;
bool movimentando = false;
int delayMovimento = 1;

unsigned long tempoAtual;

unsigned long tempoRecebimentoSerial;
float delayRecebimentoSerial = 50;
String valorRecebidoCompleto;
bool resetado;

void setup()
{
	Serial.begin(9600);

	ResetarValorRecebido();

	servo.attach(portaServo);
}

void CalcularPassosNecessarios(int posicaoDestino)
{
	if (movimentando)
		return;

	movimentando = true;

	int diferenca = posicaoDestino - posicaoAtual;

	int direcao = diferenca < 0 ? 1 : -1;

	ramps.motorZ.setDir(direcao);

	int diferencaAbsoluta = abs(diferenca);

	quantidadePassos = (int)diferencaAbsoluta / grauPorPasso;

	posicaoAtual = posicaoDestino;
}

void Movimentar()
{
	if (quantidadePassos == 0)
	{
		movimentando = false;

		return;
	}

	quantidadePassos--;

	ramps.motorZ.stepOn();
	delayMicroseconds(1000);
	ramps.motorZ.stepOff();
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

void ResetarValorRecebido()
{
	valorRecebidoCompleto = "";

	resetado = true;
}

void MovimentarServo(int graus)
{
	Serial.print("Movimentar Servo: ");
	Serial.println(graus);

	servo.write(graus);
}

void ProcessarValorRecebido()
{
	int valor = constrain(valorRecebidoCompleto.toInt(), 0, 180);

	//CalcularPassosNecessarios(valor);

	MovimentarServo(valor);
}

void loop()
{
	simpleTimer.run();

	tempoAtual = millis();

	ConstruirValorRecebido();

	Movimentar();
}
