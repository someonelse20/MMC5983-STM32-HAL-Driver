# MMC5983 STM32 HAL Driver

This is a fork of [NathanNetzel's SPI MMC5983 Driver](https://github.com/NathanNetzel/MMC5983-STM32-HAL-Driver)
C driver library for the MMC5983 3-axis magnetometer sensor.  
Designed for STM32 microcontrollers using the STM32 HAL library and I2C interface.

---

## **Features**

- Verify device ID  
- Perform software reset  
- Configure internal bandwidth  
- Set operation rate and enable periodic SET operations  
- Enable automatic SET/RESET for each measurement  
- Configure output data rate  
- Enable continuous measurement mode  
- Enable interrupt on data ready  
- Read magnetometer data from X, Y, Z axes  
- SPI communication with chip select control  
- Basic error handling  

---

## **Requirements**

- STM32 MCU with STM32 HAL library  
- SPI hardware interface connected to MMC5983 sensor  
- Basic experience with embedded C and STM32CubeIDE or similar  

---

**Note:**  
This driver includes `#include "stm32f4xx_hal.h"` in the header files and is configured for STM32F4 series.  
For other STM32 families, adjust the HAL include and any hardware-specific settings accordingly.

## **Quick Start**

1. Add driver files to your project

Include `MMC5983.c` and `MMC5983.h` in your STM32 project source folder.

2. Configure your hardware

Set up SPI peripheral and GPIO pins for SPI and Chip Select (CS) according to your board schematic.

3. Initialize driver handle structure

```c
MMC5983_HW_InitTypeDef MMC5983_Handle = {
    .SPIhandler = &hspi1,             // Your SPI handle configured in your project
    .CS_GPIOport = GPIOA,             // GPIO port of CS pin
    .CS_GPIOpin = GPIO_PIN_4,         // GPIO pin of CS
    .SPI_Timeout = 100                // SPI timeout in milliseconds
};
```

4. Verify sensor connection

```c
if (MMC5983_ID_Verify(&MMC5983_Handle) != MMC_NO_ERROR) {
    // Handle error: sensor not detected
}
```

5. Configure sensor settings

```c
MMC5983_SW_Reset(&MMC5983_Handle);
MMC5983_Set_Bandwidth(&MMC5983_Handle, MMC_BW_0400);
MMC5983_Set_Operation_Rate(&MMC5983_Handle, MMC_SET_0500);
MMC5983_Enable_Auto_Set_Reset(&MMC5983_Handle);
MMC5983_Set_Output_DataRate(&MMC5983_Handle, MMC_ODR_0100);
MMC5983_Set_Continuous_Measurement(&MMC5983_Handle);
```

6. Read magnetometer data

```c
MMC5983_Data_TypeDef mag_data;
if (MMC5983_Data_Read(&MMC5983_Handle, &mag_data) == MMC_NO_ERROR) {
    int16_t x = mag_data.axes.DX;
    int16_t y = mag_data.axes.DY;
    int16_t z = mag_data.axes.DZ;
    // Process magnetometer data
}
```

---

## **API Overview**

- `MMC5983_ID_Verify()` — Verify sensor presence  
- `MMC5983_SW_Reset()` — Perform software reset  
- `MMC5983_Set_Bandwidth()` — Configure internal bandwidth  
- `MMC5983_Set_Operation_Rate()` — Set operation rate and enable periodic SET  
- `MMC5983_Enable_Auto_Set_Reset()` — Enable automatic SET/RESET  
- `MMC5983_Set_Output_DataRate()` — Configure output data rate  
- `MMC5983_Set_Continuous_Measurement()` — Enable continuous measurement  
- `MMC5983_Set_Interrupt_Measurement()` — Enable interrupt on data ready  
- `MMC5983_Data_Read()` — Read X, Y, Z magnetometer data  
- `MMC5983_SingleRegister_Read()` / `MMC5983_SingleRegister_Write()` — Low-level register access  

---

## **License**

This project is licensed under the MIT License. See the LICENSE file for details.

---

## **Author**

Nathan Netzel  
Electrical Engineering Student - Londrina State University

