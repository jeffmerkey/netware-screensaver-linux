/***************************************************************************
*
*   Copyright(c) Jeff V. Merkey 1997-2019.  All rights reserved.
*
*   Licensed under the GNU Public License v2.
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
    int usr[MAX_WORMS];
    int sys[MAX_WORMS];
    int nice[MAX_WORMS];
    int idle[MAX_WORMS];
    WORM *worms;
} STATE;

int netware_screensaver(int cpus, int speedup);
