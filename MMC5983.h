/*
 * MMC5983.h
 *
 * Created on: Jul 29, 2025
 * Author: Nathan Netzel
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Nathan Netzel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the conditions of the MIT License.
 */

#ifndef INC_MMC5983_H_
#define INC_MMC5983_H_


#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_i2c.h"


// --- Registers ---
#define MMC_DATA_ADDRESS_INIT				0x00
#define MMC_DATAX0							0x00
#define MMC_DATAX1							0x01
#define MMC_DATAY0							0x02
#define MMC_DATAY1 							0x03
#define MMC_DATAZ0							0x04
#define MMC_DATAZ1							0x05
#define MMC_DATAXYZ2						0x06
#define MMC_DATAT							0x07

#define MMC_STATUS							0x08
#define MMC_INTERNAL_CONTROL_0				0x09
#define MMC_INTERNAL_CONTROL_1				0x0A
#define MMC_INTERNAL_CONTROL_2				0x0B
#define MMC_INTERNAL_CONTROL_3				0x0C
#define MMC_ID_REG							0x2F

// --- Device ID ---
#define MMC_DEVICE_ID						0x30

// --- Device Output Data Rate ---
#define MMC_ODR_0001		0x01
#define MMC_ODR_0010		0x02
#define MMC_ODR_0020		0x03
#define MMC_ODR_0050		0x04
#define MMC_ODR_0100		0x05
#define MMC_ODR_0200		0x06
#define MMC_ODR_1000		0x07

// --- Device BW Rate ---
#define MMC_BW_0100			0x00
#define MMC_BW_0200			0x01
#define MMC_BW_0400			0x02
#define MMC_BW_0800			0x03

// --- Device SET Operation Rate ---
#define MMC_SET_0001		0x00
#define MMC_SET_0025		0x10
#define MMC_SET_0075		0x20
#define MMC_SET_0100		0x30
#define MMC_SET_0250		0x40
#define MMC_SET_0500		0x50
#define MMC_SET_1000		0x60
#define MMC_SET_2000		0x70

// --- Operation Modes ---
#define MMC_MEASUREMODE 			0x08
#define MMC_SET_ENABLE				0x80
#define MMC_AUTO_SET_RESET_ENABLE	0x20

// --- Interrupt Map ---
#define MMC_INT_DATA_READY			0x04

// --- I2C Commands ---
#define MMC_Read_Command			0x80
#define MMC_Read_Multiple_Command	0xC0
#define MMC_Write_Command			0x00
#define MMC_DEV_Address				0x30

// --- Reset Command ---
#define MMC5983_SW_Reset_Mask		0x80

// --- I2C Timeout (ms) ---
#define MMC_Timeout	10

// --- Error Status Enum ---
typedef enum {
	MMC_NO_ERROR 			=	0x00U, 	/**< No error */
	MMC_ERROR	  			=	0x01U, 	/**< Generic error */
	MMC_HAL_ERROR			=	0x02U, 	/**< HAL communication error */
	MMC_INV_REG_DATA_READ	=	0x03U, 	/**< Invalid data read from register */
	MMC_ID_VERIFY_ERROR		=	0x04U, 	/**< Device ID verification failed */
} MMC5983_Error_TypeDef;

// --- Hardware Initialization Structure ---
typedef struct {
	I2C_HandleTypeDef *I2C_handler; 	/**< Pointer to I2C handler */
	uint8_t I2C_Timeout;			/**< I2C timeout in milliseconds */
	uint8_t INT_CTRL_0_Reg;         /**< Interrupt control register 0 content */
	uint8_t INT_CTRL_1_Reg;         /**< Interrupt control register 1 content */
	uint8_t INT_CTRL_2_Reg;         /**< Interrupt control register 2 content */
	uint8_t INT_CTRL_3_Reg;         /**< Interrupt control register 3 content */
} MMC5983_HW_InitTypeDef;

/**
 * @brief  3-axis sensor data structure
 *
 * Contains signed 16-bit values for each axis and raw data array
 */
typedef union {
    struct {
        int16_t DX;
        int16_t DY;
        int16_t DZ;
    } axes;
    uint8_t raw_data[6];
}MMC5983_Data_TypeDef;

/**
 * @brief  Temperature data structure
 */
typedef struct MMC5983_TEMP{
	uint8_t Temp;
} MMC5983_Temp_Data_TypeDef;

// --- Function Prototypes ---

/**
 * @brief  Verify the MMC5983 device ID
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_ID_Verify(MMC5983_HW_InitTypeDef *);

/**
 * @brief  Perform a software reset on the MMC5983 device
 * @note   Sets reset bit in internal control register and waits 15ms
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_SW_Reset(MMC5983_HW_InitTypeDef *);

/**
 * @brief  Configure the internal bandwidth of the MMC5983
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @param  MMC5983_BW_Rate Bandwidth configuration bits
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_Set_Bandwidth(MMC5983_HW_InitTypeDef *, uint8_t);

/**
 * @brief  Set the operation rate and enable periodic set operations
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @param  MMC5983_Set_Rate Operation rate bits (SET frequency)
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_Set_Operation_Rate(MMC5983_HW_InitTypeDef *, uint8_t);

/**
 * @brief  Enable automatic SET/RESET operations on each measurement
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_Enable_Auto_Set_Reset(MMC5983_HW_InitTypeDef *);

/**
 * @brief  Set the MMC5983 output data rate
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @param  MMC5983_Rate 8-bit value of the DataRate (see datasheet)
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_Set_Output_DataRate(MMC5983_HW_InitTypeDef *, uint8_t);

/**
 * @brief  Set the MMC5983 to continuous measurement mode
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_Set_Continuous_Measurement(MMC5983_HW_InitTypeDef *);

/**
 * @brief  Set the MMC5983 to interrupt measurement mode
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @param  MMC5983_Data Pointer to MMC5983_Data_TypeDef structure
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_Set_Interrupt_Measurement(MMC5983_HW_InitTypeDef *, MMC5983_Data_TypeDef *);

/**
 * @brief  Read X, Y, Z data from the sensor
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @param  MMC5983_Data Pointer to MMC5983_Data_TypeDef structure to store data
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_Data_Read(MMC5983_HW_InitTypeDef *, MMC5983_Data_TypeDef *);

/**
 * @brief  Read a single register from MMC5983
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @param  MMC5983_Register_Addr Register address to read
 * @param  pData Pointer to uint8_t variable to store read data
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_SingleRegister_Read(MMC5983_HW_InitTypeDef *, uint8_t, uint8_t *);

/**
 * @brief  Write a single register on MMC5983
 * @param  MMC5983_Handler Pointer to MMC5983_HW_InitTypeDef structure
 * @param  MMC5983_Register_Addr Register address to write
 * @param  MMC5983_Write_Data 8-bit data to write
 * @retval MMC5983_Error_TypeDef
 */
MMC5983_Error_TypeDef MMC5983_SingleRegister_Write(MMC5983_HW_InitTypeDef *, uint8_t, uint8_t);


#endif /* INC_MMC5983_H_ */
