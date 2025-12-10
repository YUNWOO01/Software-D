#include "header.h"
#include <stdio.h>

extern MEM data[USERNUM];
extern void output(int, int, int);

void task12(int caller, int callee, NMEM *nmem)
{
    /* 1. 前の音を止める */
    output(disconnect, caller, SR);
    output(disconnect, caller, DIALTONE);

    /* === 呼出し先が発信拒否（or_ac == NO）なら発信側にビジーを送出して終了 === */
    if (callee >= 1 && callee < USERNUM && data[callee].or_ac == NO) {
        /* 任意で表示 */
        fflush(stdout);

        /* 発信側にビジー音を送出 */
        output(connect, caller, BUSYTONE);

        /* 発信側を idle に戻す（呼出し処理を行わない） */
        nmem[0].flag = 1;
        nmem[0].ln = caller;
        nmem[0].state = idle;
        return;
    }

    /* 2. 呼び返し音／呼出し音 */
    output(connect, caller, RINGBACKTONE);
    output(connect, callee, RINGINGTONE);

    /* 3. データ設定 */
    data[caller].terminal = callee;
    data[callee].terminal = caller;

    data[caller].or_ter_ident = originate;  // 発信側
    data[callee].or_ter_ident = terminate;  // 着信側

    /* 4. 次状態予約（両方 ringing） */
    nmem[0].flag = 1;
    nmem[0].ln = caller;
    nmem[0].state = ringing;

    nmem[1].flag = 1;
    nmem[1].ln = callee;
    nmem[1].state = ringing;
}
