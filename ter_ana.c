#include <stdio.h>
#include "header.h"

extern MEM data[USERNUM];
extern void output(int mode, int ln, int tone);
extern void offer_voicemail(int caller, int callee);

TER_ANA ter_ana(int ln)
{
    TER_ANA r = {0};
    
    int st     = data[ln].state;       // 着信側の状態
    int caller = data[ln].terminal;    // 発信側の端末番号（期待値）
    int callee = ln;                   // 着信側の端末番号（= ln）

    /* ---- caller が未設定（0）のときは探索で補完する ---- */
    if (caller == 0) {
        /* 優先：誰かの terminal が callee を指していればそれを caller とする */
        for (int i = 1; i < USERNUM; ++i) {
            if (i == callee) continue;
            if (data[i].terminal == callee) {
                caller = i;
                break;
            }
        }
    }
    if (caller == 0) {
        /* 次に検出：ダイヤル操作や発信中っぽい端末（dialtone / ringing）を探す */
        for (int i = 1; i < USERNUM; ++i) {
            if (i == callee) continue;
            if (data[i].state == dialtone || data[i].state == ringing) {
                caller = i;
                break;
            }
        }
    }

    /* ------------------------------
       idle: 呼出開始 task12
    ------------------------------ */
    if (st == idle) {
        r.task = TASK12;
        return r;
    }

    /* ------------------------------
       ringing: 呼出し中 → task04（切断）
    ------------------------------ */
    if (st == ringing) {
        r.task = TASK04;
        return r;
    }

    /* ------------------------------
       talk: 通話中 → ビジー＆留守電
    ------------------------------ */
    if (st == talk) {

        printf("回線が塞がっています！\n");
        fflush(stdout);

        /* 発信側（caller）へビジー音を送出する（caller が有効な場合） */
        if (caller >= 1 && caller < USERNUM) {
            output(connect, caller, BUSYTONE);
        }

        /* 留守電（caller が 0 の場合は保存時の from は 0 になる） */
        offer_voicemail(caller, callee);

        r.task = TASK04;
        return r;
    }

    /* ------------------------------
       busy: 応答不可 → ビジー＆留守電
    ------------------------------ */
    if (st == busy) {

        printf("回線が塞がっています！\n");
        fflush(stdout);

        if (caller >= 1 && caller < USERNUM) {
            output(connect, caller, BUSYTONE);
        }

        offer_voicemail(caller, callee);

        r.task = TASK04;
        return r;
    }

    return r;
}
