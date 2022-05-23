/*
 * AD9833_driver.c
 *
 *  Created on: 23 maj 2022
 *      Author: Smuff
 */
#include "AD9833_driver.h"

void AD9833_SetCSpin(AD9833 *dev, GPIO_TypeDef* port, uint16_t pin){
	dev->cs_gpio_port = port;
	dev->cs_gpio_pin = pin;
};

void AD9833_Init(AD9833 *dev, SPI_HandleTypeDef *spiHandle){
	dev->spiHandle = spiHandle;
	uint16_t control_regster_reset = 0x0100;
	AD9833_WriteRegister(dev, &control_regster_reset, 1);
};

void AD9833_SetFrequency(AD9833 *dev, float frequency){
	int freq_reg = 0;
	freq_reg = (int)((frequency * pow(2,28))/(25000000));
	dev->FRQHW = (int)((freq_reg & 0xFFFC000) >> 14);
	dev->FRQLW = (int)(freq_reg & 0x3FFF);
	dev->FRQHW |= 0x4000;
	dev->FRQLW |= 0x4000;
	uint16_t control_register_set = 0x2100;
	AD9833_WriteRegister(dev, &control_register_set, 1); //Control Word Write
	AD9833_WriteRegister(dev, &dev->FRQLW, 1);
	AD9833_WriteRegister(dev, &dev->FRQHW, 1);
};

void AD9833_SetPhase(AD9833 *dev, float phase){
	if(phase<0)
		phase=0; // Changing Phase Value to Positive
	if(phase>360)
		phase=360; // Maximum value For Phase (In Degree)
	 dev->phase = (uint16_t)(((int)(phase*(4096/360)))|0xC000);
	 AD9833_WriteRegister(dev, &dev->phase, 1); //Control Word Write
};

void AD9833_SetWaveType(AD9833 *dev, enum WaveType wave){
	dev->wave = wave;
	switch(dev->wave){
		case 0:
			dev->wave = 0x2100;
			AD9833_WriteRegister(dev, &dev->wave, 1);
			break;
		case 1:
			dev->wave = 0x2128;
			AD9833_WriteRegister(dev, &dev->wave, 1);
			break; //0x2028
		case 2:
			dev->wave = 0x2102;
		  	AD9833_WriteRegister(dev, &dev->wave, 1);
		  	break;
		default:
			break;
	}
};

void AD9833_WriteRegister(AD9833 *dev, uint16_t * data, uint16_t size){
	HAL_GPIO_WritePin(dev->cs_gpio_port, dev->cs_gpio_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->spiHandle, (uint8_t *)data, size, 200);
	HAL_GPIO_WritePin(dev->cs_gpio_port, dev->cs_gpio_pin, GPIO_PIN_SET);
};

void AD9833_OutptEnable(AD9833 *dev){
	dev->wave &= 0xFEFF;
	AD9833_WriteRegister(dev, &dev->wave, 1);
};


