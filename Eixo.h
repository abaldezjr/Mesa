#ifndef EIXO_H_INCLUDED
#define EIXO_H_INCLUDED

#include "Arduino.h"
#include "Pino.h"
#include "Sigmoidal.h"
#include "Driver.h"

class Eixo {

	public:
		~Eixo(void){}
		Eixo(Driver *driver, Pino *pinoCursoMaximo, Pino *pinoCursoMinimo, Sigmoidal *sigmoidal){
			this->driver = driver;
			this->pinoCursoMaximo = pinoCursoMaximo;
			this->pinoCursoMinimo = pinoCursoMinimo;
			this->sigmoidal = sigmoidal;
		}

		Eixo(Driver *driver, byte milimetrosPorVolta, bool estadoFimDeCurso, Pino *pinoCursoMaximo, Pino *pinoCursoMinimo, Sigmoidal *sigmoidal){
			this->driver = driver;
			this->micrometrosPorVolta = milimetrosPorVolta * 1000;
			this->estadoFimDeCurso = estadoFimDeCurso;
			this->pinoCursoMaximo = pinoCursoMaximo;
			this->pinoCursoMinimo = pinoCursoMinimo;
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

		void rotacionarPara(int coordenada){
			this->rotacionarPasso(
				(coordenada*1000 - this->posicao) > 0? Driver::HORARIO: Driver::ANTIHORARIO,
				(fabs(coordenada*1000 - this->posicao) * this->driver->getPassosPorVolta())/this->micrometrosPorVolta
			);
		}

		void calibrar(){
			this->driver->setDirecao(Driver::ANTIHORARIO);
			this->posicao = 0;
			while(this->pinoCursoMinimo->getEstado()){
				this->driver->passo();
				delayMicroseconds(this->sigmoidal->getPeriodoMaximo());
			}
		}

	private:
		Driver *driver;
		Pino *pinoCursoMaximo, *pinoCursoMinimo;
		Sigmoidal *sigmoidal;

		int posicao = 0;
		int micrometrosPorVolta = 8000;
		bool estadoFimDeCurso = LOW;

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
			for(int i = 0, iSig = 0; i < passos; i++, i <= ((passos * this->driver->getModoPasso()) / 2)? iSig++: iSig--){
				if(this->podeMovimentar()){
					this->driver->passo();
					delayMicroseconds(this->sigmoidal->calculoDoInstante(iSig));
					this->posicao += direcao * (this->micrometrosPorVolta / this->driver->getPassosPorVolta());
				}else{
					i = passos;
					delay(3000);
					this->calibrar();
				}
			}
		}
};
#endif
