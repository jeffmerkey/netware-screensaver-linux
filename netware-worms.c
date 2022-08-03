/***************************************************************************
*
*   Copyright(c) Jeff V. Merkey 1997-2019.  All rights reserved.
*
*   Portions adapted from xscreensaver loadsnake program is
*   portions Copyright (c) 2007-2011 Cosimo Streppone <cosimo@cpan.org>
*
*   Licensed under the Lesser GNU Public License (LGPL) v2.1.
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

int main(int argc, char *argv[])
{
    int i, cpus, speedup, ret;

    for (i=0, cpus=0, speedup=0; i < argc && argv[i]; i++)
    {
       if (!strcasecmp(argv[i], "help") || !strcasecmp(argv[i], "-h") ||
	   !strcasecmp(argv[i], "-help") || !strcasecmp(argv[i], "--help") ||
	   !strcasecmp(argv[i], "/help") || !strcasecmp(argv[i], "/h")) {
          printf("Copyright (c) 1997-2019 Jeff V. Merkey. All Rights Reserved.\n");
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

