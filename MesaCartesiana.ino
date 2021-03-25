#include "Arduino.h"
#include "StepperDriver.h"
/*============================================================
 * ALEXANDRE MARQUES BALDEZ JUNIOR
*                          DEFINES
* - A configuração dos pinos do arduino esta
* fundamentada no comando:#define nome_do_pino numero_do_pino
*
* - Os valores iniciais de alguns parametros também são
* configurados pelos defines como:
* modo do passo, estado inicial da chave fim de curso,
* limite maximo da coordenada do eixo,
* milimetros por volta do eixo,
* periodo maximo e minimo e declividade da curva sigmoidal
==============================================================*/
//EIXO X
#define MODO_PASSO_X 1  //1 para 1/1, 2 para 1/2, 4 para 1/4, 8 para 1/8, 16 para 1/16
#define PINO_ENABLE_X 3
#define PINO_RESET_X 4
#define PINO_SLEEP_X 5
#define PINO_M0_X 6
#define PINO_M1_X 7
#define PINO_M2_X 8
#define PINO_IN1_X 9
#define PINO_IN2_X 10
#define PINO_IN3_X 11
#define PINO_IN4_X 12
#define PINO_CURSOMAXIMO_X 3
#define PINO_CURSOMINIMO_X 4
#define ESTADO_FIM_DE_CURSO_X LOW
#define MILIMETROS_POR_VOLTA_X 8
#define LIMITE_MAXIMO_X 10
#define SIG_PERIODO_MAXIMO_X 600
#define SIG_PERIODO_MINIMO_X 100
#define SIG_DECLIVIDADE_X 0.1

//EIXO Y
#define MODO_PASSO_Y 1  //1 para 1/1, 2 para 1/2, 4 para 1/4, 8 para 1/8, 16 para 1/16
#define PINO_ENABLE_Y 15
#define PINO_RESET_Y 16
#define PINO_SLEEP_Y 17
#define PINO_M0_Y 18
#define PINO_M1_Y 19
#define PINO_M2_Y 20
#define PINO_IN1_Y 21
#define PINO_IN2_Y 22
#define PINO_IN3_Y 23
#define PINO_IN4_Y 24
#define PINO_CURSOMAXIMO_Y 5
#define PINO_CURSOMINIMO_Y 6
#define ESTADO_FIM_DE_CURSO_Y LOW
#define MILIMETROS_POR_VOLTA_Y 8
#define LIMITE_MAXIMO_Y 10
#define SIG_PERIODO_MAXIMO_Y 600
#define SIG_PERIODO_MINIMO_Y 100
#define SIG_DECLIVIDADE_Y 0.1
/*==========================================
*            VARIAVEIS GLOBAIS
===========================================*/
String leitura;
byte x = 0, y = 0;

/*
 SENTIDO HORARIO ->
byte WAVESTEP [4] = {9, 3, 6, 12};
byte FULLSTEP [4] = {1, 2, 4,  8};
byte HALFSTEP [8] = {9, 1, 3, 2, 6, 4, 12, 8};

P|I4|I3|I2|I1|
P|BN|BP|AN|AP|
--------------
0| 1| 0| 0| 1|9
1| 0| 0| 0| 1|1
2| 0| 0| 1| 1|3
3| 0| 0| 1| 0|2
4| 0| 1| 1| 0|6
5| 0| 1| 0| 0|4
6| 1| 1| 0| 0|12
7| 1| 0| 0| 0|8

indices  impares WAVESTEP
indices    pares FULLSTEP
todos os indices HALFSTEP

0x00 0000  0
0x01 0001  1
0x02 0010  2
0x03 0011  3
0x04 0100  4
0x05 0101  5
0x06 0110  6
0x07 0111  7
0x08 1000  8
0x09 1001  9
0x0A 1010 10
0x0B 1011 11
0x0C 1100 12
0x0D 1101 13
0x0E 1110 14
0x0F 1111 15
*/

byte vetorPassos     [8] = {    9,    1,    3,    2,    6,    4,   12,    8};
byte vetorPassosHexa [8] = { 0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};
byte vetorPassosByte [8] = {B1001,B0001,B0011,B0010,B0110,B0100,B1100,B1000};

Pino *pino13;

Pino *pinoEnableX, *pinoResetX, *pinoSleepX, *pinoM0X, *pinoM1X, *pinoM2X, *pinoIN1X, *pinoIN2X, *pinoIN3X, *pinoIN4X;
Pino *cursoMaximoX, *cursoMinimoX;
Sigmoidal *sigmoidalX;
Driver *driverX;
Eixo *eixoX;

Pino *pinoEnableY, *pinoResetY, *pinoSleepY, *pinoM0Y, *pinoM1Y, *pinoM2Y, *pinoIN1Y, *pinoIN2Y, *pinoIN3Y, *pinoIN4Y;
Pino *cursoMaximoY, *cursoMinimoY;
Sigmoidal *sigmoidalY;
Driver *driverY;
Eixo *eixoY;


/*==========================================
*                 MÉTODOS
===========================================*/
void interpretador(String comando);
void movimentarMesa(String coordenada);
void escolherModoPasso(Eixo *e, String modoPasso);

void setup(){
	Serial.begin(9600);

	pino13 = new Pino(Pino::DIGITAL, 13, OUTPUT, LOW);

	pinoEnableX  = new Pino(Pino::DIGITAL, PINO_ENABLE_X, OUTPUT, LOW);
	pinoResetX   = new Pino(Pino::DIGITAL, PINO_RESET_X, OUTPUT, LOW);
	pinoSleepX   = new Pino(Pino::DIGITAL, PINO_SLEEP_X, OUTPUT, LOW);
	pinoM0X      = new Pino(Pino::DIGITAL, PINO_M0_X, OUTPUT, LOW);
	pinoM1X      = new Pino(Pino::DIGITAL, PINO_M1_X, OUTPUT, LOW);
	pinoM2X      = new Pino(Pino::DIGITAL, PINO_M2_X, OUTPUT, LOW);
	pinoIN1X     = new Pino(Pino::DIGITAL, PINO_IN1_X, OUTPUT, LOW);
	pinoIN2X     = new Pino(Pino::DIGITAL, PINO_IN2_X, OUTPUT, LOW);
	pinoIN3X     = new Pino(Pino::DIGITAL, PINO_IN3_X, OUTPUT, LOW);
	pinoIN4X     = new Pino(Pino::DIGITAL, PINO_IN4_X, OUTPUT, LOW);
	cursoMaximoX = new Pino(Pino::DIGITAL, PINO_CURSOMAXIMO_X, INPUT, HIGH);
	cursoMinimoX = new Pino(Pino::DIGITAL, PINO_CURSOMINIMO_X, INPUT, HIGH);
	sigmoidalX   = new Sigmoidal(SIG_PERIODO_MAXIMO_X, SIG_PERIODO_MINIMO_X, SIG_DECLIVIDADE_X);

	pinoEnableY  = new Pino(Pino::DIGITAL, PINO_ENABLE_Y, OUTPUT, LOW);
	pinoResetY   = new Pino(Pino::DIGITAL, PINO_RESET_Y, OUTPUT, LOW);
	pinoSleepY   = new Pino(Pino::DIGITAL, PINO_SLEEP_Y, OUTPUT, LOW);
	pinoM0Y      = new Pino(Pino::DIGITAL, PINO_M0_Y, OUTPUT, LOW);
	pinoM1Y      = new Pino(Pino::DIGITAL, PINO_M1_Y, OUTPUT, LOW);
	pinoM2Y      = new Pino(Pino::DIGITAL, PINO_M2_Y, OUTPUT, LOW);
	pinoIN1Y     = new Pino(Pino::DIGITAL, PINO_IN1_Y, OUTPUT, LOW);
	pinoIN2Y     = new Pino(Pino::DIGITAL, PINO_IN2_Y, OUTPUT, LOW);
	pinoIN3Y     = new Pino(Pino::DIGITAL, PINO_IN3_Y, OUTPUT, LOW);
	pinoIN4Y     = new Pino(Pino::DIGITAL, PINO_IN4_Y, OUTPUT, LOW);
	cursoMaximoY = new Pino(Pino::DIGITAL, PINO_CURSOMAXIMO_Y, INPUT, HIGH);
	cursoMinimoY = new Pino(Pino::DIGITAL, PINO_CURSOMINIMO_Y, INPUT, HIGH);
	sigmoidalY   = new Sigmoidal(SIG_PERIODO_MAXIMO_Y, SIG_PERIODO_MINIMO_Y, SIG_DECLIVIDADE_Y);

	driverX = new Driver(
		Driver::WAVESTEP,
		pinoEnableX,
		pinoResetX,
		pinoSleepX,
		pinoM0X,
		pinoM1X,
		pinoM2X,
		pinoIN1X,
		pinoIN2X,
		pinoIN3X,
		pinoIN4X,
		vetorPassos
	);

	driverY = new Driver(
		Driver::WAVESTEP,
		pinoEnableY,
		pinoResetY,
		pinoSleepY,
		pinoM0Y,
		pinoM1Y,
		pinoM2Y,
		pinoIN1Y,
		pinoIN2Y,
		pinoIN3Y,
		pinoIN4Y,
		vetorPassos
	);

	eixoX = new Eixo(
		driverX,
		MILIMETROS_POR_VOLTA_X,
		ESTADO_FIM_DE_CURSO_X,
		cursoMaximoX,
		cursoMinimoX,
		sigmoidalX
	);

	eixoY = new Eixo(
		driverY,
		MILIMETROS_POR_VOLTA_Y,
		ESTADO_FIM_DE_CURSO_Y,
		cursoMaximoY,
		cursoMinimoY,
		sigmoidalY
	);

	delay(3000);
}

/*
  O método loop está vazio mas na verdade ele contém em suas entranhas uma
  chamada do método SerialEvent, sendo assim uma boa aplicação do loop é
  quando necessitamos executar uma ação que não depende da porta Serial como
  enviar dados de um sensor.
*/

void loop(){

}

/*
  O método SerialEvent fica ouvindo a porta Serial cada vez que
  é enviado um comando para o arduino o SerialEvent o envia para
  o interpretador que toma a decisão do que fazer
*/

void serialEvent(){
  if(Serial.available() > 0){
    leitura = Serial.readStringUntil('\n');
    if(!leitura.equals("")){
      interpretador(leitura);
      leitura = "";
    }
  }
}

/*
  Para mover a mesa se usa o comando : MOVER x,y ex: MOVER 1,1
  Para ligar o Led 13 do arduino se usa o comando: L
  Para desligar o Led 13 do arduino se usa o comando: D
  Para mudar o passo do motor X se usa o comando: MODOPASSOX numero ex:MODOPASSOX 16
  Para mudar o passo do motor Y se usa o comando: MODOPASSOY numero ex:MODOPASSOY  8
*/

void interpretador(String comando){
  comando.toUpperCase();
  if (comando.indexOf("MOVER") > -1) movimentarMesa(comando.substring(comando.indexOf(" ")+1));
  if (comando.indexOf("L") > -1) pino13->setEstado(HIGH);
  if (comando.indexOf("D") > -1) pino13->setEstado(LOW);
  if (comando.indexOf("MODOPASSOX") > -1) escolherModoPasso(eixoX, comando.substring(comando.indexOf(" ")+1));
  if (comando.indexOf("MODOPASSOY") > -1) escolherModoPasso(eixoY, comando.substring(comando.indexOf(" ")+1));
}

/*
  O método movimentar mesa recebe um literal com o comando MOVER 1,1
  separa as coordenada e as envia para os objetos da classe Eixo eixoX e eixoY
  lembrando que para a execução x,y devem diferentes de sua posição anterior
  e estarem dentro do limite Maximo de coordenadas
*/

void movimentarMesa(String coordenada){
  if(!(coordenada.indexOf(",") == -1)){
    x = coordenada.substring(0,coordenada.indexOf(",")).toInt();
    y = coordenada.substring(coordenada.indexOf(",")+1).toInt();
    if((x >= 0 && x <= LIMITE_MAXIMO_X) && (y >= 0 && y <= LIMITE_MAXIMO_Y)){
      if(x != eixoX->getPosicao()) eixoX->rotacionarPara(x);
      //if(y != eixoY->getPosicao()) eixoY->rotacionarPara(y);
    }
  }
}

/*
MP|M0|M1|M2|R
--------------
 1| 0| 0| 0| 0 full step
 2| 1| 0| 0| 4 half step
 4| 0| 1| 0| 2 1/4  step
 8| 1| 1| 0| 6 1/8  step
16| 0| 0| 1| 1 1/16 step
32| 1| 0| 1| 5 1/32 step
32| 0| 1| 1| 3 1/32 step
32| 1| 1| 1| 7 1/32 step

e.getDriver().setarModoPasso(MP,R);
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07
*/

void escolherModoPasso(Eixo *e, String modoPasso){
  switch(modoPasso.toInt()){
    case  1: e->getDriver()->setarModoPasso( 1,0);break;
    case  2: e->getDriver()->setarModoPasso( 2,4);break;
    case  4: e->getDriver()->setarModoPasso( 4,2);break;
    case  8: e->getDriver()->setarModoPasso( 8,6);break;
    case 16: e->getDriver()->setarModoPasso(16,1);break;
    case 32: e->getDriver()->setarModoPasso(32,7);break;
  }
}
