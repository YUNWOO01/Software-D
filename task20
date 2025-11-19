#include <stdio.h>
#include "header.h"

extern MEM data[USERNUM];

void task20(int ln, NMEM *nmem)
{
    int partner = data[ln].terminal;

    /* 1. 音を止める */
    output(disconnect, ln, RINGINGTONE);
    output(disconnect, partner, RINGBACKTONE);

    /* 2. 接続メッセージを表示 */
    /* ログの形式 "[1]と[2]=>接続" に合わせる */
    printf("[%d]と[%d]=>接続\n", partner, ln);

    /* 3. 両方を talk (通話中) にする */
    nmem[0].flag = 1;
    nmem[0].ln = ln;
    nmem[0].state = talk;

    nmem[1].flag = 1;
    nmem[1].ln = partner;
    nmem[1].state = talk;
}
