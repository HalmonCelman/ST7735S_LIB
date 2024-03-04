# How to write driver file?
you just need to write few functions

**If you have already delay() function you can set in *st7735s_config.h*  ST7735S_DISABLE_DELAY option to 1**

then your delay will be used: delay should be declared as `void delay(uint16_t);`

## Required functions
- `void ST7735S_SPI_Init(void)`: this function should initialize your spi interface which is connected to your lcd
- `uint8_t SPI_SendRcv(uint8_t data)`: this function should send and recieve 8 bytes by spi
- `void ST7735S_GPIO_Init(void)`: this function should initialize all your gpio pins( connected to lcd )
- `void ST7735S__LowDC(void)`: this function should put your DC(data/command) pin into low state(0V)
- `void ST7735S__HighDC(void)`: this function should put your DC(data/command) pin into hihg state(3,3V)
- if you don't have your own delay() function:
    1. `void ST7735S_DelayInterruptInit(void)`: you should initilize here interrupt that will work on 1Khz (1ms period)
    2. you should create wrapper
    ```
    void <your interrupt name in linker file>(void){
        DelayInterrupt();
    }
    ```
    ***without that delay won't work!*** (and screen will be always white)

[see example of implementation in Examples](Examples/driver.c)