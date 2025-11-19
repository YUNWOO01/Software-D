

#include <stdio.h>
#include "header.h"


extern MEM data[USERNUM];


void task30(int term_num, NMEM *nmem)
{
    int terminate_term; // 相手端末番号 (着信側)

       terminate_term = data[term_num].terminal;

        output(disconnect, term_num, terminate_term); 

    
    
    //   nmem[0]: 切断した端末 (発信側) の状態を idle (0) に設定
    nmem[0].flag = 1;
    nmem[0].ln   = term_num;
    nmem[0].state = idle; // 状態: idle (0)

    //   nmem[1]: 相手端末 (着信側) の状態を idle (0) に設定
    nmem[1].flag = 1;
    nmem[1].ln   = terminate_term;
    nmem[1].state = idle; // 状態: idle (0)

    // 4. グローバルデータのクリア (次回の発信に備えて、相手情報などをリセット)
    data[term_num].terminal = 0;
    data[term_num].or_ter_ident = 0;
    data[terminate_term].terminal = 0;
    data[terminate_term].or_ter_ident = 0;

    printf(">> TASK30: 通話終了。両端末 (端末%d, 端末%d) を待機状態に戻しました。\n", term_num, terminate_term);
}
