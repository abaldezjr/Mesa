#ifndef SIGMOIDAL_H_INCLUDED
#define SIGMOIDAL_H_INCLUDED

#include <math.h>

#define E 2.71828182845904523536

class Sigmoidal {

	public:
		~Sigmoidal(void){}

		Sigmoidal(double pmax, double pmin, double de){
			this->periodoMaximo = pmax;
			this->periodoMinimo = pmin;
			this->amplitude = this->periodoMaximo - this->periodoMinimo;
			this->vetorAceleracao = 1;
			this->declividade = de;
			this->xMedio = this->calculoDoXMedio();
		}

		double calculoDoXMedio(void) const {
			return ((-1)*log( this->amplitude / ((this->periodoMaximo * 0.99999) - this->periodoMinimo) -1)) / this->declividade;
		}

		void setVetorAceleracao(int va){
			this->vetorAceleracao = va;
		}

		void setPeriodoMaximo(double pmax){
			this->periodoMaximo = pmax;
		}

		void setPeriodoMinimo(double pmin){
			this->periodoMinimo = pmin;
		}

		void setDeclividade(double de){
			this->declividade = de;
		}

		double getPeriodoMaximo(void) const {
			return this->periodoMaximo;
		}

		double getPeriodoMinimo(void) const {
			return this->periodoMinimo;
		}

		int getVetorAceleracao(void) const {
			return this->vetorAceleracao;
		}

		double getAmplitude(void) const {
			return this->amplitude;
		}

		double getDeclividade(void) const {
			return this->declividade;
		}

		double getXMedio(void) const {
			return this->xMedio;
		}

		double calculoDoInstante(int i) const {
			return ( this->amplitude / ( 1 + pow( E , (this->vetorAceleracao * (this->declividade * (i - this->xMedio)))))) + this->periodoMinimo;
		}

	private:
		double periodoMaximo    { 600 },
			   periodoMinimo    { 100 },
			   amplitude        { this->periodoMaximo - this->periodoMinimo  },
			   declividade      { 0 },
			   xMedio           { 0 };
		int    vetorAceleracao  { 1 };
};
#endif
