//-----------------------------------------------
// Universidad del Valle de Guatemala
// IE2023: Programacion de Microcontroladores
// ContadorBinario.c
// Autor: Ian Anleu Rivera
// Proyecto: Prelab 4
// Hardware: ATMEGA328P
// Creado: 07/04/2024
// Ultima modificacion: 08/04/2024
//-----------------------------------------------

#define F_CPU 1600000 //Frecuencia es 16Mhz
#include <avr/io.h>
#include <util/delay.h> // Lib para delays
#include <stdint.h> //Lib para enteros

void setup(void);

int main(void)
{
	setup();
	//Otros Registros
	uint8_t contador = 0;
	uint8_t estadosprevios = 0xFF;
	uint8_t estadosactuales = 0;
	
    while (1) 
    {
		
		// Antirrebote
		estadosactuales = PINB;
		if (estadosprevios!=estadosactuales)
		{
			_delay_ms(10); //Lectura nueva
			estadosactuales = PINB;
			if (estadosprevios!=estadosactuales)
			{ // Si son distintos determino los bits distintos
				if(!(PINB & 0x02)) //Si es PB1 se resta al contador
				{
					contador--;
				}//Fin Resta
				if(!(PINB & 0x01)){ // Si es PB0 se añade al contador
					contador++;
				}// Fin Suma
				estadosprevios = estadosactuales;
			}//Fin Botones
		} //Fin Antirrebote
		PORTD = contador; //Output en PORTD
    }// Fin Main Loop
}

void setup(void){
	//ENTRADAS Y SALIDAS -----------------------
	//Entradas
	DDRB &= 0x00;
	PORTB |= 0xFF; //Pullups
	
	//Salidas
	DDRD |= 0xFF;
	UCSR0B = 0;
	PORTD = 0;
}