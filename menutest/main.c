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

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "gfx.h"

static GConsoleObject			gc;
static GButtonObject			gNext;
static GButtonObject			gPrev;
static GListener				gl;

/*------------------------------------------------------------------------*
 * GINPUT Touch Driver Calibrator.                                        *
 *------------------------------------------------------------------------*/
int main(void) {
	GSourceHandle			gs, gsNext, gsPrev;
	GEvent					*pe;
	GEventGWinButton		*peb;
	coord_t					swidth, sheight;
	GHandle					ghc, ghNext, ghPrev;
	font_t					font;

	halInit();		// Initialise the Hardware
	chSysInit();		// Initialize the OS
	gdispInit();		// Initialize the display

	// Get the display dimensions
	swidth = gdispGetWidth();
	sheight = gdispGetHeight();
	ghNext = ghPrev = 0;

	// Create our title
	//gdispFillStringBox(0, 0, swidth, 20, "Touch Calibration", font, Red, White, justifyLeft);

	// Create our main display window
	//ghc = gwinCreateConsole(&gc, 0, 20, swidth, sheight-20, font);
	//gwinClear(ghc);
	//gp = gwinGetConsoleStream(ghc);

	geventListenerInit(&gl);
	gs = ginputGetMouse(0);
	geventAttachSource(&gl, gs, GLISTEN_MOUSEDOWNMOVES|GLISTEN_MOUSEMETA);

        //gdispSetOrientation(GDISP_ROTATE_90);
	gdispClear(Blue); 

	font = gdispOpenFont("UI2");

	ghNext = gwinCreateButton(&gNext, swidth-50, 10, 50, 40, font, GBTN_NORMAL);
	gwinSetButtonText(ghNext, "Next", FALSE);
	gsNext = gwinGetButtonSource(ghNext);
	geventAttachSource(&gl, gsNext, 0);
	gwinAttachButtonMouseSource(ghNext, gs);

	ghPrev = gwinCreateButton(&gPrev, swidth-100, 10, 50, 40, font, GBTN_NORMAL);
	gwinSetButtonText(ghPrev, "Back", FALSE);
	gsPrev = gwinGetButtonSource(ghPrev);
	geventAttachSource(&gl, gsPrev, 0);
	gwinAttachButtonMouseSource(ghPrev, gs);

	gdispFillStringBox(0, 0, swidth, 40, "Touch Calibration", font, Green, White, justifyLeft);
	gwinButtonDraw(ghNext);
	gwinButtonDraw(ghPrev);

	while(1) {
	while(1) {
		pe = geventEventWait(&gl, TIME_INFINITE);
		if (pe->type == GEVENT_GWIN_BUTTON) {
			peb = (GEventGWinButton *)pe;
			if (peb->button == ghPrev) {
				gdispClear(Red);
	gwinButtonDraw(ghNext);
	gwinButtonDraw(ghPrev);
}
			if (peb->button == ghNext) {
				gdispClear(Blue); 
	gwinButtonDraw(ghNext);
	gwinButtonDraw(ghPrev);
}
		}
	}
	}
}
