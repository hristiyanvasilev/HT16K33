#include "HT16K33.h"
#include "main.h"

static const uint8_t ht16k33_map[] = {
  0x3F,   //  0
  0x06,   //  1
  0x5B,   //  2
  0x4F,   //  3
  0x66,   //  4
  0x6D,   //  5
  0x7D,   //  6
  0x07,   //  7
  0x7F,   //  8
  0x6F,   //  9
  0x77,   //  A
  0x7C,   //  B
  0x39,   //  C
  0x5E,   //  D
  0x79,   //  E
  0x71,   //  F
  0x00,   //  empty
  0x40,   //  minus
  0x61,   //  TOP_C
  0x63,   //  degree
};

#define HT16K33_EMPTY 16

static struct {
	bool is_init;
	I2C_HandleTypeDef *hi2c;
	uint16_t addr;
	uint8_t brightness;
} ht16k33_state;



static bool ht16k33_is_connected()
{
	if (!ht16k33_state.is_init) {
		return false;
	}

	return HAL_I2C_IsDeviceReady(ht16k33_state.hi2c, ht16k33_state.addr, 1, HAL_MAX_DELAY) == HAL_OK;
}

static bool ht16k33_send_commands_blocking(uint8_t *buff, uint16_t size)
{
	if (!ht16k33_state.is_init) {
		return false;
	}

	return HAL_I2C_Master_Transmit(ht16k33_state.hi2c, ht16k33_state.addr, buff, size, HAL_MAX_DELAY) == HAL_OK;
}

static bool ht16k33_send_command(uint8_t cmd)
{
	return ht16k33_send_commands_blocking(&cmd, 1);
}

static bool ht16k33_display_on()
{
	return ht16k33_send_command(HT16K33_ON) &&
		   ht16k33_send_command(HT16K33_DISPLAYON) &&
		   ht16k33_send_command(HT16K33_BRIGHTNESS | ht16k33_state.brightness);
}

static uint8_t ht16k33_get_seg_pos(uint8_t seg_num)
{
	return seg_num * 2;
}

static bool ht16k33_display_digit_at_pos(uint8_t digit, uint8_t pos)
{
	uint8_t buff[2] = { ht16k33_get_seg_pos(pos),
						ht16k33_map[digit] };
	return ht16k33_send_commands_blocking(buff, sizeof(buff));
}

bool ht16k33_display_int(uint16_t num)
{
	for (uint8_t idx = 0; idx < HT16K33_SEG_TOTAL; idx++) {
		//skip the ":" in the middle
		if (idx == 2) {
			continue;
		}
		if (!ht16k33_display_digit_at_pos(num % 10, HT16K33_SEG_NUM - idx)) {
			return false;
		}
		num /= 10;
	}
	return true;
}

static bool ht16k33_display_clear()
{
	//clear all and colon in the middle
	for (uint8_t idx = 0; idx < HT16K33_SEG_TOTAL; idx++) {
		if (!ht16k33_display_digit_at_pos(HT16K33_EMPTY, HT16K33_SEG_NUM - idx)) {
			return false;
		}
	}
	return true;
}

static bool ht16k33_reset()
{
	return ht16k33_is_connected() &&
		   ht16k33_display_on() &&
		   ht16k33_display_clear();
}

void ht16k33_init(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t brightness)
{
	if (ht16k33_state.is_init) {
		return;
	}

	ht16k33_state.is_init = true;

	ht16k33_state.hi2c = hi2c;

	//address needs to be left shifted by 1 for the HAL interfaces
	ht16k33_state.addr = addr << 1;

	ht16k33_state.brightness = brightness;

	ht16k33_reset();
}

