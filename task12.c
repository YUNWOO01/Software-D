#include "header.h"

extern MEM data[USERNUM];

void task12(int term_num, int ter_term, NMEM *nmem)
{
    // 1. 前の音を切断 (ログに合わせる)
    output(disconnect, term_num, SR);       // 数字受信装置切断
    output(disconnect, term_num, DIALTONE); // ダイアルトーン切断

    // 2. 呼び出し音の送出
    output(connect, term_num, RINGBACKTONE); // 発信者へPrrr
    output(connect, ter_term, RINGINGTONE);  // 着信者へRinging

    // 3. 端末情報の結合と設定 (task20のために必須)
    // --- 発信者 ---
    data[term_num].terminal = ter_term;
    data[term_num].or_ter_ident = originate;
    data[term_num].or_ac = NO;
    
    // --- 着信者 ---
    data[ter_term].terminal = term_num;
    data[ter_term].or_ter_ident = terminate;
    data[ter_term].or_ac = NO;

    // 4. 次状態 (ringing) の予約
    nmem[0].flag = 1;
    nmem[0].ln = term_num;
    nmem[0].state = ringing;

    nmem[1].flag = 1;
    nmem[1].ln = ter_term;
    nmem[1].state = ringing;
}