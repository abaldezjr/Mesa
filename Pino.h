#ifndef PINO_H_INCLUDED
#define PINO_H_INCLUDED

#include "Arduino.h"

class Pino {

	public:
		enum TipoPino {
			ANALOGICO  = 0,
			DIGITAL    = 1,
		};

		~Pino(void){}
		Pino(TipoPino t, byte n, byte m, unsigned int e){
			this->setTipo(t);
			this->setNumero(n);
			this->setModo(m);
			this->setEstado(e);
		}

		void setModo(byte m){
			this->modo = m;
			pinMode( this->numero, this->modo);
		}

		void setEstado(int e){
			this->estado = e;
			this->tipo == TipoPino::DIGITAL? digitalWrite(this->numero, this->estado): analogWrite(this->numero, this->estado);
		}

		TipoPino getTipo(void) const {
			return this->tipo;
		}

		byte getNumero(void) const {
			return this->numero;
		}

		byte getModo(void) const {
			return this->modo;
		}

		unsigned int getEstado(void){
			this->estado = this->tipo == TipoPino::DIGITAL? digitalRead(this->numero): analogRead(this->numero);
			return this->estado;
		}

	private:
		TipoPino tipo;
		byte numero;
		byte modo;
		int estado;

		void setTipo(TipoPino t){
			this->tipo = t;
		}

		void setNumero(byte n){
			this->numero = n;
		}
};
#endif
