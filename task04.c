#include "header.h"

extern MEM data[USERNUM];
extern void output(int, int, int);

void task04(int ln, NMEM *nmem)
{
    /* 1. 全ての音を止める（存在するものだけ） */
    output(disconnect, ln, SR);
    output(disconnect, ln, DIALTONE);
    output(disconnect, ln, RINGINGTONE);
    output(disconnect, ln, RINGBACKTONE);
    output(disconnect, ln, BUSYTONE);

    /* 2. 状態を idle に戻す */
    nmem[0].flag = 1;
    nmem[0].ln = ln;
    nmem[0].state = idle;
}
