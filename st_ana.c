#include <stdio.h>
#include "header.h"

extern MEM data[USERNUM];
extern void output(int, int, int);

ST_ANA st_ana(int ln, int sig)
{
    ST_ANA r = {0, 0};

    int st = data[ln].state;
    int id = data[ln].or_ter_ident;
    int partner = data[ln].terminal;   // 相手端末番号

    /* ---- idle ---- */
    if (st == idle) {
        if (sig == offhook) { r.anal = OR_ANA_FUNC; return r; }
        if (sig == dial)    { r.task = TASK04;      return r; }
    }

    /* ---- dialtone ---- */
    else if (st == dialtone) {
        if (sig == dial)    { r.anal = NUM_ANA_FUNC; return r; }
        if (sig == onhook)  { r.task = TASK10;       return r; }
    }

    /* ---- ringing ---- */
    else if (st == ringing) {

        /* ★★★ TASK20 を呼ばずに task20 の処理を st_ana 内で実行 ★★★ */
        if (sig == offhook && id == terminate) {

            /* 1. 音を止める */
            output(disconnect, ln, RINGINGTONE);
            output(disconnect, partner, RINGBACKTONE);

            /* 2. 接続メッセージを表示 */
            printf("[%d]と[%d]=>接続\n", partner, ln);

            /* 3. 次状態を talk に設定 */
            r.task = 0;  // task20 を呼ばないためタスクは返さない

            /* next_state のための設定はここではできないので
               data[] の状態を直接更新する */
            data[ln].state = talk;
            data[partner].state = talk;

            return r;
        }

        /* 発信側が切った → 呼出停止 */
        if (sig == onhook && id == originate) {
            r.task = TASK14;
            return r;
        }
    }

    /* ---- talk ---- */
    else if (st == talk) {
        if (sig == onhook)  { r.task = TASK23; return r; }
        if (sig == dial)    { r.task = TASK04; return r; }
    }

    /* ---- busy ---- */
    else if (st == busy) {
        if (sig == onhook)  { r.task = TASK01; return r; }
    }

    return r;
}


