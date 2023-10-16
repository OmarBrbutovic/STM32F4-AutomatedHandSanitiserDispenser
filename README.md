<a name="readme-top"></a> <br />

<div align="center">
  <h1 align="center">Automated Hand Sanitizer Dispenser with STM32F4</h1>
</div>

# About The Project

The PMPS Automated Hand Sanitizer Dispenser is a hardware project built on the STM32F4 microcontroller platform. This project combines various sensors and actuators to create an automated hand sanitizer dispenser with intelligent features.

### Components Used

- **STM32F407VG microcontroller**
- **TTL-USB module**
- **Ultrasonic Sensor (HC-SR04)**
- **Water level sensor (K-0135)**
- **DC pump (BI0002051)**
- **Relay (SRD-05VDC-SL-C)**
- **LEDs**

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Wiring Diagram

<div align="center">
  <img src="pictures\Diagram.png" alt="Diagram" width=800>
</div>

_The wiring diagram illustrates the connections between various components, helping with the hardware setup._

<div align="center">

| Pin  | Function             |
| ---- | -------------------- |
| PA2  | RX                   |
| PA3  | TX                   |
| PC5  | Water level sensor S |
| PD10 | Relay IN1            |
| PD12 | HCSR-04 Trigger      |
| PD13 | Echo                 |
| PE0  | LED Green            |
| PE1  | LED Yellow           |
| PE2  | LED Red              |

</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## STM32F4 Pinout

<div align="center">
  <img src="pictures\STM32F4.png" alt="Diagram" width=800>
</div>

_The STM32 pinout diagram specifies the connections of various peripherals and GPIO pins used in the project._

<p align="right">(<a href="#readme-top">back to top</a>)</p>
