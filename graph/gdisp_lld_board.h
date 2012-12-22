/*
    ChibiOS/GFX - Copyright (C) 2012
                 Joel Bodenmann aka Tectu <joel@unormal.org>

    This file is part of ChibiOS/GFX.

    ChibiOS/GFX is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/GFX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    drivers/gdisp/SSD1289/gdisp_lld_board_firebullstm32f103.h
 * @brief   GDISP Graphic Driver subsystem board interface for the SSD1289 display.
 *
 * @addtogroup GDISP
 * @{
 */

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

#define SET_CS		palSetPad(GPIOD, 7)
#define CLR_CS		palClearPad(GPIOD, 7)

#define SET_RS		palSetPad(GPIOD, 11)
#define CLR_RS		palClearPad(GPIOD, 11)

#define SET_WR		palSetPad(GPIOD, 8)
#define CLR_WR		palClearPad(GPIOD, 8)

#define SET_RD		palSetPad(GPIOD, 9)
#define CLR_RD		palClearPad(GPIOD, 9)

#define SET_RST		palSetPad(GPIOD, 10)
#define CLR_RST		palClearPad(GPIOD, 10)

/*
CS			PD7
RST			PD10
RS			PD11
WR			PD8
RD			PD9

DB[0:3]		PD[0:3]
DB[4:15]	PE[4:15]

*/

/**
 * @brief   Initialise the board for the display.
 * @notes	This board definition uses GPIO and assumes exclusive access to these GPIO pins
 *
 * @notapi
 */
static __inline void init_board(void) {

    palSetGroupMode(GPIOD, 0x000F, 0, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
    palSetGroupMode(GPIOE, 0xFFF0, 0, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);

    palSetPadMode(GPIOD, 7, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOD, 8, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOD, 9, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOD, 10, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOD, 11, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	
    // Configure the pins to a well know state
    SET_CS;
    CLR_RS;
    SET_RD;
    SET_WR;
    //CLR_CS;
}


/**
 * @brief   Set or clear the lcd reset pin.
 *
 * @param[in] state		TRUE = lcd in reset, FALSE = normal operation
 * 
 * @notapi
 */
static __inline void setpin_reset(bool_t state) {
    (void) state;
    if(state)
        CLR_RST;
    else
        SET_RST;
}

/**
 * @brief   Set the lcd back-light level.
 *
 * @param[in] percent		0 to 100%
 * 
 * @notapi
 */
static __inline void set_backlight(uint8_t percent) {
	(void) percent;
	/* Nothing to do here - Backlight always on */
}

/**
 * @brief   Take exclusive control of the bus
 *
 * @notapi
 */
static __inline void acquire_bus(void) {
	/* Nothing to do here since LCD is the only device on that bus */
    CLR_CS;
}

/**
 * @brief   Release exclusive control of the bus
 *
 * @notapi
 */
static __inline void release_bus(void) {
	/* Nothing to do here since LCD is the only device on that bus */
    SET_CS;
}

// #define lcd_lld_delay asm volatile("nop"); asm volatile("nop");
#define lcd_lld_delay halPolledDelay(1)

/**
 * @brief   Send data to the index register.
 *
 * @param[in] index		The index register to set
 *
 * @notapi
 */
static __inline void write_index(uint16_t index) {
    CLR_RS;
    GPIOD->BSRR.W=((~index&0x000F)<<16)|(index&0x000F);
    GPIOE->BSRR.W=((~index&0xFFF0)<<16)|(index&0xFFF0);
    CLR_WR;
    //CLR_CS;
    lcd_lld_delay;
    //SET_CS;
    SET_WR;
}

/**
 * @brief   Send data to the lcd.
 *
 * @param[in] data		The data to send
 * 
 * @notapi
 */
static __inline void write_data(uint16_t data) {
    SET_RS;
    GPIOD->BSRR.W=((~data&0x000F)<<16)|(data&0x000F);
    GPIOE->BSRR.W=((~data&0xFFF0)<<16)|(data&0xFFF0);
    CLR_WR;
    //CLR_CS;
    lcd_lld_delay;
    //SET_CS;
    SET_WR;
}

#if GDISP_HARDWARE_READPIXEL || GDISP_HARDWARE_SCROLL || defined(__DOXYGEN__)
/**
 * @brief   Read data from the lcd.
 *
 * @return	The data from the lcd
 * @note	The chip select may need to be asserted/de-asserted
 * 			around the actual spi read
 * 
 * @notapi
 */
static __inline uint16_t read_data(void) {
    uint16_t value;
	
    palSetGroupMode(GPIOD, 0x000F, 0, PAL_MODE_INPUT);
    palSetGroupMode(GPIOE, 0xFFF0, 0, PAL_MODE_INPUT);

    SET_RS;
    CLR_RD;
    lcd_lld_delay;
    value = (GPIOE->IDR&0xFFF0)|(GPIOD->IDR&0x000F);
    GPIOD->ODR=GPIOD->IDR;
    GPIOE->ODR=GPIOE->IDR;
    SET_RD;
    lcd_lld_delay;

    palSetGroupMode(GPIOD, 0x000F, 0, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
    palSetGroupMode(GPIOE, 0xFFF0, 0, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);

    return value;
}
#endif

#endif /* _GDISP_LLD_BOARD_H */
/** @} */
