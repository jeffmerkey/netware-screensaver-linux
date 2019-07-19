/***************************************************************************
*
*   Copyright(c) Jeff V. Merkey 1997-2019.  All rights reserved.
*
*   Portions adapted from xscreensaver loadsnake program is
*   portions Copyright (c) 2007-2011 Cosimo Streppone <cosimo@cpan.org>
*
*   Licensed under the GNU Public License v2.
*
*   Permission to use, copy, modify, distribute, and sell this software and its
*   documentation for any purpose is hereby granted without fee, provided that
*   the above copyright notice appear in all copies and that both that
*   copyright notice and this permission notice appear in supporting
*   documentation.  No representations are made about the suitability of
*   this software for any purpose.  It is provided "as is" without express or
*   implied warranty.
*
*   NetWare SMP style worm screesnsaver for Linux using ncurses
*
*   USAGE:  netware-worms [cpus|speedup]
*       i.e. worm cpus=<num=8 to 256> speedup=<divisor=1|2|3|4>
*   examples:
*       netware-worms cpus=8;
*          start worm screensaver with 8 worm threads
*       netware-worms speedup=4
*          run worm screensaver at 4X speed
*
**************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <termios.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <sys/ioctl.h>

#include "netware-worms.h"

#ifndef LONGLONG
typedef long long LONGLONG;
#endif
#ifndef ULONG
typedef unsigned long  ULONG;
#endif
#ifndef WORD
typedef unsigned short WORD;
#endif
#ifndef BYTE
typedef unsigned char  BYTE;
#endif

// 6845 color codes for PC displays
#define BLINK		0x80
#define	BLACK		0x00
#define BLUE		0x01
#define GREEN		0x02
#define CYAN		0x03
#define RED		0x04
#define MAGENTA		0x05
#define BROWN		0x06
#define WHITE		0x07
#define	GRAY		0x08
#define LTBLUE		0x09
#define LTGREEN		0x0A
#define LTCYAN		0x0B
#define LTRED		0x0C
#define LTMAGENTA	0x0D
#define YELLOW		0x0E
#define BRITEWHITE	0x0F
#define	BGBLACK		0x00
#define BGBLUE		0x10
#define BGGREEN		0x20
#define BGCYAN		0x30
#define BGRED		0x40
#define BGMAGENTA	0x50
#define BGBROWN		0x60
#define BGWHITE		0x70
#define UP_CHAR         0x1E
#define DOWN_CHAR       0x1F

static ULONG worm_chars[] =
{
   (219 | A_ALTCHARSET),
   (178 | A_ALTCHARSET),
   (177 | A_ALTCHARSET),
   (176 | A_ALTCHARSET),
};

static ULONG worm_colors[]=
{
   (LTRED | BGBLACK),
   (BLUE | BGBLACK),
   (LTGREEN | BGBLACK),
   (LTCYAN | BGBLACK),
   (YELLOW | BGBLACK),
   (BRITEWHITE | BGBLACK),
   (MAGENTA | BGBLACK),
   (BROWN | BGBLACK),
   (RED | BGBLACK),
   (LTBLUE | BGBLACK),
   (LTMAGENTA | BGBLACK),
   (GRAY | BGBLACK),
   (LTRED | BGBLACK),
   (WHITE | BGBLACK),
   (GREEN | BGBLACK),
   (CYAN | BGBLACK),
};

static int has_color;

//  Here we attempt to map the ncurses color pair numbers into
//  something a little more PC friendly.

static int color_map[128]=
{
    1,  2,  3,  4,  5,  6,  7,  8, 8,  2,  3,  4,  5,  6,  7,  8,
    9, 10, 11, 12, 13, 14, 15, 16, 16, 10, 11, 12, 13, 14, 15, 16,
   17, 18, 19, 20, 21, 22, 23, 24, 24, 18, 19, 20, 21, 22, 23, 24,
   25, 26, 27, 28, 29, 30, 31, 32, 32, 26, 27, 28, 29, 30, 31, 32,
   33, 34, 35, 36, 37, 38, 39, 40, 40, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48, 48, 42, 43, 44, 45, 46, 47, 48,
   49, 50, 51, 52, 53, 54, 55, 56, 56, 50, 51, 52, 53, 54, 55, 56,
   57, 58, 59, 60, 61, 62, 53, 64, 64, 58, 59, 60, 61, 62, 53, 64
};

static int attr_map[128]=
{
   0, 0, 0, 0, 0, 0, 0, A_BOLD, 0, A_BOLD, A_BOLD, A_BOLD,
   A_BOLD, A_BOLD, A_BOLD, A_BOLD,
   0, 0, 0, 0, 0, 0, 0, A_BOLD, 0, A_BOLD, A_BOLD, A_BOLD,
   A_BOLD, A_BOLD, A_BOLD, A_BOLD,
   0, 0, 0, 0, 0, 0, 0, A_BOLD, 0, A_BOLD, A_BOLD, A_BOLD,
   A_BOLD, A_BOLD, A_BOLD, A_BOLD,
   0, 0, 0, 0, 0, 0, 0, A_BOLD, 0, A_BOLD, A_BOLD, A_BOLD,
   A_BOLD, A_BOLD, A_BOLD, A_BOLD,
   0, 0, 0, 0, 0, 0, 0, A_BOLD, 0, A_BOLD, A_BOLD, A_BOLD,
   A_BOLD, A_BOLD, A_BOLD, A_BOLD,
   0, 0, 0, 0, 0, 0, 0, A_BOLD, 0, A_BOLD, A_BOLD, A_BOLD,
   A_BOLD, A_BOLD, A_BOLD, A_BOLD,
   0, 0, 0, 0, 0, 0, 0, A_BOLD, 0, A_BOLD, A_BOLD, A_BOLD,
   A_BOLD, A_BOLD, A_BOLD, A_BOLD,
   0, 0, 0, 0, 0, 0, 0, A_BOLD, 0, A_BOLD, A_BOLD, A_BOLD,
   A_BOLD, A_BOLD, A_BOLD, A_BOLD
};

static ULONG get_color_pair(ULONG attr)
{
   return ((COLOR_PAIR(color_map[attr & 0x7F]) |
	  attr_map[attr & 0x7F] | ((attr & BLINK) ? A_BLINK : 0)));
}

static void set_color(ULONG attr)
{
    if (has_color)
       attrset(get_color_pair(attr));
}

static void clear_color(void)
{
    attroff(A_BOLD | A_BLINK | A_REVERSE);
}

static int init_ncurses()
{
     register int i, pair, ret;
     unsigned long w;
     FILE *f;
     char wait[100], *s;
     int bg_colors[8]=
     {
        COLOR_BLACK, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
        COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
     };

     setlocale(LC_ALL, "");
     initscr();
     nonl();
     intrflush(stdscr, FALSE);
     keypad(stdscr, TRUE);
     noecho();

     // if the terminal does not support colors, or if the
     // terminal cannot support at least eight primary colors
     // for foreground/background color pairs, then default
     // the library to use ASCII characters < 127 (7 bit), disable
     // ncurses color attributes, and do not attempt to use
     // the alternate character set for graphic characters.

     if (has_colors())
     {
	if (start_color() == OK)
	{
	   if (COLORS >= 8)
	   {
	      has_color = TRUE;
              pair = 1;

              // We create our color pairs in the order defined
              // by the PC based text attribute color scheme.  We do
              // this to make it relatively simple to use a table
              // driven method for mapping the PC style text attributes
              // to ncurses.

              for (i=0; i < 8; i++)
              {
	         init_pair(pair++, COLOR_BLACK, bg_colors[i]);
	         init_pair(pair++, COLOR_BLUE, bg_colors[i]);
	         init_pair(pair++, COLOR_GREEN, bg_colors[i]);
	         init_pair(pair++, COLOR_CYAN, bg_colors[i]);
	         init_pair(pair++, COLOR_RED, bg_colors[i]);
	         init_pair(pair++, COLOR_MAGENTA, bg_colors[i]);
	         init_pair(pair++, COLOR_YELLOW, bg_colors[i]);
	         init_pair(pair++, COLOR_WHITE, bg_colors[i]);
	      }
	   }
	}
     }

     wclear(stdscr);
     curs_set(0);  // turn off the cursor
     refresh();

     f = fopen("/sys/module/kernel/parameters/consoleblank", "r");
     if (f != NULL)
     {
        s = fgets(wait, 98, f);
        if (s) {
           sscanf(wait, "%lu", &w);
#if VERBOSE
	   printf("console blank timer: %lu\n", w);
#endif
	}
	fclose(f);
     }
     // disable screen blanking
     if (w) {
        ret = system("setterm -blank 0");
        if (ret < 0)
	   return ret;
     }
     return 0;
}

static int clear_ncurses()
{
    curs_set(1);  // turn on the cursor
    endwin();

    // reset terminal escape sequence
    printf("%c%c", 0x1B, 'c');

    // enable screen blanking
#if 0
    system("setterm -blank 10");
#endif
    return 0;
}

// this function remaps single byte box and line characters into utf8
// unicode characters for display on wide character terminals.  This
// allows the program to store multi byte characters as single byte
// ASCII codes for ncurses.

static void mvputc(ULONG row, ULONG col, const chtype ch)
{
   switch (ch & 0xFF)
   {
      // solid block
      case 219:
         mvprintw(row, col, "\u2588");
         break;
      // dark shade block
      case 178:
         mvprintw(row, col, "\u2593");
         break;
      // medium shade block
      case 177:
         mvprintw(row, col, "\u2592");
         break;
      // light shade block
      case 176:
         mvprintw(row, col, "\u2591");
         break;

      default:
         mvaddch(row, col, ch);
         break;
    }
    return;
}

static void worm_put_char(int c, long row, long col, ULONG attr)
{
    if (col >= COLS)
       return;
    if (row >= LINES)
       return;
    set_color(attr);
    mvputc(row, col, c);
    clear_color();
    return;
}

static int worm_kbhit(void)
{
   int STDIN = 0, bytes = 0;
   struct termios init;
   struct termios term;

   // setup the terminal to check
   // for pending keystrokes
   tcgetattr(STDIN, &init);
   tcgetattr(STDIN, &term);
   term.c_lflag &= ~ICANON;
   tcsetattr(STDIN, TCSANOW, &term);

   setbuf(stdin, NULL);
   ioctl(STDIN, FIONREAD, &bytes);
   // set the terminal to default
   tcsetattr(STDIN, TCSANOW, &init);
   return bytes;
}

static int get_processors(void)
{
    int cpus = 0;
    cpus = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpus < 1)
        cpus = 1;
    return (cpus);
}

static int get_cpu_load(STATE *st, int cpu)
{
    static char line[100];
    int p_usr = 0, p_nice = 0, p_sys  = 0, p_idle = 0, load = 0, len;
    FILE *f;
    char src[100] = "\0", *s;

    if (cpu > st->cpus)
        return 0;

    if (cpu > MAX_WORMS)
        return 0;

    // convert cpu num to ascii text number
    // and null terminate
    snprintf(src, 100, "cpu%d", cpu);
    len = strlen(src);

    if (NULL != (f = fopen("/proc/stat", "r")))
    {
        while (!feof(f) && !load)
	{
            s = fgets(line, 98, f);
            if (s && !strncasecmp(src, line, len))
	    {
		p_usr  = st->usr[cpu];
                p_nice = st->nice[cpu];
                p_sys  = st->sys [cpu];
                p_idle = st->idle[cpu];

                if (sscanf(&line[len + 1], "%d %d %d %d",
			   &(st->usr[cpu]), &(st->nice[cpu]),
			   &(st->sys[cpu]), &(st->idle[cpu])) == 4)
		{
		    // calculate total cycles
                    load = st->usr[cpu] - p_usr + st->nice[cpu] - p_nice
			    + st->sys[cpu] - p_sys + st->idle[cpu] - p_idle;

		    // prevent divide by zero if result is 0
		    if (!load)
                        load = 1;

		    // subtract idle cycles from load and mulitply * 100
		    // to express as percentage
		    load = (load - (st->idle[cpu] - p_idle)) * 100 / load;
                    break;
                }
                else
		{
	            fclose(f);
                    return 0;
		}
            }
        }
	fclose(f);
    }

    len = load * load * WORM_MAX_LEN / 10000.0;
    if (len < WORM_MIN_LEN)
        len = WORM_MIN_LEN;
#if VERBOSE
    printw("Load on cpu %d is %d%% length %d\n", cpu, load, len);
#endif
    return (len);
}

static int get_system_load(void)
{
    char load[100], *s;
    float l1 = 0;
    int l2 = 0;
    FILE *f;

    f = fopen("/proc/loadavg", "r");
    if (f != NULL)
    {
        s = fgets(load, 98, f);
        if (s) {
           sscanf(load, "%f", &l1);
#if VERBOSE
           printw("Load from /proc/loadavg is %f\n", l1);
#endif
	}
        fclose(f);
    }
    else
        l1 = 0;

    // convert from float to integer
    l1 *= 1000.0;
    l2 = (int) l1;
    l2 /= 1000;

    return l2;
}

static void move_worm(STATE *st, WORM *s)
{
    int n = 0, dir = 0;
    int x = 0, y = 0;

    /* worm head position */
    x = s->x[0];
    y = s->y[0];

    /* and direction */
    dir = s->direction;

    /* 0=up, 2=right, 4=down, 6=left */
    switch(dir)
    {
        case 0: y++;      break;
        case 1: y++; x++; break;
        case 2:      x += 2; break;
        case 3: y--; x++; break;
        case 4: y--;      break;
        case 5: y--; x--; break;
        case 6:      x -= 2; break;
        case 7: y++; x--; break;
    }

    /* Check bounds and change direction */
    if (x < 0 && (dir >= 5 && dir <= 7)) {
        x = 1;
        dir -= 4;
    }
    else if (y < 0 && (dir >= 3 && dir <= 5)) {
        y = 1;
        dir -= 4;
    }
    else if (x >= (st->cols - 2) && (dir >= 1 && dir <= 3)) {
        x = st->cols - 2;
        dir += 4;
    }
    else if (y >= st->rows && (dir == 7 || dir == 0 || dir == 1)) {
        y = st->rows - 1;
        dir += 4;
    }
    else if (s->runlength == 0) {
        int rnd;

	rnd = random() % 128;
	if(rnd > 90)
            dir += 2;
	else if(rnd == 1)
            dir++;
        else if(rnd == 2)
            dir--;
        // set this to the current worm length
	s->runlength = s->length;
    }
    else {
        int rnd;

	s->runlength--;
	rnd = random() % 128;
	if(rnd == 1)
            dir++;
        else if(rnd == 2)
            dir--;
    }

    if (dir < 0)
        dir = -dir;
    dir = dir % 8;

    s->direction = dir;

    /* Copy x,y coords in "tail" positions */
    for(n = s->length - 1; n > 0; n--) {
	s->x[n] = s->x[n-1];
        s->y[n] = s->y[n-1];
    }

    /* New head position */
    s->x[0] = x;
    s->y[0] = y;

}

static int grow_worm(STATE *st, WORM *s)
{
    int newlen = get_cpu_load(st, s->cpu);
    int len = s->length;

#if VERBOSE
    printw("grow: cpu %d len %d newlen %d\n", s->cpu, len, newlen);
#endif
    if (newlen > len) {
        int x, y;

        x = s->x[len - 1];
        y = s->y[len - 1];

        switch(s->direction) {
            case 0: y--;      break;
            case 1: y--; x--; break;
            case 2:      x -= 2; break;
            case 3: y++; x--; break;
            case 4: y++;      break;
            case 5: y++; x++; break;
            case 6:      x += 2; break;
            case 7: y--; x++; break;
        }
        len++;

        if (len >= WORM_MAX_LEN)
            len = WORM_MAX_LEN - 1;

        s->x[len] = x;
        s->y[len] = y;
    }
    else if (newlen < len) {
        len--;
        if (len < WORM_MIN_LEN)
            len = WORM_MIN_LEN;
        s->x[len + 1] = 0;
        s->y[len + 1] = 0;
    }
    s->length = len;
    return(len);
}

static void clear_worm(STATE *st, WORM *s)
{
    int n;

    for (n = s->length_prev - 1; n >= 0; n--) {
       worm_put_char(' ', s->y_prev[n], s->x_prev[n],
                  WHITE | BGBLACK);
       worm_put_char(' ', s->y_prev[n], s->x_prev[n] + 1,
                  WHITE | BGBLACK);
    }
}

/*

For drawing the worm the following set of equations map the
worm chars to create the effect of the worm moving and expanding.

The logic is non-intuitive but it is described below.  There are
four worm drawing characters in total.  The mapping is defined as:

current char position = n
div = length / 4
mod = length % 4
c = n < (div + 1) * mod ? n / (div + 1) : (n - mod) / div

the above routine produces the following output:

LENGTH    DIV/MOD     WINDOW                CHARACTER MAP
----------------------------------------------------------
length 04 div 1 mod 0 (div + 1) * mod = 00  0 1 2 3
length 05 div 1 mod 1 (div + 1) * mod = 02  0 0 1 2 3
length 06 div 1 mod 2 (div + 1) * mod = 04  0 0 1 1 2 3
length 07 div 1 mod 3 (div + 1) * mod = 06  0 0 1 1 2 2 3
length 08 div 2 mod 0 (div + 1) * mod = 00  0 0 1 1 2 2 3 3
length 09 div 2 mod 1 (div + 1) * mod = 03  0 0 0 1 1 2 2 3 3
length 10 div 2 mod 2 (div + 1) * mod = 06  0 0 0 1 1 1 2 2 3 3
length 11 div 2 mod 3 (div + 1) * mod = 09  0 0 0 1 1 1 2 2 2 3 3
length 12 div 3 mod 0 (div + 1) * mod = 00  0 0 0 1 1 1 2 2 2 3 3 3
length 13 div 3 mod 1 (div + 1) * mod = 04  0 0 0 0 1 1 1 2 2 2 3 3 3
length 14 div 3 mod 2 (div + 1) * mod = 08  0 0 0 0 1 1 1 1 2 2 2 3 3 3
length 15 div 3 mod 3 (div + 1) * mod = 12  0 0 0 0 1 1 1 1 2 2 2 2 3 3 3
length 16 div 4 mod 0 (div + 1) * mod = 00  0 0 0 0 1 1 1 1 2 2 2 2 3 3 3 3
length 17 div 4 mod 1 (div + 1) * mod = 05  0 0 0 0 0 1 1 1 1 2 2 2 2 3 3 3 3
length 18 div 4 mod 2 (div + 1) * mod = 10  0 0 0 0 0 1 1 1 1 1 2 2 2 2 3 3 3 3
length 19 div 4 mod 3 (div + 1) * mod = 15  0 0 0 0 0 1 1 1 1 1 2 2 2 2 2 3 3 3 3
length 20 div 5 mod 0 (div + 1) * mod = 00  0 0 0 0 0 1 1 1 1 1 2 2 2 2 2 3 3 3 3 3
length 21 div 5 mod 1 (div + 1) * mod = 06  0 0 0 0 0 0 1 1 1 1 1 2 2 2 2 2 3 3 3 3 3
length 22 div 5 mod 2 (div + 1) * mod = 12  0 0 0 0 0 0 1 1 1 1 1 1 2 2 2 2 2 3 3 3 3 3
length 23 div 5 mod 3 (div + 1) * mod = 18  0 0 0 0 0 0 1 1 1 1 1 1 2 2 2 2 2 2 3 3 3 3 3
length 24 div 6 mod 0 (div + 1) * mod = 00  0 0 0 0 0 0 1 1 1 1 1 1 2 2 2 2 2 2 3 3 3 3 3 3
length 25 div 6 mod 1 (div + 1) * mod = 07  0 0 0 0 0 0 0 1 1 1 1 1 1 2 2 2 2 2 2 3 3 3 3 3 3
length 26 div 6 mod 2 (div + 1) * mod = 14  0 0 0 0 0 0 0 1 1 1 1 1 1 1 2 2 2 2 2 2 3 3 3 3 3 3
length 27 div 6 mod 3 (div + 1) * mod = 21  0 0 0 0 0 0 0 1 1 1 1 1 1 1 2 2 2 2 2 2 2 3 3 3 3 3 3
length 28 div 7 mod 0 (div + 1) * mod = 00  0 0 0 0 0 0 0 1 1 1 1 1 1 1 2 2 2 2 2 2 2 3 3 3 3 3 3 3
length 29 div 7 mod 1 (div + 1) * mod = 08  0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 2 2 2 2 2 2 2 3 3 3 3 3 3 3
length 30 div 7 mod 2 (div + 1) * mod = 16  0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 3 3 3 3 3 3 3
length 31 div 7 mod 3 (div + 1) * mod = 24  0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3
length 32 div 8 mod 0 (div + 1) * mod = 00  0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3
length 33 div 8 mod 1 (div + 1) * mod = 09  0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3
length 34 div 8 mod 2 (div + 1) * mod = 18  0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3
length 35 div 8 mod 3 (div + 1) * mod = 27  0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3
length 36 div 9 mod 0 (div + 1) * mod = 00  0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3

*/

static void draw_worm(STATE *st, WORM *s)
{
    int n, div, mod, c;

    // save last worm position and coordinates
    // for clearing later
    for (n = s->length - 1; n >= 0; n--) {
        s->x_prev[n] = s->x[n];
        s->y_prev[n] = s->y[n];
    }
    s->length_prev = s->length;

    // get character interval and draw worm it is
    // assumed that the minimum worm length is 4
    div = s->length / 4;
    mod = s->length % 4;
    for (n = s->length - 1; n >= 0 && div; n--) {
	   c = n < (div + 1) * mod ? n / (div + 1) : (n - mod) / div;
	   worm_put_char(worm_chars[c % 4],
		    s->y[n], s->x[n], worm_colors[s->cpu % 16]);
	   worm_put_char(worm_chars[c % 4],
		    s->y[n], s->x[n] + 1, worm_colors[s->cpu % 16]);
#if VERBOSE
       printw("cpu %d x[n] = %d y[n] = %d n = %d\n",
	      s->cpu, s->x[n], s->y[n], n);
#endif
    }
#if VERBOSE
    printw("\n");
#endif
}

#define NANOSLEEP

static unsigned long run_worms(STATE *st)
{
    register float range, increment;
    int n;

    // reset columns and lines in case the screenw was resized
    st->cols = COLS;
    st->rows = LINES;

    for (n = 0; n < st->cpus; n++) {
        WORM *s = (WORM *) &st->worms[n];

       if (++s->count >= s->limit) {
           s->count = 0;
           grow_worm(st, s);
           move_worm(st, s);
           clear_worm(st, s);
           s->limit = 4 - (s->length / (WORM_MAX_LEN / 4));
#if VERBOSE
           printw("length %d limit %d\n", s->length, s->limit);
#endif
	}

        // update all worms even those sleeping to
	// maintain worm overwrite stacking order
	// when one worm overwrites another during
	// display
        draw_worm(st, s);
        refresh();
    }

    // decrease base wait time if system load increases
    // range is 0-100 load average before reaching
    // minimum st->delay wait time
    n = get_system_load();
#ifdef NANOSLEEP
    range = MAX_NANOSEC - MIN_NANOSEC;
    increment = range / MAX_LOADAVG;
    st->delay = MAX_NANOSEC - (n * increment);
    if (st->delay < MIN_NANOSEC)
       st->delay = MIN_NANOSEC;
    st->delay /= st->divisor;
#else
    range = MAX_MICROSEC - MIN_MICROSEC;
    increment = range / MAX_LOADAVG;
    st->delay = MAX_MICROSEC - (n * increment);
    if (st->delay < MIN_MICROSEC)
        st->delay = MIN_MICROSEC;
    st->delay /= st->divisor;
#endif
#if VERBOSE
    printw("delay %d load(n) = %d\n", st->delay, n);
#endif
    return st->delay;
}

int netware_screensaver(int cpus, int speedup)
{
    register int n, i, ret;
    STATE state, *st = &state;

    memset(st, 0, sizeof(STATE));
    st->cpus = get_processors();
    if (!st->cpus)
       exit(1);

    if (cpus > st->cpus)
       st->cpus = cpus;

    if (st->cpus > MAX_WORMS)
       st->cpus = MAX_WORMS;

    if (speedup > 0)
       st->divisor = speedup;
    else
       st->divisor = 1;

    ret = init_ncurses();
    if (ret < 0)
       return 1;

    // initialize random number generator
    srand(time(0));

    st->cols = COLS;
    st->rows = LINES;
#ifdef NANOSLEEP
    st->delay = MAX_NANOSEC / st->divisor;
#else
    st->delay = MAX_MICROSEC / st->divisor;
#endif

    st->worms = (WORM *)calloc(st->cpus, sizeof(WORM));
    if (!st->worms)
       exit(1);
    memset(st->worms, 0, st->cpus * sizeof(WORM));

    for (n = 0; n < st->cpus; n++) {
        WORM *s = (WORM *)&st->worms[n];

	s->cpu  = n;
        s->x[0] = random() % (COLS - 1);
	s->y[0] = random() % LINES;
	for (i=1; i < WORM_MAX_LEN; i++)
	{
           s->x[i] = s->x[0];
           s->y[i] = s->y[0];
	}
	s->direction = ((random() % 9) >> 1) << 1;
	s->length = WORM_MIN_LEN;
        s->runlength = WORM_MIN_LEN;
#if VERBOSE
	printw(stderr, "worm %d starting at %d,%d dir %d length %d\n",
	       s->cpu, s->x[0], s->y[0], s->direction, s->length);
#endif
        s->length = WORM_MIN_LEN;
    }

    while (!worm_kbhit())
    {
#ifdef NANOSLEEP
       struct timespec ts = { 0, st->delay };
#endif
       run_worms(st);
#ifdef NANOSLEEP
       nanosleep(&ts, NULL);
#else
       usleep(st->delay);
#endif
    }

    if (st->worms)
       free(st->worms);

    clear_ncurses();
    return 0;
}

