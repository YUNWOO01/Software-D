#include "header.h"

void task04(int term_num, NMEM *nmem)
{
   
    output(connect, term_num, BUSYTONE);

   
    nmem[0].flag = 1;     // 書き換えフラグ: ON
    nmem[0].ln   = term_num; // 対象端末番号
    nmem[0].state = busy; // 次状態: busy (4)

    // nmem[1] (着端末側) は未使用
    nmem[1].flag = 0;
}
