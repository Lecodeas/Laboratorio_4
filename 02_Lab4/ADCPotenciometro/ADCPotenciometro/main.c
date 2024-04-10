/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 ADCPotenciometro.c
 Autor: Ian Anleu Rivera
 Proyecto: Lab 4
 Hardware: ATMEGA328P
 Creado: 08/04/2024
 Ultima modificacion: 08/04/2024
    -----------------------------------------------
*/

//HEADER FILES
#define F_CPU 1600000 //Frecuencia es 16Mhz
#include <avr/io.h>
#include <util/delay.h> // Lib para delays
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones
 
//VARIABLES GLOBALES
//Otras variables
uint8_t contador = 0;
uint8_t estadosprevios = 0xFF;
uint8_t estadosactuales = 0;
uint8_t valoradc = 0;

//"Tabla" para 7 segmentos
uint8_t Tabla7seg[] = {0x77, 0x11, 0x6B, 0x3B, 0x1D, 0x3E, 0x7E, 0x13, 0x7F, 0x3F, 0x5F, 0x7C, 0x66, 0x79, 0x6E, 0x4E};

//PROTOTIPOS DE FUNCIÓN
void setup(void);
void setupADC(void);

//INTERRUPCIONES

//ADC 
ISR(ADC_vect){
	valoradc = ADCH; // Almacenar valor
	ADCSRA |= (1<<ADIF); // Apagar bandera
	ADCSRA |= (1<<ADSC); //Volver a iniciar
}

//PRINCIPAL
int main(void)
{
	cli();
	setup();
	setupADC();
	sei();
	
	while (1)
	{
		//Contador Binario --------------------------------------
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
		
		//Despliegue secuencial ---------------------------------
		_delay_ms(70); // Cambio a 7 seg. 1
		PORTD = Tabla7seg[(valoradc & 0xF0)>>4];
		PORTC = 0b00000001;
		_delay_ms(70); // Cambio a 7 seg. 1 
		PORTD = Tabla7seg[valoradc & 0x0F];
		PORTC = 0b00000010;
		_delay_ms(70); // Contador
		PORTD = contador;
		PORTC = 0b00000100;
		
		
	}// Fin Main Loop
}

// OTRAS FUNCIONES
void setup(void){
	//ENTRADAS Y SALIDAS -----------------------
	//Entradas (PORTB será también salida)
	DDRB &= 0xF0;
	PORTB |= 0xFF; //Pullups (Y estado incial de muxado)
	
	//Salidas
	DDRD |= 0xFF; //Contador Binario
	UCSR0B = 0;
	PORTD = 0;
	
	DDRC |= 0xFF; //7 Segmentos
	PORTD = 0;
}

void setupADC(void){
	ADMUX = 0;
	ADMUX |= (1<<REFS0)|(1<<ADLAR)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0); //Vcc ref | Just. Izq. | Mux ADC7
	ADCSRA = 0;
	ADCSRA |= (1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Int. En | Presc. 128 (125kHz)
	ADCSRA |= (1<<ADEN); // Enable
}