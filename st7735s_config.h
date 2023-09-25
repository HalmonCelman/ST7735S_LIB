/*
 * st7735s_config.h
 *
 *  Created on: Sep 14, 2023
 *      Author: KK
 */

#ifndef ST7735S_CONFIG_H_
#define ST7735S_CONFIG_H_

/*
 *	ST7735S_DISABLE_DELAY
 *	if you have your own implementation of delay() function you can set this option to 1
 *	you will disable then delay function which is builtin in this library
 *
 *	delay(x) should wait x miliseconds - important to lcd initialization
 */
#define ST7735S_DISABLE_DELAY 0

#if ST7735S_DISABLE_DELAY == 0
	#define F_CPU 8000000				//default without choosing clock source its from HSI(in STM32) which is 8MHz

	extern void ST7735S_DelayInterruptInit(void);
#endif


//! functions - mcu dependent - implemented in st7735s_config.c !
extern void ST7735S_SPI_Init(void);
extern uint8_t SPI_SendRcv(uint8_t data);
extern void ST7735S_GPIO_Init(void);
extern void ST7735S__LowDC(void);
extern void ST7735S__HighDC(void);

/*
 * features:
 *	USE_FONT - if you want to minimize your code you should set it to 0 then WriteChar and WriteString functions are disabled
 */
#define ST7735S_USE_FONT 1

// configuration
#define ST7735S_LCD_WIDTH 160
#define ST7735S_LCD_HEIGHT 128

#define ST7735S_LCD_REFRESH_INV_HORIZONTAL	0	// 0 - from left to right, 	1 - from right to left
#define ST7735S_LCD_REFRESH_INV_VERTICAL   	0	// 0 - from top to bottom, 	1 - from bottom to top
#define ST7735S_LCD_DATA_INV_HORIZONTAL 	1	// 0 - from left to right, 	1 - from right to left
#define ST7735S_LCD_DATA_INV_VERTICAL   	0	// 0 - from top to bottom, 	1 - from bottom to top


#define ST7735S_X_OFFS 2
#define ST7735S_Y_OFFS 1

/*	Software color format
 * 	colors should always be in RGB format
 *  0 (default) - 8-8-8 - 8 bits per R,G and B
 *	1 - percentage - from 0 to 100 for each color
 *	2 - 6-6-6
 *	3 - 5-6-5
 *	4 - 4-4-4
 */
#define ST7735S_SOFTWARE_COLOR_FORMAT		0

/*	Hardware color format
 *	0 (default)	- 6-6-6
 *	1 - 5-6-5
 *	2 - 4-4-4 - not working yet
 */
#define ST7735S_HARDWARE_COLOR_FORMAT		0
#define ST7735S_LCD_RGB_BGR   0					// 0 - RGB colors, 			1 - BGR colors

#endif /* SRC_ST7735S_ST7735S_CONFIG_H_ */
