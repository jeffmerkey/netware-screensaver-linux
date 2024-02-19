/***************************************************************************
*
*   Copyright(c) Jeff V. Merkey 1997-2022.  All rights reserved.
*
*   Portions adapted from xscreensaver loadsnake program is
*   portions Copyright (c) 2007-2011 Cosimo Streppone <cosimo@cpan.org>
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

int main(int argc, char *argv[])
{
    int i, cpus, speedup, ret;

    for (i=0, cpus=0, speedup=0; i < argc && argv[i]; i++)
    {
       if (!strcasecmp(argv[i], "help") || !strcasecmp(argv[i], "-h") ||
	   !strcasecmp(argv[i], "-help") || !strcasecmp(argv[i], "--help") ||
	   !strcasecmp(argv[i], "/help") || !strcasecmp(argv[i], "/h")) {
          printf("Copyright (c) 1997-2022 Jeff V. Merkey. All Rights Reserved.\n");
          printf("USAGE:  netware-worms [cpus|speedup]\n");
          printf("        (i.e. netware-worms cpus=<num> speedup=<divisor>)\n");
          printf("examples:\n");
          printf("   netware-worms cpus=8\n");
          printf("       start screensaver with 8 worm threads\n");
          printf("   netware-worms speedup=4\n");
          printf("       run screensaver at 4X speed\n");
          exit(0);
       }

       if (!strncasecmp(argv[i], "cpus=", 5))
          cpus = atoi(&argv[i][5]);

       if (!strncasecmp(argv[i], "speedup=", 8)) {
          if (argv[i][8])
             speedup = atoi(&argv[i][8]);
       }
    }
    ret = netware_screensaver(cpus, speedup);
    exit(ret);
}

