#ifndef PINO_H_INCLUDED
#define PINO_H_INCLUDED

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

		/**
		 * @brief Seta o valor do modo do pino utilizando a função pinMode.
		 * @param byte m: Modo do pino
		 * @return void
		**/
		void setModo(byte m){
			this->modo = m;
			pinMode( this->numero, this->modo);
		}

		/**
		 * @brief Seta o valor do estado do pino conforme o atributo tipo(ANALOGICO, DIGITAL).
		 * @param unsigned int e: Estado do pino
		 * @return void
		**/
		void setEstado(unsigned int e){
			this->estado = e;
			this->tipo == TipoPino::DIGITAL? digitalWrite(this->numero, this->estado): analogWrite(this->numero, this->estado);
		}

		/**
		 * @brief
		 * @param
		 * @return
		**/
		TipoPino getTipo(void) const {
			return this->tipo;
		}

		/**
		 * @brief
		 * @param
		 * @return
		**/
		byte getNumero(void) const {
			return this->numero;
		}

		/**
		 * @brief
		 * @param
		 * @return
		**/
		byte getModo(void) const {
			return this->modo;
		}

		/**
		 * @brief
		 * @param
		 * @return
		**/
		unsigned int getEstado(void){
			this->estado = this->tipo == TipoPino::DIGITAL? digitalRead(this->numero): analogRead(this->numero);
			return this->estado;
		}

	private:
		TipoPino tipo        { TipoPino::DIGITAL };
		byte numero          { 0 };
		byte modo            { OUTPUT };
		unsigned int estado  { LOW };

		/**
		 * @brief
		 * @param
		 * @return
		**/
		void setTipo(TipoPino t){
			this->tipo = t;
		}

		/**
		 * @brief
		 * @param
		 * @return
		**/
		void setNumero(byte n){
			this->numero = n;
		}
};
#endif
