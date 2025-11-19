/* * tk_select.c (修正版 3)
 * * tk_select.c 内にあったダミーの next_state の定義を削除
 * (next_state.c を使うため)
 */

#include <stdio.h>
#include "header.h"

/* -----------------------------------------------
 * グローバル変数の参照
 * ----------------------------------------------- */
extern MEM data[USERNUM];



/* -----------------------------------------------
 * tk_select 本体 (仕様書ベース)
 * ----------------------------------------------- */
void tk_select(int term_num, int signal, int dial_num)
{
    NMEM nmem[2];
    nmem[0].flag = 0;
    nmem[1].flag = 0;
    ST_ANA result;
    int ter_term_num = 0; // 相手端末番号 (分析で設定される)

    // 1. 状態分析関数(st_ana)をコール
    result = st_ana(term_num, signal);

    // 2. 分析関数 (anal) の実行
    while (result.task == 0 && result.anal != 0)
    {
        switch (result.anal)
        {
            case OR_ANA_FUNC: // 40 (発信分析)
                {
                    OR_ANA r_or = or_ana(term_num);
                    result.task = r_or.task;
                    result.anal = r_or.anal;
                }
                break;

            case NUM_ANA_FUNC: // 41 (数字分析)
                {
                    NUM_ANA r_num = num_ana(dial_num);
                    result.task = r_num.task;
                    result.anal = r_num.anal;
                    ter_term_num = r_num.info; // 相手端末番号を保持
                }
                break;

            case TER_ANA_FUNC: // 42 (着信分析)
                {
                    TER_ANA r_ter = ter_ana(ter_term_num);
                    result.task = r_ter.task;
                    result.anal = r_ter.anal;
                }
                break;

            default:
                printf(">> ERROR: 不明な分析関数 %d\n", result.anal);
                result.task = TASK04; // エラー時は話中処理へ
                result.anal = 0;
                break;
        }
    }

    // 3. タスク (task) の実行
    switch (result.task)
    {
        // 存在するファイル
        case TASK04: // 504
            task04(term_num, nmem);
            break;
        case TASK12: // 512
            // ter_term_num が 0 の場合 (ringing 中の offhook)
            if (ter_term_num == 0) ter_term_num = data[term_num].terminal; 
            task12(term_num, ter_term_num, nmem);
            break;
        case TASK20:
            task20(term_num, nmem);
            break;  
        case TASK30:
            task30(term_num, nmem);
            break;  
        // ダミーで呼び出すタスク
        case TASK01: task01(term_num, nmem); break;
        case TASK10: task10(term_num, nmem); break;
        case TASK14: task14(term_num, nmem); break;
        case TASK23: task23(term_num, nmem); break;
        case TASK40: task40(term_num, nmem); break;

        default:
            printf(">> ERROR: 不明なタスク %d\n", result.task);
            break;
    }

    // 4. 状態書き換え関数を起動する
    // (header.h 経由で next_state.c の関数が呼び出される)
    next_state(nmem);

    // (デバッグ用) 現在の状態を表示
    printf(">> [現在状態] 1:%d, 2:%d, 3:%d, 4:%d\n", 
           data[1].state, data[2].state, data[3].state, data[4].state);

}
