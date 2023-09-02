# Detect Color Difference with TCS3200

Table of Contents
- [Theory](#theory)
- [Circuit](#circuit)
- [Project Structure](#project-structure)
- [Configuration](#configuration)
- [Usage](#usage)
- [Customization](#customization)
- [Resources](#resources)
- [Contributing](#contributing)
- [License](#license)

---

## Theory

Color difference is the seperation between two colors. Common definitions of color difference are distance within a color space. The starndard means of determining distance is Euclidean distance.

$distance=\sqrt{(R2-R1)^2+(G2-G1)^2+(B2-B1)^2}$

![Alt Text](./image/color_space.png)

## Circuit

The color sensor TCS3200 is used to sample color information. The output is a square wave with frequency directly proportional to light intensity.

There four types of photodiodes on the chip: red, green, blue, and clear. S2 and S3 pins are used to select which group of photodiodes are active.

The MCU is an Atmega328P. Output from TCS3200 is connected to external interrupt pin (PB0) on MCU. The pin is configured to generate a interrupt request on rising edge of input signal.

![Alt Text](./image/tcs3200.svg)

## Operation

 <pre>
 '''
 ISR(INT0_vect) {
    ...
 }
 '''
 </pre>

## Configuration

List any configuration files or settings that need to be modified for specific projects. Mention how to modify these files for customization.