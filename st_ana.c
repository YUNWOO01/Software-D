#include "header.h"

extern MEM data[USERNUM];

ST_ANA st_ana(int ln, int sig) {
    ST_ANA r;
    r.task = 0;
    r.anal = 0;

    int st = data[ln].state;

    if (st == idle) {
        if (sig == offhook) {
            r.anal = OR_ANA_FUNC;
            return r;
        }
        if (sig == dial) { // 追加: idleでdial操作時のエラー対策
             r.task = TASK04;
             return r;
        }
    }
    else if (st == dialtone) {
        if (sig == dial) {
            r.anal = NUM_ANA_FUNC;
            return r;
        }
        if (sig == onhook) {
            r.task = TASK10;
            return r;
        }
    }
    else if (st == ringing) {
        // 着信応答
        if (sig == offhook && data[ln].or_ter_ident == terminate) {
            r.task = TASK20;
            return r;
        }
        // 発信中止
        if (sig == onhook && data[ln].or_ter_ident == originate) {
            r.task = TASK14;
            return r;
        }
    }
    else if (st == talk) {
        // 通話切断：ログに合わせて TASK23 を使用
        if (sig == onhook) {
            r.task = TASK23; 
            return r;
        }
    }
    else if (st == busy) {
        // 話中からの復帰
        if (sig == onhook) {
            r.task = TASK01; // または TASK10
            return r;
        }
    }
    
    return r;
}