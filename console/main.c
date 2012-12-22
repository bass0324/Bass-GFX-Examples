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
 *	The following stuff is optional in your gfxconf.h:
 *
 *				#define GDISP_NEED_SCROLL	TRUE	(optional but recommended if your GDISP driver supports it)
 */

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "gfx.h"

/* The handles for our three consoles */
GHandle GW1, GW2, GW3;

/* The streams for our three consoles */
BaseSequentialStream *S1, *S2, *S3;

static WORKING_AREA(waThread1, 512);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("S1");
  while (TRUE) {
	chprintf(S1, "Hello ChibiOS/GFX!\r\n");
	chThdSleepMilliseconds(2500);
  }
}

static WORKING_AREA(waThread2, 512);
static msg_t Thread2(void *arg) {

  (void)arg;
  int i = 0;
  chRegSetThreadName("S2");
  while (TRUE) {
	chprintf(S2, "Message Nr.: %d\r\n", i++);
	chThdSleepMilliseconds(1000);
  }
}

static WORKING_AREA(waThread3, 512);
static msg_t Thread3(void *arg) {

  (void)arg;
  int i = 0;
  chRegSetThreadName("S3");
  while (TRUE) {
        chprintf(S3, "Message Nr.: %d\r\n", i++);
	chThdSleepMilliseconds(1500);
  }
}


int main(void) {
	uint8_t i;
	font_t	font1, font2;

	halInit();
	chSysInit();

	/* initialize and clear the display */
	gdispInit();
	gdispClear(Black);
	font1 = gdispOpenFont("UI2 Double");
	font2 = gdispOpenFont("Small");

	/* create the three console windows and set a font for each */
	GW1 = gwinCreateConsole(NULL, 0, 0, gdispGetWidth(), gdispGetHeight()/2, font1);
	GW2 = gwinCreateConsole(NULL, 0, gdispGetHeight()/2, gdispGetWidth()/2, gdispGetHeight(), font2);
	GW3 = gwinCreateConsole(NULL, gdispGetWidth()/2, gdispGetHeight()/2, gdispGetWidth(), gdispGetHeight(), font2);

	/* Set the fore- and background colors for each console */
	gwinSetColor(GW1, Green);
	gwinSetBgColor(GW1, Black);
	gwinSetColor(GW2, White);
	gwinSetBgColor(GW2, Blue);
	gwinSetColor(GW3, Black);
	gwinSetBgColor(GW3, Red);

	/* clear all console windows - to set background */
	gwinClear(GW1);
	gwinClear(GW2);
	gwinClear(GW3);

	/* receive the stream pointers of each console */
	S1 = gwinGetConsoleStream(GW1);
	S2 = gwinGetConsoleStream(GW2);
	S3 = gwinGetConsoleStream(GW3);

	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
	chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);
	chThdCreateStatic(waThread3, sizeof(waThread3), NORMALPRIO, Thread3, NULL);

	while(TRUE) {
		chThdSleepMilliseconds(500);
	}
}

