/*
 * ws2812.h
 *
 *  Created on: 4 paü 2020
 *      Author: kurza
 */

#ifndef COMPONENTS_WS2812_WS2812_H_
#define COMPONENTS_WS2812_WS2812_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>

#include <driver/rmt.h>
#include <driver/gpio.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef void *ws2812_handle_t;

#ifdef NEO_KHZ400
typedef uint16_t neoPixelType; ///< 3rd arg to Adafruit_NeoPixel constructor
#else
typedef uint8_t  neoPixelType; ///< 3rd arg to Adafruit_NeoPixel constructor
#endif

typedef struct
{
	uint8_t white;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}led_color_t;

typedef struct
{
	//const enum rgb_led_type_t rgb_led_type;
	const neoPixelType rgb_led_type;
	uint32_t led_strip_length;

	// RMT peripheral settings
	rmt_channel_t rmt_channel;

	/*
	 * Interrupt table is located in soc.h
	 * As of 11/27/16, reccomended interrupts are:
	 * 9, 12, 13, 17, 18, 19, 20, 21 or 23
	 * Ensure that the same interrupt number isn't used twice
	 * across all libraries
	 */
	//int rmt_interrupt_num;
	gpio_num_t gpio;// Must be less than GPIO_NUM_33

	// Double buffering elements
	bool showing_buf_1;
	led_color_t *led_strip_buf_1;
	led_color_t *led_strip_buf_2;

	SemaphoreHandle_t access_semaphore;
	uint8_t rOffset;///< Red index within each 3- or 4-byte pixel
	uint8_t gOffset;///< Index of green byte
	uint8_t bOffset;///< Index of blue byte
	uint8_t wOffset;///< Index of white (==rOffset if no white)
	bool hasWhite;
}led_strip_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* COMPONENTS_WS2812_WS2812_H_ */
