/*
 * Boostiny.cpp
 *
 * Created: 05/01/2020 01:17:11
 * Author : Lucas
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int e[2];
int u = 0;
int integrative = -256000;


ISR(ADC_vect){
	e[1] = e[0];
	e[0] = 191 - ADCH;
	
	integrative += e[0];
	if(integrative > 0)
		integrative = 0;
	if(integrative < -2560)
		integrative = -2560;
	u =-( 5*e[0] + integrative/10);
	
	//u = -2*e[0];
	if(u<50) u = 50;
	else if(u>255) u = 255;
	
	OCR1B = (unsigned char) u;
	//OCR1B = (unsigned char)u;
	//if(OCR1B) OCR1B = 0;
	//else OCR1B = 255;
	//if(OCR1B>250)
	//	OCR1B = 0;
	//else
	//	OCR1B = 255;
	//OCR1B = TCNT0;
	//OCR1B = ADCH;
	//OCR1B++;
}

int main(void)
{
	for(volatile unsigned int j=0; j<60000; j++); //aguarda alguns milissegundos
    /* Replace with your application code */
	
	PLLCSR |= 1<<1; //PLLE: PLL Enable
	while(!(PLLCSR & 1)); //Wait PLOCK
	PLLCSR |= 1<<2; //PCK Enable
	
	//TCCR1 |= (1<<6) | (1<<4);// 01010001//CTC1=0, PWM1A=1(bit 6), COM1A=01(bit 5 e 4)=pwmHighLow, CS1 = 1(PCK)
	
	GTCCR |= (1<<4)|(1<<6);
	//GTCCR |= ; //PWM na COM1B
	DDRB |= (1<<4); //Liga saida COM1B não negada PB4
		PORTB |= (1<<4); //Saída ficará em HIGH (mosfet cortado)

	TCCR1 |=  (1<<0); //ativa clock, com alta velocidade PCK
	OCR1B =255;
	
	////////////////////////////////////////////////////////////////
	////CONFIGURA ADC
	ADMUX |= (2<<6) | (1<<4); //REFS110 internal 2,56V reference
	ADMUX |= (1<<5); //ADLAR
	ADMUX |= 3; //0011 = ADC3(PB3) channel
	
	/*
	ADCSRA |= (1<<7); //ADC Enable
	//ADCSRA |= (1<<6); //ADSC Start Conv
	ADCSRA |= (1<<5); //ADC Auto Trigger Enable
	//ADCSRA |= (1<<4); //ADC Interrupt Flag
	ADCSRA |= (1<<3); //ADC Interrupt Enable
	ADCSRA |= (6<<0); //ADC Prescaller x64 -> 16MHz / 64 = 250kHz
	*/
	//ADCSRB |= 4; //COM0A trigger
	//freerunning
	ADCSRA = 0B10101110;
	ADCSRB = 0;

	
	GTCCR |= 0;
	TCCR0A = 0; //CTC=2
	OCR0A = 255-1;
	TCCR0B = 5; //prescaller 16MHz/1024/255 = 61Hz
	
	sei();
	
	//ADCH
	//ADCL
	
	//DDRB |= (1<<4);
	ADCSRA |= (1<<6); //ADSC Start Conv
	
	
	
    while (1) 
    {
		//OCR1B = TCNT0;		//PORTB ^= (1<<4);
		//ADCSRA |= (1<<6);
		//PORTB ^= 1<<4;
    }
}

