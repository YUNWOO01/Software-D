#include <stdio.h>
#include "header.h"

extern MEM data[USERNUM];
extern void output(int, int, int);

void task12(int term_num, int ter_term, NMEM *nmem)
{
    int caller = term_num;   // 発端末番号(1〜4)
    int callee = ter_term;   // 着端末番号(1〜4)

    // 1. 前の音を停止（呼び出し側の数字受信＋ダイヤルトーン）
    output(disconnect, caller, SR);
    output(disconnect, caller, DIALTONE);

    // 2. 音を出す
    output(connect, caller, RINGBACKTONE);  // 発信側: 呼び返し音
    output(connect, callee, RINGINGTONE);   // 着信側: 呼出し音

    // 3. 個別データ設定（仕様書どおり）
    // 相手端末番号 → terminal
    // 発着識別子 → or_ter_ident
    data[caller].terminal      = callee;
    data[caller].or_ter_ident  = originate;  // 発信側
    // data[caller].or_ac は触らない（発信許可のまま）

    data[callee].terminal      = caller;
    data[callee].or_ter_ident  = terminate;  // 着信側
    // data[callee].or_ac も触らない

    // デバッグ（必要なら残す）
    printf("[DEBUG] task12 after setting callee: caller=%d callee=%d | "
           "callee.terminal=%d or_ter_ident=%d\n",
           caller, callee, data[callee].terminal, data[callee].or_ter_ident);

    // 4. 次状態予約（ln はそのまま 1〜4 を使う）
    nmem[0].flag  = 1;
    nmem[0].ln    = caller;
    nmem[0].state = ringing;

    nmem[1].flag  = 1;
    nmem[1].ln    = callee;
    nmem[1].state = ringing;
}
