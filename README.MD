# Detect Color Difference with TCS3200

Table of Contents
- [Theory](#theory)
- [Circuit](#circuit)
- [Operation](#operation)
- [Conclusion](#conclusion)

---

## Theory

Color difference is the seperation between two colors. Common definitions of color difference are distance within a color space. The starndard means of determining distance is Euclidean distance.

$distance=\sqrt{(R2-R1)^2+(G2-G1)^2+(B2-B1)^2}$

![Alt Text](./image/color_space.png)

## Circuit

The color sensor TCS3200 is used to sample color information. The output is a square wave with frequency directly proportional to light intensity.

There are four types of photodiodes on the chip: red, green, blue, and clear. S2 and S3 pins are used to select which group of photodiodes are active.

The MCU is an Atmega328P. Output from TCS3200 is connected to an external interrupt pin (PB0) on MCU. This pin is configured to generate a interrupt request on rising edge of input signal.

![Alt Text](./image/tcs3200.svg)

## Operation

Upon initialization, S2 and S3 pins are low, thus red photodiodes are active. The 16 bit timer 1 on MCU is configured to generate a interrupt request at every 3332 &mu;s. 

Inside this timer interrupt service routine, S2 and S3 pins are toggled to select a different group of photodioes. Before toggling, pulse counts accumulated inside INT0 ISR are read out and set to zero. Since variable pulse_count is read out every 3332&mu;s, the sampling fequency is 300.12 Hz. And since red, green, blue channels are sampled in turn, the sampling frequency for all three channels is 100.04 Hz. This is fast enough to capture rapid transition in color. For example, it is suitable for detecting the blinking sequence of a RGB led.

A finite state machine controls the channel switching.

<pre>
ISR(TIMER1_COMPA_vect) {
    fsm();
}
</pre>

Inside the INT0 ISR, the number of pulses is accumulated.

 <pre>
 ISR(INT0_vect) {
    pulse_count++;
 }
 </pre>

With proper distance between sensor and light source and output level set, the value of variable pulse_count will fall within the range of {0, 255}. The integer values for three color channels are used to calculate the Euclidean distance in RGB color space.

## Conclusion

The algorithm is robust and efficient enough to be implemented on MCU like Atmega328P running at 16MHz.