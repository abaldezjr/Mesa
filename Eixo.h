#ifndef EIXO_H_INCLUDED
#define EIXO_H_INCLUDED

#include "Arduino.h"
#include "Pino.h"
#include "Sigmoidal.h"
#include "Driver.h"
#include "Eixo.h"

class Eixo {

  public:
    ~Eixo(void){}
    Eixo(Driver *driver, Pino *pinoCursoMaximo, Pino *pinoCursoMinimo, Sigmoidal *sigmoidal){
      this->driver = driver;
      this->milimetrosPorVolta = 8;
      this->estadoFimDeCurso = LOW;
      this->pinoCursoMaximo = pinoCursoMaximo;
      this->pinoCursoMinimo = pinoCursoMinimo;  
      this->posicao = 0;  
      this->sigmoidal = sigmoidal;
    }

    Eixo(Driver *driver, byte milimetrosPorVolta, bool estadoFimDeCurso, Pino *pinoCursoMaximo, Pino *pinoCursoMinimo, Sigmoidal *sigmoidal){
      this->driver = driver;
      this->milimetrosPorVolta = milimetrosPorVolta;
      this->estadoFimDeCurso = estadoFimDeCurso;  
      this->pinoCursoMaximo = pinoCursoMaximo;
      this->pinoCursoMinimo = pinoCursoMinimo;
      this->posicao = 0;  
      this->sigmoidal = sigmoidal;
    }

    void setPosicao(double p){ 
      this->posicao = p; 
    }

    Driver* getDriver(void) const { 
      return this->driver; 
    }

    Pino* getPinoCursoMaximo(void) const{ 
      return this->pinoCursoMaximo; 
    }

    Pino* getPinoCursoMinimo(void) const { 
      return this->pinoCursoMinimo; 
    }

    bool getEstadoFimDeCurso(void) const { 
      return this->estadoFimDeCurso; 
    }

    double getPosicao(void) const { 
      return this->posicao; 
    }

    Sigmoidal* getSigmoidal(void) const { 
      return this->sigmoidal; 
    }

    void rotacionarPara(double coordenada){
      this->rotacionarPasso(
        (coordenada - this->posicao) > 0? Driver::HORARIO: Driver::ANTIHORARIO,
        abs(coordenada - this->posicao) * ( this->driver->getPassosPorVolta() / this->milimetrosPorVolta)
      );
      this->posicao = coordenada;
    }
  
  private:
    Driver *driver;
    Pino *pinoCursoMaximo;
    Pino *pinoCursoMinimo;
    Sigmoidal *sigmoidal;
    
    double posicao;
    int milimetrosPorVolta;
    bool estadoFimDeCurso;

    bool podeMovimentar(void) const {
      return 
      ( this->estadoFimDeCurso && ((this->driver->getDirecao() && !this->pinoCursoMaximo->getEstado()) || 
                                  (!this->driver->getDirecao() && !this->pinoCursoMinimo->getEstado()))) 
                                                ||
      (!this->estadoFimDeCurso && ((this->driver->getDirecao() &&  this->pinoCursoMaximo->getEstado()) || 
                                  (!this->driver->getDirecao() &&  this->pinoCursoMinimo->getEstado())));
    }

    void rotacionarPasso(Driver::Direcao direcao,int passos){ 
      this->driver->setDirecao(direcao);     
      double distancia = 0;
      for(int i = 0, iSig = 0; i < passos; i++, i <= ((passos * this->driver->getModoPasso()) / 2)? iSig++: iSig--){
        if(this->podeMovimentar()){
          distancia+=((double) this->milimetrosPorVolta / this->driver->getPassosPorVolta() / this->driver->getModoPasso());
          this->driver->passo();
          delayMicroseconds(this->sigmoidal->calculoDoInstante(iSig));
          Serial.println(this->sigmoidal->calculoDoInstante(iSig));
        }
      }
      Serial.println("dist:"+ (String) distancia);
    }
};
#endif
