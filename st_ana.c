#include <stdio.h>
#include "header.h"

extern MEM data[USERNUM];
extern void output(int, int, int);

ST_ANA st_ana(int ln, int sig)
{
    ST_ANA r = (ST_ANA){0, 0};

    int st      = data[ln].state;
    int id      = data[ln].or_ter_ident;
    int partner = data[ln].terminal;

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

        /* ★★★ 着信側 offhook → 接続（task20 を st_ana 内で実行） ★★★ */
        if (sig == offhook && id == terminate) {

            /* 1. 音を止める */
            output(disconnect, ln,      RINGINGTONE);
            output(disconnect, partner, RINGBACKTONE);

            /* 2. 接続メッセージ */
            printf("[%d]と[%d]=>接続\n", partner, ln);

            /* 3. 両方を通話状態へ */
            data[ln].state      = talk;
            data[partner].state = talk;

            return r;
        }

        /* 発信側が onhook → 呼出停止 */
        if (sig == onhook && id == originate) {
            r.task = TASK14;
            return r;
        }
    }

    /* ---- talk ---- */
    else if (st == talk) {

        /* ★★★ 通話中に onhook → 切断（task23 を st_ana 内で実行） ★★★ */
        if (sig == onhook) {

            /* 1. 切断メッセージ */
            printf("[%d]と[%d]=>切断\n", ln, partner);

            /* 2. ビジー音（相手側に送出） */
            printf("[%d]=>ビジー音送出\n", partner);

            /* 3. 状態終了 */
            data[ln].state      = idle;
            data[partner].state = idle;

            return r;
        }

        /* 話中にダイヤル → エラー */
        if (sig == dial) {
            r.task = TASK04;
            return r;
        }
    }

    /* ---- busy ---- */
    else if (st == busy) {
        if (sig == onhook) {
            r.task = TASK01;
            return r;
        }
    }

    return r;
}
