#include <stdio.h>
#include "header.h"

// main.c 側で定義されているグローバル変数
extern MEM data[USERNUM];

/*
 * 端末の信号入力を受けて状態を更新する関数
 */
void tk_select(int term_num, int signal, int dial_num)
{
    MEM *t = &data[term_num]; // 対象端末

    switch (signal)
    {
        case offhook:
            if (t->state == idle) {
                t->state = dialtone;
                printf("[端末%d] offhook → dialtone に遷移しました。\n", term_num);
            } else {
                printf("[端末%d] offhook信号を受信しましたが、すでに使用中です。\n", term_num);
            }
            break;

        case onhook:
            if (t->state != idle) {
                printf("[端末%d] onhook。通話・発信を終了します。\n", term_num);
                t->state = idle;
                t->or_ter_ident = 0;
            } else {
                printf("[端末%d] すでにidle状態です。\n", term_num);
            }
            break;

        case dial:
            if (t->state == dialtone) {
                // 対象番号を探索
                int i, found = 0;
                for (i = 1; i < USERNUM; i++) {
                    if (data[i].telnum == dial_num) {
                        found = i;
                        break;
                    }
                }

                if (found == 0) {
                    printf("[端末%d] 番号 %d は存在しません。\n", term_num, dial_num);
                } else if (data[found].state != idle) {
                    printf("[端末%d] 相手端末 %d は使用中 (state=%d) です。\n",
                           term_num, found, data[found].state);
                    t->state = busy;
                } else {
                    printf("[端末%d] → [端末%d] に発信しました。\n", term_num, found);
                    t->state = talk;
                    data[found].state = ringing;
                    data[found].or_ter_ident = term_num;
                }
            } else {
                printf("[端末%d] dial信号は dialtone 状態でのみ有効です。\n", term_num);
            }
            break;

        default:
            printf("[端末%d] 不明な信号 (%d) を受信しました。\n", term_num, signal);
            break;
    }
}
