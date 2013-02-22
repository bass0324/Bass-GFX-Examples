/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "usb_cdc.h"
#include "shell.h"
#include "chprintf.h"

#include "gfx.h"

#include "gdisp_lld_board.h"

#define COLOR_SIZE 20
#define PEN_SIZE 20
#define OFFSET 3

#define COLOR_BOX(a)            (ev.x >= a && ev.x <= a + COLOR_SIZE)
#define PEN_BOX(a)              (ev.y >= a && ev.y <= a + COLOR_SIZE)
#define GET_COLOR(a)            (COLOR_BOX(a * COLOR_SIZE + OFFSET))
#define GET_PEN(a)              (PEN_BOX(a * 2 * PEN_SIZE + OFFSET))
#define DRAW_COLOR(a)           (a * COLOR_SIZE + OFFSET)
#define DRAW_PEN(a)             (a * 2 * PEN_SIZE + OFFSET)
#define DRAW_AREA(x, y)         (x >= PEN_SIZE + OFFSET + 3 && x <= gdispGetWidth() && \
                                 y >= COLOR_SIZE + OFFSET + 3 && y <= gdispGetHeight())

// Vecchia versione
#if 0
// Peripherial Clock 42MHz SPI2 SPI3
// Peripherial Clock 84MHz SPI1                                SPI1        SPI2/3
#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000) //  42 MHz      21 MHZ
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0008) //  21 MHz      10.5 MHz
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0010) //  10.5 MHz    5.25 MHz
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018) //  5.25 MHz    2.626 MHz  
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020) //  2.626 MHz   1.3125 MHz
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028) //  1.3125 MHz  656.25 KHz
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030) //  656.25 KHz  328.125 KHz
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038) //  328.125 KHz 164.06 KHz

static const SPIConfig spicfg = { NULL, TP_CS_PORT, TP_CS_PIN, SPI_BaudRatePrescaler_32 };

static const TouchscreenDriver TOUCHPADD1 = { &SPID2, &spicfg, TP_IRQ_PORT, TP_IRQ_PIN, TRUE };
#endif

void drawScreen(void);

GEventMouse             ev;

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    int time;

    time = 500;
    palClearPad(GPIOC, 7);
	palClearPad(GPIOC, 8);
	palClearPad(GPIOC, 9);
    chThdSleepMilliseconds(time);
    palSetPad(GPIOC, 7);
	palSetPad(GPIOC, 8);
	palSetPad(GPIOC, 9);
    chThdSleepMilliseconds(time);
  }
}

void drawScreen(void) {
        char *msg = "Notepad/GFX";
        font_t          font1, font2;

        font1 = gdispOpenFont("UI2 Double");
        font2 = gdispOpenFont("LargeNumbers");

        gdispClear(White);
        //gdispFillArea(0,0,320,240,White);
        gdispDrawString(gdispGetWidth()-gdispGetStringWidth(msg, font1)-3, 3, msg, font1, Black);

        /* colors */
        gdispFillArea(0 * COLOR_SIZE + 3, 3, COLOR_SIZE, COLOR_SIZE, Black);    /* Black */
        gdispFillArea(1 * COLOR_SIZE + 3, 3, COLOR_SIZE, COLOR_SIZE, Red);              /* Red */
        gdispFillArea(2 * COLOR_SIZE + 3, 3, COLOR_SIZE, COLOR_SIZE, Yellow);   /* Yellow */
        gdispFillArea(3 * COLOR_SIZE + 3, 3, COLOR_SIZE, COLOR_SIZE, Green);    /* Green */
        gdispFillArea(4 * COLOR_SIZE + 3, 3, COLOR_SIZE, COLOR_SIZE, Blue);             /* Blue */
        gdispDrawBox (5 * COLOR_SIZE + 3, 3, COLOR_SIZE, COLOR_SIZE, Black);    /* White */

        /* pens */
        gdispDrawString(OFFSET * 2, DRAW_PEN(1), "1", font2, Black);
        gdispDrawString(OFFSET * 2, DRAW_PEN(2), "2", font2, Black);
        gdispDrawString(OFFSET * 2, DRAW_PEN(3), "3", font2, Black);
        gdispDrawString(OFFSET * 2, DRAW_PEN(4), "4", font2, Black);
        gdispDrawString(OFFSET * 2, DRAW_PEN(5), "5", font2, Black);

        gdispCloseFont(font1);
        gdispCloseFont(font2);
}

/*
 * Application entry point.
 */
int main(void) {
    color_t color = Black;
    uint16_t pen = 0;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
    halInit();
    chSysInit();

    gdispInit();
    ginputGetMouse(0);
    gdispSetOrientation(GDISP_ROTATE_90);

    drawScreen();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  
  while (TRUE) {

    ginputGetMouseStatus(0, &ev);
    if (!(ev.current_buttons & GINPUT_MOUSE_BTN_LEFT))
    {
        chThdSleepMicroseconds(500); // Senza questo sleep l'USB non parte
        continue;
    }

    /* inside color box ? */
    if(ev.y >= OFFSET && ev.y <= COLOR_SIZE) {
      if(GET_COLOR(0))   color = Black;
      else if(GET_COLOR(1))   color = Red;
      else if(GET_COLOR(2))   color = Yellow;
      else if(GET_COLOR(3))   color = Green;
      else if(GET_COLOR(4))   color = Blue;
      else if(GET_COLOR(5))   color = White;

    /* inside pen box ? */
    } else if(ev.x >= OFFSET && ev.x <= PEN_SIZE) {
      if(GET_PEN(1))             pen = 0;
      else if(GET_PEN(2))             pen = 1;
      else if(GET_PEN(3))             pen = 2;
      else if(GET_PEN(4))             pen = 3;
      else if(GET_PEN(5))             pen = 4;

    /* inside drawing area ? */
    } else if(DRAW_AREA(ev.x, ev.y)) {
      if(pen == 0)
         gdispDrawPixel(ev.x, ev.y, color);
      else
          gdispFillCircle(ev.x, ev.y, pen, color);
    }
  }
}
