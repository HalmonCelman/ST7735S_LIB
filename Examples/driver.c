/*
 * st7735s_config.c
 *
 *  Created on: Sep 16, 2023
 *      Author: KK
 */
#include <stdint.h>
#include <st7735s.h>

void ST7735S_SPI_Init(void){
// initializing SPI2 on stm32f302rbt6 - if you want another device or spi you should write this by yourself
	*((uint32_t *)0x4002101C) |= (1<<14); // RCC for spi2 enable - APB1
	*((uint16_t *)0x40003804) = 0x0700;   // SPI2-CR2,  datasize 8bit
	*((uint16_t *)0x40003800) = 0x0344;   // SPI2->CR1, SPI2 enable, master mode, SSM: 1 - let's free our nss pin, SSI: 1 - let's enable our slave
}

uint8_t SPI_SendRcv(uint8_t data){ 		//this command should send and recieve data by SPI
	*((uint8_t *)0x4000380C) = data; 	//SPI2->DR = data
	while((*((uint16_t *)0x40003808)) & (1<<7));	//while SPI2->SR BSY flag is set to 1
	return *((uint8_t *)0x4000380C);	//return SPI2->DR
}

void ST7735S_GPIO_Init(void){
// GPIO for SPI2
*((uint32_t *)0x40021014) |= (1<<18); // RCC for port B enable

*((uint32_t *)0x48000400) |= (2<<30) | (2<<28) | (2<<26); 		//MODER - SCK(PB13)-AF MISO(PB14)-AF MOSI(PB15)-AF
*((uint32_t *)0x48000408) |= (3<<30) | (3<<28) | (3<<26);		//OSPEEDR - PB: 13,14,15 - high speed
*((uint32_t *)0x48000424) |= (0x5<<28) | (0x5<<24) | (0x5<<20); //AFRH - AF 5 on all

// DC pin(PB10)
*((uint32_t *)0x48000400) |= (1<<20); 							//MODER - DC(PB10) as output
*((uint32_t *)0x48000408) |= (3<<20);							//OSPEEDR - high speed

// BL pin(PB4)
*((uint32_t *)0x48000400) &=~ (1<<9);
*((uint32_t *)0x48000400) |= (1<<8);							//MODER - BL(PB4) as output
*((uint32_t *)0x4800040C) &=~ (1<<8);
*((uint32_t *)0x48000414) |= (1<<4);							//ODR - high state

// RST pin(PB5)
*((uint32_t *)0x48000400) |= (1<<10);							//MODER - RST(PB5) as output
*((uint32_t *)0x48000414) |= (1<<5);							//ODR - high state
}

// put DC pin into low state
void ST7735S__LowDC(void){
	*((uint32_t *)0x48000414) &=~ (1<<10);	// GPIOB PIN10 low
}

//put DC pin into high state
void ST7735S__HighDC(void){
	*((uint32_t *)0x48000414) |= (1<<10);	// GPIOB PIN10 low
}

#if ST7735S_DISABLE_DELAY == 0

	void ST7735S_DelayInterruptInit(void){

		ST7735S_DelayCounter=0;
		*((uint32_t *)0xE000E100) |= (1<<25); 	// NVIC_ISER0: lets enable interrupt TIM1_UP

		*((uint32_t *)0x40021018) |= (1<<11);		// RCC for TIM1 enable
		*((uint16_t *)0x40012C0C) = 0x01;			// TIM1->DIER enable update event
		*((uint16_t *)0x40012C28) = F_CPU/1000000-1;				// TIM1->PSC prescale set to 63 (or mcu is default 8MHz( restarting from hsi) so it will generate 1MHz)
		*((uint16_t *)0x40012C2C) = 1000;			// ARR= 1000 as a second divider timer will generate 1Khz signal
		*((uint16_t *)0x40012C00) = 0x01;			// TIM1->CR1 - enable timer


	}

	void TIM1_UP_TIM16_IRQHandler(void) {	//wrapper to interrupt function - without it delay won't work - it should match the name of your interrupt in linker script
		DelayInterrupt();		
	}

#endif
