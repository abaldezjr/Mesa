#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

#include "Arduino.h"
#include "Pino.h"

class Driver {
    
  public:
    typedef enum {
      WAVESTEP    = 0,
      FULLSTEP    = 1,
      HALFSTEP    = 2
    } TipoPasso;

    typedef enum {
      ANTIHORARIO = 0,
      HORARIO     = 1
    } Direcao;

    ~Driver(void){}
    Driver(Pino* pinoIN1, Pino* pinoIN2, Pino* pinoIN3, Pino* pinoIN4, byte *vetorPassos){
      this->setarModoPasso(1,0);
      this->tipoPasso = Driver::WAVESTEP;
      this->pinoIN1 = pinoIN1;
      this->pinoIN2 = pinoIN2;
      this->pinoIN3 = pinoIN3;
      this->pinoIN4 = pinoIN4;
      this->vetorPassos = vetorPassos;
      this->direcao = Driver::HORARIO;
    }
    
    Driver(TipoPasso tipoPasso, Pino* pinoEnable, Pino* pinoReset, Pino* pinoSleep, Pino* pinoM0, Pino* pinoM1, Pino* pinoM2,
    Pino* pinoIN1, Pino* pinoIN2, Pino* pinoIN3, Pino* pinoIN4, byte *vetorPassos){
      this->setarModoPasso(1,0);
      this->tipoPasso = tipoPasso;
      this->pinoEnable = pinoEnable;
      this->pinoReset = pinoReset;
      this->pinoSleep = pinoSleep;
      this->pinoM0 = pinoM0;
      this->pinoM1 = pinoM1;
      this->pinoM2 = pinoM2;
      this->pinoIN1 = pinoIN1;
      this->pinoIN2 = pinoIN2;
      this->pinoIN3 = pinoIN3;
      this->pinoIN4 = pinoIN4;
      this->direcao = Driver::HORARIO;
      this->vetorPassos = vetorPassos;
    }

    void setDirecao(Direcao d){ 
      this->direcao = d; 
    }

    Pino* getPinoEnable(void) const {
      return this->pinoEnable; 
    }

    Pino* getPinoReset(void) const { 
      return this->pinoReset; 
    }

    Pino* getPinoSleep(void) const { 
      return this->pinoSleep; 
    }

    Direcao getDirecao(void) const {
      return this->direcao; 
    }

    byte getModoPasso(void) const { 
      return this->modoPasso; 
    }

    int getPassosPorVolta(void) const { 
      return this->passosPorVolta; 
    }

    void setarModoPasso(int mp,byte b){
      this->modoPasso = mp;
      this->passosPorVolta = 200 * this->modoPasso;
      this->pinoM0->setEstado((b >> 2) & 1);
      this->pinoM1->setEstado((b >> 1) & 1);
      this->pinoM2->setEstado((b >> 0) & 1);
      this->passoAtual = TipoPasso::FULLSTEP? 1 :0;
    }

    void setarEstados(byte b){
      this->pinoIN1->setEstado((b >> 0) & 1);
      this->pinoIN2->setEstado((b >> 1) & 1);
      this->pinoIN3->setEstado((b >> 2) & 1);
      this->pinoIN4->setEstado((b >> 3) & 1);
    }

    void passo(void){
      switch(this->tipoPasso){
        case TipoPasso::WAVESTEP: this->executarPasso(2,0,6); break;
        case TipoPasso::FULLSTEP: this->executarPasso(2,1,7); break;
        case TipoPasso::HALFSTEP: this->executarPasso(1,0,7); break;  
      }
    }
    
  private:
    Pino *pinoEnable;
    Pino *pinoReset;
    Pino *pinoSleep;
    Pino *pinoM0;
    Pino *pinoM1;
    Pino *pinoM2;
    Pino *pinoIN1;
    Pino *pinoIN2;
    Pino *pinoIN3;
    Pino *pinoIN4;

    TipoPasso tipoPasso;
    Direcao direcao;
    int passoAtual = 0;
    int modoPasso;
    int passosPorVolta;
    byte *vetorPassos;

    void executarPasso(int contador, int limiteInferior, int limiteSuperior){
      this->setarEstados(this->vetorPassos[ this->passoAtual ]);
      this->passoAtual = this->direcao == Direcao::HORARIO? this->passoAtual + contador :this->passoAtual - contador;
      if(this->passoAtual >= limiteSuperior) this->passoAtual = limiteInferior;
      if(this->passoAtual  < limiteInferior) this->passoAtual = limiteSuperior;
    }

};
#endif
