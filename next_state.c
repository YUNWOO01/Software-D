#include <stdio.h>
#include "header.h"

extern MEM data[USERNUM];

void next_state(NMEM *nmem)
{
    // nmem[0] (主に発信側 or 単独) の処理
    if (nmem[0].flag == 1) 
    {
        int ln = nmem[0].ln;
        int new_state = nmem[0].state;

        // 端末番号が有効な範囲かチェック
        if (ln >= 1 && ln < USERNUM) {
            printf(">> STATE: 端末%d の状態を %d に変更します。\n", ln, new_state);
            data[ln].state = new_state;
        } else {
            printf(">> STATE ERROR: 端末%d は不正な番号です。\n", ln);
        }

        // 処理が終わったらフラグをリセット (必須ではないが安全)
        nmem[0].flag = 0;
    }

    // nmem[1] (主に着信側) の処理
    if (nmem[1].flag == 1) 
    {
        int ln = nmem[1].ln;
        int new_state = nmem[1].state;

        if (ln >= 1 && ln < USERNUM) {
            printf(">> STATE: 端末%d の状態を %d に変更します。\n", ln, new_state);
            data[ln].state = new_state;
        } else {
            printf(">> STATE ERROR: 端末%d は不正な番号です。\n", ln);
        }

        // 処理が終わったらフラグをリセット
        nmem[1].flag = 0;
    }
}
