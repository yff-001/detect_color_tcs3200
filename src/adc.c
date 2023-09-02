#include <avr/io.h>
#include "adc.h"

void init_adc() {
    ADMUX |= (1 << REFS0);              // AVCC AREF
    // ADMUX |= (1 << ADLAR);              // left aligned
    ADMUX |= (1 << MUX0);
    
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1);// f/64
    
    ADCSRA |= (1 << ADATE);             // enable auto trigger
    ADCSRA |= (1 << ADIE);              // enable interrupt
}

void start_adc() {
    ADCSRA |= (1 << ADEN);
    ADCSRA |= (1 << ADSC);
}

void stop_adc() {
    ADCSRA &= ~(1 << ADEN);
}