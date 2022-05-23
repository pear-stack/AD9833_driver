/*************************************************************************************
							 AD9833 DDS Wave Generator Driver
							 Author: Adrian Grusza
							 Created: 23.05.2022
*************************************************************************************/

#ifndef AD9833_SPI_DRIVER_H
#define AD9833_SPI_DRIVER_H

#include "stm32f3xx_hal.h" // Needed for SPI
#include <math.h> //Needed for frequency and phase calculations

// ------------------------- Sensor Struct -------------------------
typedef struct{
	SPI_HandleTypeDef *spiHandle;
	GPIO_TypeDef *cs_gpio_port;
	uint16_t cs_gpio_pin;
	uint16_t FRQLW;    // MSB of Frequency Tuning Word
	uint16_t FRQHW;    // LSB of Frequency Tuning Word
	uint16_t phase;  // Phase value
	uint16_t wave;
} AD9833;


// ------------------------- Defines -------------------------
#define FMCLK 25000000        // Internal clock frequency
enum WaveType {SIN, SQR, TRI};

// ------------------------- Functions -------------------------
void AD9833_SetWaveType(AD9833 *dev, enum WaveType wave);                // Sets output wave type
void AD9833_SetFrequency(AD9833 *dev, float frequency);              // Sets output wave frequency
void AD9833_SetPhase(AD9833 *dev, float phase);    					 // Sets output wave phase
void AD9833_Init(AD9833 *dev, SPI_HandleTypeDef *spiHandle);   // Initializing AD9833
void AD9833_WriteRegister(AD9833 *dev, uint16_t *data, uint16_t size);
void AD9833_SetCSpin(AD9833 *dev, GPIO_TypeDef* port, uint16_t pin);
void AD9833_OutptEnable(AD9833 *dev);

#endif
