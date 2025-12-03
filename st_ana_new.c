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
        if (sig == offhook) {
            if (data[ln].or_ac == NO) {
                printf("[%d]=>ビジー音送出\n", ln);
                r.task = TASK04;
                return r;
            }
            r.anal = OR_ANA_FUNC; 
            return r;
        }
        if (sig == dial)    { r.task = TASK04;      return r; }
        if (sig == onhook)  { printf("イベント無効です！\n"); r.task = 0; return r;}
    }

    /* ---- dialtone ---- */
    else if (st == dialtone) {
        if (sig == dial)    { r.anal = NUM_ANA_FUNC; return r; }
        if (sig == onhook)  { r.task = TASK10;       return r; }
        if (sig == offhook)  { printf("イベント無効です！\n"); r.task = 0; return r;}
    }

    /* ---- ringing ---- */
    else if (st == ringing) {

        
        if (sig == offhook && id == terminate) {
            r.task=TASK23;

            return r;
        }

        /* 発信側が onhook → 呼出停止 */
        if (sig == onhook && id == originate) {
            r.task = TASK14;
            return r;
        }

        if (sig == offhook) {
            printf("イベント無効です！\n");
            r.task = 0;
            return r;
        }
    }

    /* ---- talk ---- */
    else if (st == talk) {

        if (sig == onhook) {
            r.task=TASK30;

            return r;
        }

        /* 話中にダイヤル → エラー */
        if (sig == dial) {
            printf("イベント無効です！\n");
            r.task = 0;
            return r;
        }
    }

    /* ---- busy ---- */
    else if (st == busy) {
        if (sig == onhook) {
            r.task = TASK40;
            return r;
        }
    }

    return r;
}
