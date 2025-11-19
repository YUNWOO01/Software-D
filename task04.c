#include <stdio.h>
#include "header.h"

void task04(int term_num, NMEM *nmem)
{
    // 指定のメッセージを表示 (先頭にスペースがあることに注意)
    printf(" \n回線が塞がってます！\n");

    /* 発信側にビジートーンを送出 */
    output(connect, term_num, BUSYTONE);

    /* 次状態 busy を設定 */
    nmem[0].flag  = 1;
    nmem[0].ln    = term_num;
    nmem[0].state = busy;

    nmem[1].flag = 0;
}