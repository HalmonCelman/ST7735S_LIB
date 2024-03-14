/*
 * st7735s.c
 *
 *  Created on: Sep 13, 2023
 *      Author: KK
 */
#include <stdint.h>
#include "st7735s.h"
#include "st7735s_cl.h"

#if ST7735S_DISABLE_DELAY
	extern void delay(uint16_t);
#endif

#if ST7735S_USE_FONT
	#include "font8x8_basic.h"
#endif

void ST7735S__WriteCommand(uint8_t);
void ST7735S__Write(uint8_t);
void ST7735S__WriteColor(uint32_t);
uint32_t ST7735S__CalculateColor(uint32_t);

void ST7735S_Init(void){
	ST7735S_GPIO_Init();
	ST7735S_SPI_Init();

#if ST7735S_DISABLE_DELAY == 0
	ST7735S_DelayInterruptInit();
#endif
	delay(120);								// lets give it some time to stabilize timers
	ST7735S__WriteCommand(ST7735S_SLPOUT);
	delay(120);								// time to exit slpin

	ST7735S__WriteCommand(ST7735S_MADCTL);	// set Memory Adress Data Control
	ST7735S__Write(ST7735S__CALC_MADCTL);

	ST7735S__WriteCommand(ST7735S_COLMOD);
	#if ST7735S_HARDWARE_COLOR_FORMAT == 0
		ST7735S__Write(0b110);
	#elif ST7735S_HARDWARE_COLOR_FORMAT == 1
		ST7735S__Write(0b101);
	#elif ST7735S_HARDWARE_COLOR_FORMAT == 2
		ST7735S__Write(0b011);
	#else
		Wrong Hardware color format
	#endif

	ST7735S__WriteCommand(ST7735S_DISPON);

}

void ST7735S_SetPixel(uint8_t x,uint8_t y,uint32_t color){
	ST7735S__WriteCommand(ST7735S_CASET);
	ST7735S__Write(0);
	ST7735S__Write(ST7735S__OFFSETX(x));
	ST7735S__Write(0);
	ST7735S__Write(ST7735S__OFFSETX(x));
	ST7735S__WriteCommand(ST7735S_RASET);
	ST7735S__Write(0);
	ST7735S__Write(ST7735S__OFFSETY(y));
	ST7735S__Write(0);
	ST7735S__Write(ST7735S__OFFSETY(y));
	ST7735S__WriteCommand(ST7735S_RAMWR);
	color = ST7735S__CalculateColor(color);
	ST7735S__WriteColor(color);
}


void ST7735S_FillRect(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint32_t color){
	ST7735S__WriteCommand(ST7735S_CASET);
	ST7735S__Write(0);
	ST7735S__Write(ST7735S__OFFSETX(x));
	ST7735S__Write(0);
	ST7735S__Write(ST7735S__OFFSETX(x+width-1));
	ST7735S__WriteCommand(ST7735S_RASET);
	ST7735S__Write(0);
	ST7735S__Write(ST7735S__OFFSETY(y));
	ST7735S__Write(0);
	ST7735S__Write(ST7735S__OFFSETY(y+height-1));
	ST7735S__WriteCommand(ST7735S_RAMWR);
	color=ST7735S__CalculateColor(color);
	for(int i=0;i<(width*height);i++){
		ST7735S__WriteColor(color);
	}
}

void ST7735S_FillScreen(uint32_t color){
	ST7735S_FillRect(0,0,ST7735S_LCD_WIDTH,ST7735S_LCD_HEIGHT,color);
}

#if ST7735S_USE_FONT
	void ST7735S_WriteChar(char charToWrite,uint8_t x,uint8_t y,uint32_t color,uint32_t backgroundColor){
		ST7735S__WriteCommand(ST7735S_CASET);
		ST7735S__Write(0);
		ST7735S__Write(ST7735S__OFFSETX(x));
		ST7735S__Write(0);
		ST7735S__Write(ST7735S__OFFSETX(x+7));
		ST7735S__WriteCommand(ST7735S_RASET);
		ST7735S__Write(0);
		ST7735S__Write(ST7735S__OFFSETY(y));
		ST7735S__Write(0);
		ST7735S__Write(ST7735S__OFFSETY(y+7));
		ST7735S__WriteCommand(ST7735S_RAMWR);
		color = ST7735S__CalculateColor(color);
		for(uint8_t j=0;j<8;j++){
			for(uint8_t i=0;i<8;i++){
				if(font8x8_basic[(uint8_t)charToWrite][j] & (1<<i)){
					ST7735S__WriteColor(color);
				}else{
					ST7735S__WriteColor(backgroundColor);
				}
			}
		}
	}

	void ST7735S_WriteString(char * string,uint8_t x,uint8_t y,uint32_t color,uint32_t backgroundColor){
		uint8_t i=0;
		while(*string){
			ST7735S_WriteChar(*string++, x+i, y, color, backgroundColor);
			i+=8;
		}
	}
#endif

#if ST7735S_DISABLE_DELAY == 0
	uint32_t ST7735S_DelayCounter;
	
	void DelayInterrupt(void){
		if(ST7735S_DelayCounter)
			ST7735S_DelayCounter--;
		*((uint16_t *)0x40012C10) &=~ 0x01;  // TIM1->SR: delete UIF status
		*((uint32_t *)0xE000E280) = (1<<25); // NVIC_ICPR0: lets clear pending interrupt on TIM1 UPDATE
	}
#endif

void ST7735S__WriteCommand(uint8_t command){
	ST7735S__LowDC();
	ST7735S_SPI_SendRcv(command);
}

void ST7735S__Write(uint8_t data){
	ST7735S__HighDC();
	ST7735S_SPI_SendRcv(data);
}

void ST7735S__WriteColor(uint32_t color){ // not really working in 4-4-4 mode TBD
	#if	ST7735S_HARDWARE_COLOR_FORMAT==0
		ST7735S__Write((uint8_t)(color>>16));
	#endif
		ST7735S__Write((uint8_t)((color>>8) & 0xFF));
		ST7735S__Write((uint8_t)(color & 0xFF));
}
uint32_t ST7735S__CalculateColor(uint32_t color){
	#if (ST7735S_HARDWARE_COLOR_FORMAT+2) == ST7735S_SOFTWARE_COLOR_FORMAT
		return color;
	#else

		uint8_t red_f,green_f,blue_f;
		uint8_t tmpred,tmpgreen,tmpblue;

		#if	ST7735S_HARDWARE_COLOR_FORMAT==0
			red_f=63;
			green_f=63;
			blue_f=63;
		#elif ST7735S_HARDWARE_COLOR_FORMAT==1
			red_f=31;
			green_f=63;
			blue_f=31;
		#elif ST7735S_HARDWARE_COLOR_FORMAT==2
			red_f=15;
			green_f=15;
			blue_f=15;
		#else
			Unknown Hardware Color Format
		#endif

		#if ST7735S_SOFTWARE_COLOR_FORMAT==0
			tmpred =	(color>>16)*red_f/255;
			tmpgreen =	((color>>8) & 0xFF)*green_f/255;
			tmpblue =	(color & 0xFF)*blue_f/255;
		#elif ST7735S_SOFTWARE_COLOR_FORMAT==1
			tmpred =	(color>>16)*red_f/100;
			tmpgreen =	((color>>8) & 0xFF)*green_f/100;
			tmpblue =	(color & 0xFF)*blue_f/100;
		#elif ST7735S_SOFTWARE_COLOR_FORMAT==2
			tmpred =	(color>>16)*red_f/63;
			tmpgreen =	((color>>8) & 0xFF)*green_f/63;
			tmpblue =	(color & 0xFF)*blue_f/63;
		#elif ST7735S_SOFTWARE_COLOR_FORMAT==3
			tmpred =	(color>>16)*red_f/31;
			tmpgreen =	((color>>8) & 0xFF)*green_f/63;
			tmpblue =	(color & 0xFF)*blue_f/31;
		#elif ST7735S_SOFTWARE_COLOR_FORMAT==4
			tmpred =	(color>>16)*red_f/15;
			tmpgreen =	((color>>8) & 0xFF)*green_f/15;
			tmpblue =	(color & 0xFF)*blue_f/15;
		#else
			Unknown Software Color Format
		#endif

		#if	ST7735S_HARDWARE_COLOR_FORMAT==0
			#if ST7735S_LCD_RGB_BGR
				return (tmpblue<<18) | (tmpgreen<<10) | (tmpred<<2);
			#else
				return (tmpred<<18) | (tmpgreen<<10) | (tmpblue<<2);
			#endif
		#elif ST7735S_HARDWARE_COLOR_FORMAT==1
			#if ST7735S_LCD_RGB_BGR
				return (tmpblue<<11) | (tmpgreen<<5) | tmpred;
			#else
				return (tmpred<<11) | (tmpgreen<<5) | tmpblue;
			#endif
		#elif ST7735S_HARDWARE_COLOR_FORMAT==2
			#if ST7735S_LCD_RGB_BGR
				return (tmpblue<<8) | (tmpgreen<<4) | tmpred;
			#else
				return (tmpred<<8) | (tmpgreen<<4) | tmpblue;
			#endif
		#else
			Unknown Hardware Color Format
		#endif
	#endif
}
