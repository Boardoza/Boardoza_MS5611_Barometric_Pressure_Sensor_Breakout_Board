# Boardoza MS5611 Barometric Pressure Sensor Breakout Board

The **Boardoza MS5611** is a **high-precision barometric pressure sensor module** designed for **altitude measurement, weather monitoring, and industrial pressure sensing**. It integrates the **MS5611-01BA03** digital pressure sensor, providing accurate pressure readings with ultra-low power consumption.  

This module operates with **3.3V or 5V**, making it compatible with a wide range of microcontrollers. It communicates via **I<sup>2</sup>C by default**, but can be switched to **SPI mode** using the **PS pin**. The **CSB, SDO, and PS pins operate at 3.3V**, ensuring easy interfacing with low-power microcontrollers.  

## [Click here to purchase!](https://www.ozdisan.com/maker-ve-iot-urunleri/boardoza/boardoza-modulleri/BOARDOZA-MS5611/1202433)

|Front Side|Back Side|
|:---:|:---:|
| ![ MS5611 Front](./assets/MS5611%20Front.png)| ![ MS5611 Back](./assets/MS5611%20Back.png)|

---

## Key Features

- **High-Precision Barometric Pressure Sensor:** Ideal for altitude measurement, weather monitoring, and UAV applications.  
- **24-bit High-Resolution Output:** Ultra-precise pressure measurement using an integrated 24-bit ADC.  
- **Flexible Power Compatibility:** Operates with **3.3V or 5V**, making it suitable for various MCUs.  
- **I<sup>2</sup>C and SPI Support:** Default interface is **I<sup>2</sup>C**, but can be configured to **SPI mode** using the **PS pin**.  
- **Ultra-Low Power Consumption:** Optimized for battery-powered applications.  

---

## Technical Specifications

**Model:** MS5611-01BA03  
**Input Voltage:** 3.3V - 5.5V  
**Voltage Input Type:** 4-pin 2.50mm header  
**Interface:** I<sup>2</sup>C (default), SPI (configurable)  
**Functions:** High-Precision Barometric Pressure Measurement  
**ADC Resolution:** 24-bit  
**Operating Pressure Range:** 450 to 1100 mbar (Full Accuracy)  
**Extended Pressure Range:** 10 to 1200 mbar (Linear ADC Range)  
**Operating Temperature:** -40°C ~ +85°C  
**Board Dimensions:** 20mm x 20mm  

---

## Board Pinout

### **( J1 ) I<sup>2</sup>C Communication Pins**  

| Pin Number | Pin Name | Description |
|:---:|:---:|---|
| 1 | VCC | Power Supply (3.3V - 5.5V) |
| 2 | SCL | I<sup>2</sup>C Serial Clock Pin |
| 3 | SDA | I<sup>2</sup>C Serial Data Pin |
| 4 | GND | Ground |

### **( J2 ) SPI Mode & Configuration Pins**  

| Pin Number | Pin Name | Description |
|:---:|:---:|---|
| 1 | CSB | Chip select (active low), internally pulled down to ground with a pull-down resistor. |
| 2 | SDO | Serial data output. |
| 3 | PS | Protocol selection:<ul><li>PS High (3.3V) = I<sup>2</sup>C (Default address: 0x77)</li><li>PS Low (GND) = SPI Mode</li></ul> (Internally pulled up to 3.3V with a pull-up resistor). |

---

## Board Dimensions

<img src="./assets/MS5611 Dimension.png" alt=" MS5611 Dimension" width="450"/>

---

## Step Files

[Boardoza MS5611.step](./assets/MS5611%20Step.step)

---

## Datasheet

[Boardoza MS5611 Datasheet.pdf](./assets/MS5611%20Datasheet.pdf)

---

## Version History

- V1.0.0 - Initial Release

---

## Support

- If you have any questions or need support, please contact <support@boardoza.com>

---

## License

Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a [Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
