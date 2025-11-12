#include "header.h"

extern MEM data[USERNUM];  // main.c で定義されているグローバル変数を参照


void task12(int term_num, int ter_term, NMEM *nmem)
{
    
    output(connect, term_num, RINGBACKTONE);

   
    output(connect, ter_term, RINGINGTONE);

   
    data[term_num].terminal = ter_term;      // 相手端末番号
    data[term_num].or_ter_ident = originate; // 発信側 (1)

    data[ter_term].terminal = term_num;      // 相手端末番号
    data[ter_term].or_ter_ident = terminate; // 着信側 (2)

  
    nmem[0].flag = 1;
    nmem[0].ln = term_num;   // 発端末番号
    nmem[0].state = ringing; // 次状態

  
    nmem[1].flag = 1;
    nmem[1].ln = ter_term;   // 着端末番号
    nmem[1].state = ringing; // 次状態
}
