#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#define TIMER1   F_CPU / 1024
//#define IFQ_FREQ       5  

uint8_t state = 0;
uint16_t led = -1;
int port[] = {PB1, PB2, PB3, PB4, PB5};

// Rotina de tratamento da interrupção PCINT0
ISR(PCINT0_vect)
{
    if (PINB & (1 << PB0)) {       // lê PB0  
        if(state){ //se state=1 
            state = 0; //desativa
        }
        else{ //se state=1
            state = 1; //ativa      
        }      
    } 
}

int main(void) {
	
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);    // seta PB1 a PB5 como saídas
    DDRB &= ~(1 << PB0);           // seta PB0 como entrada
    PORTB &= ~(1 << PB0);          // desabilita pull-up de PB1

    PCICR |= (1 << PCIE0);         // habilita vetor de interrupção para PB7 .. PB0
    PCMSK0 |= (1 << PCINT0);       // habilita interrupção para PB1

    sei(); // habilita interrupções
    
    while (1) {
        if(state){ // se state=1  -> ativa irrigação
            led++;

            // piscar leds
            for(int i=led; i < 5; i++){
               PORTB ^= (1 << port[i]);               
            }
            _delay_ms(500);
            for(int i=led; i < 5; i++){
               PORTB ^= (1 << port[i]);  
            }
    
            _delay_ms(3000);    // delay de 3 segundos      

            PORTB ^= (1 << port[led]);  // liga o led      
                
            if(PINB & (1 << PB5)){   // se o PB5 está ligado, desativa irrigação
                state = 0;
            }  
        } 
        else{ //se state=0  -> desativa irrigação        
            if(PINB & (1 << PB1)){   // se PB1 está ligado
                _delay_ms(5000);     // delay de 5 segundos
                PORTB ^= (1 << port[led]);  // desliga o led
                led--;             
            }                   
        }     
    }
}