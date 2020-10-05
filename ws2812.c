/*
 * ws2812.c
 *
 *  Created on: 4 paü 2020
 *      Author: kurza
 */

/*********************
 *      INCLUDES
 *********************/


#include "ws2812.h"

/*********************
 *      DEFINES
 *********************/
#define LED_STRIP_RMT_TICKS_BIT_1_HIGH_WS2812 9 // 900ns (900ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_1_LOW_WS2812  3 // 300ns (350ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_0_HIGH_WS2812 3 // 300ns (350ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_0_LOW_WS2812  9 // 900ns (900ns +/- 150ns per datasheet)

/**********************
 *      TYPEDEFS
 **********************/
// Function pointer for generating waveforms based on different LED drivers
typedef void (*led_fill_rmt_items_fn)(led_color_t *led_strip_buf, rmt_item32_t *rmt_items, uint32_t led_strip_length, uint8_t wOffset, uint8_t rOffset, uint8_t gOffset, uint8_t bOffset);

/**********************
 *  STATIC PROTOTYPES
 **********************/
static inline void led_strip_fill_item_level(rmt_item32_t *item, int high_ticks, int low_ticks);
static inline void led_strip_rmt_bit_1_ws2812(rmt_item32_t *item);
static inline void led_strip_rmt_bit_0_ws2812(rmt_item32_t *item);
static void led_strip_fill_rmt_items_ws2812(led_color_t *led_strip_buf, rmt_item32_t *rmt_items, uint32_t led_strip_length, uint8_t wOffset, uint8_t rOffset, uint8_t gOffset, uint8_t bOffset);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void led_strip_fill_rmt_items_ws2812(led_color_t *led_strip_buf, rmt_item32_t *rmt_items, uint32_t led_strip_length, uint8_t wOffset, uint8_t rOffset, uint8_t gOffset, uint8_t bOffset)
{
	uint32_t rmt_items_index = 0;

	for (uint32_t led_index = 0;
			led_index < led_strip_length; led_index++)
	{
		led_color_t led_color = led_strip_buf[led_index];
		uint8_t bytes[4] =
			{
				led_color.red,
				led_color.green,
				led_color.blue,
				led_color.white 
			};

		for (uint8_t bit = 8; bit != 0; bit--)
		{
			uint8_t bit_set = (bytes[wOffset] >> (bit - 1)) & 1;
			if (bit_set)
			{
				led_strip_rmt_bit_1_ws2812(&(rmt_items[rmt_items_index]));
			}
			else
			{
				led_strip_rmt_bit_0_ws2812(&(rmt_items[rmt_items_index]));
			}
			rmt_items_index++;
		}
		for (uint8_t bit = 8; bit != 0; bit--)
		{
			uint8_t bit_set = (bytes[gOffset] >> (bit - 1)) & 1;
			if (bit_set)
			{
				led_strip_rmt_bit_1_ws2812(&(rmt_items[rmt_items_index]));
			}
			else
			{
				led_strip_rmt_bit_0_ws2812(&(rmt_items[rmt_items_index]));
			}
			rmt_items_index++;
		}
		for (uint8_t bit = 8; bit != 0; bit--)
		{
			uint8_t bit_set = (bytes[bOffset] >> (bit - 1)) & 1;
			if (bit_set)
			{
				led_strip_rmt_bit_1_ws2812(&(rmt_items[rmt_items_index]));
			}
			else
			{
				led_strip_rmt_bit_0_ws2812(&(rmt_items[rmt_items_index]));
			}
			rmt_items_index++;
		}
		if (wOffset != rOffset)
		{
			for (uint8_t bit = 8; bit != 0; bit--)
			{
				uint8_t bit_set = (bytes[wOffset] >> (bit - 1)) & 1;
				if (bit_set)
				{
					led_strip_rmt_bit_1_ws2812(&(rmt_items[rmt_items_index]));
				}
				else
				{
					led_strip_rmt_bit_0_ws2812(&(rmt_items[rmt_items_index]));
				}
				rmt_items_index++;
			}
		}
	}
}

static inline void led_strip_fill_item_level(rmt_item32_t *item, int high_ticks, int low_ticks)
{
	item->level0 = 1;
	item->duration0 = high_ticks;
	item->level1 = 0;
	item->duration1 = low_ticks;
}

static inline void led_strip_rmt_bit_1_ws2812(rmt_item32_t *item)
{
	led_strip_fill_item_level(item, LED_STRIP_RMT_TICKS_BIT_1_HIGH_WS2812, LED_STRIP_RMT_TICKS_BIT_1_LOW_WS2812);
}

static inline void led_strip_rmt_bit_0_ws2812(rmt_item32_t *item)
{
	led_strip_fill_item_level(item, LED_STRIP_RMT_TICKS_BIT_0_HIGH_WS2812, LED_STRIP_RMT_TICKS_BIT_0_LOW_WS2812);
}
