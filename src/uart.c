#include <avr/io.h>

#include "uart.h"

#define BAUD_0      1000000
#define UBRR_0      ((F_CPU)/(BAUD_0*8UL)-1)    // see spec sheet: buad rate generator        

void uart0_init() {
    UBRR0L = (uint8_t)UBRR_0;                   // write lower byte
    UBRR0H = (uint8_t)(UBRR_0 >> 8);            // write higher byte

    UCSR0A |= (1<<U2X0);                        // U2X0 bit seems to be 1 by default on Pro Mini 3.3V 8Mhz
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);      // enable transmitter & receiver
    UCSR0B |= (1 << RXCIE0);                    // enable RX Complete Interrupt
}

void uart0_transmit(uint8_t data) {
    while (!(UCSR0A & (1<<UDRE0)));             // wait for empty transmit buffer
    UDR0 = data;                                // put data into buffer
}

void uart0_puts(const char* message) {
    while (*message) {
        uart0_transmit(*message++);
    }
}