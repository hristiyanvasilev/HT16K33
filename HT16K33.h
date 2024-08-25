#ifndef HT16K33_H
#define HT16K33_H

#include "main.h"
#include "stdbool.h"

//4x7 segments
#define HT16K33_SEG_NUM     4
//there is also a ":" in the middle of the display - "X X : X X"
#define HT16K33_SEG_TOTAL   (HT16K33_SEG_NUM + 1)

#define HT16K33_DEFAULT_ADDR 0x70
#define HT16K33_DEFAULT_BRIGHTNESS 8

//  Commands
#define HT16K33_ON              0x21  //  0 = off   1 = on
#define HT16K33_STANDBY         0x20  //  bit xxxxxxx0


//  bit pattern 1000 0xxy
//  y    =  display on / off
//  xx   =  00=off     01=2Hz     10 = 1Hz     11 = 0.5Hz
#define HT16K33_DISPLAYON       0x81
#define HT16K33_DISPLAYOFF      0x80
#define HT16K33_BLINKON0_5HZ    0x87
#define HT16K33_BLINKON1HZ      0x85
#define HT16K33_BLINKON2HZ      0x83
#define HT16K33_BLINKOFF        0x81


//  bit pattern 1110 xxxx
//  xxxx    =  0000 .. 1111 (0 - F)

#define HT16K33_BRIGHTNESS      0xE0


bool ht16k33_display_int(uint16_t num);

void ht16k33_init(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t brightness);

#endif /* HT16K33_H */
