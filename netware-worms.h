/***************************************************************************
*
*   Copyright(c) Jeff V. Merkey 1997-2022.  All rights reserved.
*
*   Licensed under the MIT/X License
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to 
*   deal in the Software without restriction, including without limitation 
*   the rights to use, copy, modify, merge, publish, distribute, sublicense, 
*   and/or sell copies of the Software, and to permit persons to whom the 
*   Software is furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included 
*   in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
*   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
*   DEALINGS IN THE SOFTWARE.
*
*   NetWare SMP style worm screesnsaver for Linux using ncurses
*
*   USAGE:  netware-worms [cpus|speedup]
*       i.e. worm cpus=<num=8 to 256> speedup=<divisor=1|2|3|4>
*   examples:
*       netware-worms cpus=8\n");
*          start worm screensaver with 8 worm threads
*       netware-worms speedup=4\n");
*          run worm screensaver at 4X speed
*
**************************************************************************/

#define WORM_MIN_LEN    4
#define WORM_MAX_LEN   36
#define WORM_TAIL_LEN   3
#define MAX_WORMS      64

// adjust worm total length based on screen area size.  smaller
// displays have shorter worms.  the current logic adjusts for
// worm speedup based on the current max worm length.  Here we
// check a min and max screen area for worm length expansion.

#define AREA_BASE_LEN   (WORM_MAX_LEN / 2)
#define AREA_MINLINES   19
#define AREA_MINCOLS    80
#define AREA_MIN        (AREA_MINLINES * AREA_MINCOLS)
#define AREA_MAXLINES   64
#define AREA_MAXCOLS    160
#define AREA_MAX        (AREA_MAXLINES * AREA_MAXCOLS)
#define AREA            ((COLS * LINES) < (AREA_MINLINES * AREA_MINCOLS) \
			 ? (AREA_MINLINES * AREA_MINCOLS) : (COLS * LINES))
#define AREA_DIVISOR    ((AREA_MAX - AREA_MIN) / AREA_BASE_LEN)
#define AREA_EXT_LEN    (((AREA - (AREA_MIN)) / (AREA_DIVISOR)) > \
			AREA_BASE_LEN ? AREA_BASE_LEN : \
			((AREA - (AREA_MIN)) / (AREA_DIVISOR)))

#define MAX_LOADAVG   100
#define MAX_NANOSEC   100000000
#define MIN_NANOSEC   10000000
#define MAX_MICROSEC  100000
#define MIN_MICROSEC  10000

typedef struct _WORM
{
    int cpu;
    int count;
    int limit;
    int x[WORM_MAX_LEN];
    int y[WORM_MAX_LEN];
    int x_prev[WORM_MAX_LEN];
    int y_prev[WORM_MAX_LEN];
    int length;
    int length_prev;
    int direction;
    int runlength;
    int windowlength;
} WORM;

typedef struct _STATE
{
    int cpus;
    int delay;
    int divisor;
    int rows;
    int cols;
    unsigned long long usr[MAX_WORMS];
    unsigned long long sys[MAX_WORMS];
    unsigned long long nice[MAX_WORMS];
    unsigned long long idle[MAX_WORMS];
    unsigned long long io[MAX_WORMS];
    unsigned long long irq[MAX_WORMS];
    unsigned long long sirq[MAX_WORMS];
    unsigned long long steal[MAX_WORMS];
    unsigned long long guest[MAX_WORMS];
    unsigned long long guest_nice[MAX_WORMS];
    WORM *worms;
} STATE;

int netware_screensaver(int cpus, int speedup);
