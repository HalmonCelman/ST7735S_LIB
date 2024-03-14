/*
 * st7735s.h
 *
 *  Created on: Sep 13, 2023
 *      Author: KK
 */

#ifndef ST7735S_H
#define ST7735S_H

#include "../st7735s_config.h"

//! functions - mcu dependent - implemented in st7735s_config.c !
extern void ST7735S_SPI_Init(void);
extern uint8_t ST7735S_SPI_SendRcv(uint8_t data);
extern void ST7735S_GPIO_Init(void);
extern void ST7735S__LowDC(void);
extern void ST7735S__HighDC(void);

//! functions independent
void ST7735S_Init(void);
void ST7735S_SetPixel(uint8_t x,uint8_t y,uint32_t color);
void ST7735S_FillRect(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint32_t color);
void ST7735S_FillScreen(uint32_t color);

#if ST7735S_USE_FONT
	void ST7735S_WriteChar(char,uint8_t,uint8_t,uint32_t,uint32_t);
	void ST7735S_WriteString(char *,uint8_t,uint8_t,uint32_t,uint32_t);
#endif

#if ST7735S_DISABLE_DELAY == 0
	#define delay(x) ST7735S_DelayCounter=x; while(ST7735S_DelayCounter)
	
	extern uint32_t ST7735S_DelayCounter;
	void DelayInterrupt(void); 
#endif

// useful macros
#if ST7735S_LCD_WIDTH > ST7735S_LCD_HEIGHT
	#define ST7735S__MX ST7735S_LCD_DATA_INV_VERTICAL
	#define ST7735S__MY ST7735S_LCD_DATA_INV_HORIZONTAL
	#define ST7735S__MV	1

	#define ST7735S__OFFSETX(x) (x+ST7735S_Y_OFFS)
	#define ST7735S__OFFSETY(x) (x+ST7735S_X_OFFS)
#else
	#define ST7735S__MX ST7735S_LCD_DATA_INV_HORIZONTAL
	#define ST7735S__MY ST7735S_LCD_DATA_INV_VERTICAL
	#define ST7735S__MV	0

	#define ST7735S__OFFSETY(x) (x+ST7735S_X_OFFS)
	#define ST7735S__OFFSETX(x) (x+ST7735S_Y_OFFS)
#endif

#define ST7735S__CALC_MADCTL ((ST7735S_LCD_REFRESH_INV_HORIZONTAL<<2) | (ST7735S_LCD_RGB_BGR << 3) | (ST7735S_LCD_REFRESH_INV_VERTICAL << 4) | (ST7735S__MV << 5) | (ST7735S__MX << 6) | (ST7735S__MY << 7))


#endif /* ST7735S_ST7735S_H_ */
