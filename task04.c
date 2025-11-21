#include <stdio.h>
#include "header.h"

extern MEM data[USERNUM];
extern void output(int, int, int);

void task04(int term_num, NMEM *nmem)
{
    printf("\n 発信が許可されていません！\n");
    output(connect, term_num, BUSYTONE);

    nmem[0].flag  = 1;
    nmem[0].ln    = term_num;
    nmem[0].state = busy;

    nmem[1].flag  = 0;
}
