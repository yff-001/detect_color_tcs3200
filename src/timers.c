#include <avr/io.h>

#include "timers.h"

/* CTC */
void init_timer1() {
    TCCR1B |= (1 << WGM12);                     // phase and frequency correct, OCR1A as TOP
    TCCR1B |= (1 << CS10);                      // f=clk_io/64
    TCCR1B |= (1 << CS11);
    TIMSK1 |= (1 <<OCIE1A);                     // enable OUTPUT COMPARE A MATCH INTERRUPT
    OCR1A = 832;                                // 300.12 Hz
}
