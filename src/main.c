#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adc.h"
#include "timers.h"
#include "uart.h"

#define r_channel   0x00
#define g_channel   0x03
#define b_channel   0x01
#define TRESHOLD    0.1

volatile uint8_t    color_data_ready = 0;
volatile uint8_t    color_channel = 0;
volatile uint8_t    color_value[3];
volatile uint16_t   color_pulse_count = 0;

void init_system();

int main() {
    init_system();

    /* target color */
    uint8_t r_t = 100;
    uint8_t g_t = 100;
    uint8_t b_t = 100;

    for (;;) {
        if (color_data_ready) {
            float difference = sqrt((color_value[0]-r_t)*(color_value[0]-r_t)+(color_value[1]-g_t)*(color_value[1]-g_t)+(color_value[2]-b_t)*(color_value[2]-b_t));
            if (difference <= TRESHOLD) {
                uart0_puts("Colors match!\r\n");
            }
            else {
                uart0_puts("Colors do not match!\r\n");
            }
        }
    }
}

void init_system() {
    uart0_init();
    init_timer1();

    /* init TCS3200 */
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
    PORTB |= (1 << PB4) | (1 << PB5);                                       // set output frequency at 100%

    EICRA |= (1 << ISC01) | (1 << ISC00);                                   // rising edge on INT0 generates an interrupt request
    EIMSK |= (1 << INT0);                                                   // enable INT0

    sei();
}

void switch_color_channel(uint8_t color) {
    /* s2 pin on tcs3200 */
    if (color&0x02) {
        PORTB |= (1 << PB2);
    }
    else {
        PORTB &= ~(1 << PB2);
    }
    /* s3 pin on tcs3200 */
    if (color&0x01) {
        PORTB |= (1 << PB3);
    }
    else {
        PORTB &= ~(1 << PB3);
    }
}

ISR(TIMER1_COMPA_vect) {
    if (color_channel == 1) {
        color_value[0] = color_pulse_count;                                 // update red value
        switch_color_channel(g_channel);
        color_channel = 2;
    }
    else if (color_channel == 2) {
        color_value[1] = color_pulse_count;                                 // update green value
        switch_color_channel(b_channel);
        color_channel = 3;
    }
    else if (color_channel == 3) {
        color_value[2] = color_pulse_count;                                 // update blue value
        switch_color_channel(r_channel);
        color_channel = 1;

        color_data_ready = 1;
    }
    else {
        color_channel = 1;
        switch_color_channel(r_channel);
    }

    color_pulse_count = 0;
}

ISR(INT0_vect) {
    color_pulse_count++;                                                    // accumulate number of pulses
}