#include "Arduino.h"
#include "StepperDriver.h"

#define MODO_PASSO_X 1
#define PINO_ENABLE_X 30
#define PINO_RESET_X 40
#define PINO_SLEEP_X 50
#define PINO_M0_X 60
#define PINO_M1_X 70
#define PINO_M2_X 80
#define PINO_PASSO_X 200
#define PINO_DIRECAO_X 200
#define PINO_IN1_X 0
#define PINO_IN2_X 0
#define PINO_IN3_X 0
#define PINO_IN4_X 0
#define PINO_CURSOMAXIMO_X 3
#define PINO_CURSOMINIMO_X 4
#define ESTADO_FIM_DE_CURSO_X LOW
#define MILIMETROS_POR_VOLTA_X 8
#define LIMITE_MAXIMO_X 10
#define SIG_PERIODO_MAXIMO_X 600
#define SIG_PERIODO_MINIMO_X 100
#define SIG_DECLIVIDADE_X 0.1

#define MODO_PASSO_Y 1
#define PINO_ENABLE_Y 15
#define PINO_RESET_Y 16
#define PINO_SLEEP_Y 17
#define PINO_M0_Y 18
#define PINO_M1_Y 19
#define PINO_M2_Y 20
#define PINO_PASSO_Y 34
#define PINO_DIRECAO_Y 44
#define PINO_IN1_Y 0
#define PINO_IN2_Y 0
#define PINO_IN3_Y 0
#define PINO_IN4_Y 0
#define PINO_CURSOMAXIMO_Y 8
#define PINO_CURSOMINIMO_Y 9
#define ESTADO_FIM_DE_CURSO_Y LOW
#define MILIMETROS_POR_VOLTA_Y 8
#define LIMITE_MAXIMO_Y 10
#define SIG_PERIODO_MAXIMO_Y 600
#define SIG_PERIODO_MINIMO_Y 100
#define SIG_DECLIVIDADE_Y 0.1

String leitura;
int x = 0, y = 0;

byte vetorPassos     [8] = {    9,    1,    3,    2,    6,    4,   12,    8};
//byte vetorPassosHexa [8] = { 0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};
//byte vetorPassosByte [8] = {B1001,B0001,B0011,B0010,B0110,B0100,B1100,B1000};

Pino *pino13;

Pino *pinoEnableX, *pinoResetX, *pinoSleepX;
Pino *pinoM0X, *pinoM1X, *pinoM2X;
Pino *pinoPassoX, *pinoDirecaoX;
Pino *pinoIN1X, *pinoIN2X, *pinoIN3X, *pinoIN4X;
Pino *cursoMaximoX, *cursoMinimoX;
Sigmoidal *sigmoidalX;
Driver *driverX;
Eixo *eixoX;


Pino *pinoEnableY, *pinoResetY, *pinoSleepY;
Pino *pinoM0Y, *pinoM1Y, *pinoM2Y;
Pino *pinoPassoY, *pinoDirecaoY;
Pino *pinoIN1Y, *pinoIN2Y, *pinoIN3Y, *pinoIN4Y;
Pino *cursoMaximoY, *cursoMinimoY;
Sigmoidal *sigmoidalY;
Driver *driverY;
Eixo *eixoY;

void inicializarEixos(Driver::TipoAcionamento tp);
void interpretarComandos(String comando);
void movimentarMesa(String coordenada);
void escolherModoPasso(Eixo *e, String modoPasso);

void setup(){
	Serial.begin(9600);
	pino13 = new Pino(Pino::DIGITAL, 13, OUTPUT, LOW);
	inicializarEixos(Driver::SOFTWARE);
	delay(3000);
}

void loop(){

}

void serialEvent(){
	if(Serial.available() > 0){
		leitura = Serial.readStringUntil('\n');
		if(!leitura.equals("")){
			interpretarComandos(leitura);
			leitura = "";
		}
	}
}

void inicializarEixos(Driver::TipoAcionamento tp){
	pinoEnableX  = new Pino(Pino::DIGITAL, PINO_ENABLE_X, OUTPUT, LOW);
	pinoResetX   = new Pino(Pino::DIGITAL, PINO_RESET_X, OUTPUT, LOW);
	pinoSleepX   = new Pino(Pino::DIGITAL, PINO_SLEEP_X, OUTPUT, LOW);
	pinoM0X      = new Pino(Pino::DIGITAL, PINO_M0_X, OUTPUT, LOW);
	pinoM1X      = new Pino(Pino::DIGITAL, PINO_M1_X, OUTPUT, LOW);
	pinoM2X      = new Pino(Pino::DIGITAL, PINO_M2_X, OUTPUT, LOW);
	cursoMaximoX = new Pino(Pino::DIGITAL, PINO_CURSOMAXIMO_X, INPUT, HIGH);
	cursoMinimoX = new Pino(Pino::DIGITAL, PINO_CURSOMINIMO_X, INPUT, HIGH);
	sigmoidalX   = new Sigmoidal(SIG_PERIODO_MAXIMO_X, SIG_PERIODO_MINIMO_X, SIG_DECLIVIDADE_X);

	pinoEnableY  = new Pino(Pino::DIGITAL, PINO_ENABLE_Y, OUTPUT, LOW);
	pinoResetY   = new Pino(Pino::DIGITAL, PINO_RESET_Y, OUTPUT, LOW);
	pinoSleepY   = new Pino(Pino::DIGITAL, PINO_SLEEP_Y, OUTPUT, LOW);
	pinoM0Y      = new Pino(Pino::DIGITAL, PINO_M0_Y, OUTPUT, LOW);
	pinoM1Y      = new Pino(Pino::DIGITAL, PINO_M1_Y, OUTPUT, LOW);
	pinoM2Y      = new Pino(Pino::DIGITAL, PINO_M2_Y, OUTPUT, LOW);
	cursoMaximoY = new Pino(Pino::DIGITAL, PINO_CURSOMAXIMO_Y, INPUT, HIGH);
	cursoMinimoY = new Pino(Pino::DIGITAL, PINO_CURSOMINIMO_Y, INPUT, HIGH);
	sigmoidalY   = new Sigmoidal(SIG_PERIODO_MAXIMO_Y, SIG_PERIODO_MINIMO_Y, SIG_DECLIVIDADE_Y);

	if(tp == Driver::HARDWARE){
		pinoPassoX   = new Pino(Pino::DIGITAL, PINO_PASSO_X, OUTPUT, LOW);
		pinoDirecaoX = new Pino(Pino::DIGITAL, PINO_DIRECAO_X, OUTPUT, LOW);
		pinoPassoY   = new Pino(Pino::DIGITAL,PINO_PASSO_Y, OUTPUT, LOW);
		pinoDirecaoY = new Pino(Pino::DIGITAL,PINO_DIRECAO_Y, OUTPUT, LOW);

		driverX = new Driver(
			pinoEnableX,
			pinoResetX,
			pinoSleepX,
			pinoM0X,
			pinoM1X,
			pinoM2X,
			pinoPassoX,
			pinoDirecaoX
		);

		driverY = new Driver(
			pinoEnableY,
			pinoResetY,
			pinoSleepY,
			pinoM0Y,
			pinoM1Y,
			pinoM2Y,
			pinoPassoY,
			pinoDirecaoY
		);
	}else{
		pinoIN1X = new Pino(Pino::DIGITAL, PINO_IN1_X, OUTPUT, LOW);
		pinoIN2X = new Pino(Pino::DIGITAL, PINO_IN2_X, OUTPUT, LOW);
		pinoIN3X = new Pino(Pino::DIGITAL, PINO_IN3_X, OUTPUT, LOW);
		pinoIN4X = new Pino(Pino::DIGITAL, PINO_IN4_X, OUTPUT, LOW);
		pinoIN1Y = new Pino(Pino::DIGITAL, PINO_IN1_Y, OUTPUT, LOW);
		pinoIN2Y = new Pino(Pino::DIGITAL, PINO_IN2_Y, OUTPUT, LOW);
		pinoIN3Y = new Pino(Pino::DIGITAL, PINO_IN3_Y, OUTPUT, LOW);
		pinoIN4Y = new Pino(Pino::DIGITAL, PINO_IN4_Y, OUTPUT, LOW);

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
	}

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

	eixoX->calibrar();
	eixoY->calibrar();
}

void interpretarComandos(String comando){
	comando.toUpperCase();
	if (comando.indexOf("MOVER") > -1) movimentarMesa(comando.substring(comando.indexOf(" ")+1));
	if (comando.indexOf("L") > -1) pino13->setEstado(HIGH);
	if (comando.indexOf("D") > -1) pino13->setEstado(LOW);
	if (comando.indexOf("MODOPASSOX") > -1) escolherModoPasso(eixoX, comando.substring(comando.indexOf(" ")+1));
	if (comando.indexOf("MODOPASSOY") > -1) escolherModoPasso(eixoY, comando.substring(comando.indexOf(" ")+1));
}

void movimentarMesa(String coordenada){
	if(!(coordenada.indexOf(",") == -1)){
		x = coordenada.substring(0,coordenada.indexOf(",")).toDouble();
		y = coordenada.substring(coordenada.indexOf(",")+1).toDouble();
		if(x >= 0 && x <= 10 && y >= 0 && y <= 10){
			if(x != eixoX->getPosicao()) {
				eixoX->rotacionarPara(x);
				Serial.println(eixoX->getPosicao());
			}
			if(y != eixoY->getPosicao()) {
				eixoY->rotacionarPara(y);
				Serial.println(eixoY->getPosicao());
			}
		}
	}
}

void escolherModoPasso(Eixo *e, String modoPasso){
	switch(modoPasso.toInt()){
		case  1: e->getDriver()->setarModoPasso( 1,0);break;
		case  2: e->getDriver()->setarModoPasso( 2,4);break;
		case  4: e->getDriver()->setarModoPasso( 4,2);break;
		case  8: e->getDriver()->setarModoPasso( 8,6);break;
		//case 16: e->getDriver()->setarModoPasso(16,1);break;
		//case 32: e->getDriver()->setarModoPasso(32,7);break;
	}
}
