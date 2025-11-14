#include <stdio.h>
#include "header.h"

extern MEM data[USERNUM];


// 不足している分析関数 (ダミー)
OR_ANA or_ana(int term_num)
{
    OR_ANA r;
    printf(">> ANA (DUMMY): or_ana を実行。発信を許可します。\n");
    r.task = TASK01; // (仮) TASK01 (発信音) を呼び出す
    r.anal = 0;
    return r;
}

NUM_ANA num_ana(int dial_num)
{
    NUM_ANA r;
    printf(">> ANA (DUMMY): num_ana を実行。番号 %d を分析。\n", dial_num);
    
    // (仮) 相手端末を検索
    int ter_term = 0;
    for (int i = 1; i < USERNUM; i++) {
        if (data[i].telnum == dial_num) ter_term = i;
    }
    
    if (ter_term == 0) { // 番号なし
        r.task = TASK14; // (仮) 番号エラータスク
        r.anal = 0;
        r.info = 0; 
    } else {
        r.task = 0; // タスク未定
        r.anal = TER_ANA_FUNC; // 次に ter_ana を呼ぶ
        r.info = ter_term; // 相手端末番号を渡す
    }
    return r;
}

TER_ANA ter_ana(int ter_term)
{
    TER_ANA r;
    printf(">> ANA (DUMMY): ter_ana を実行。相手端末 %d をチェック。\n", ter_term);

    if (data[ter_term].state == idle) {
        r.task = TASK12; // 呼び出しタスク (task12.c が存在する)
    } else {
        r.task = TASK04; // 話中タスク (task04.c が存在する)
    }
    r.anal = 0;
    return r;
}

// 不足しているタスク関数 (ダミー)
void task01(int term_num, NMEM *nmem) {
    printf(">> TASK (DUMMY): task01 を実行 (発信音)\n");
    output(connect, term_num, DIALTONE);
    nmem[0].flag = 1; nmem[0].ln = term_num; nmem[0].state = dialtone;
}
void task10(int term_num, NMEM *nmem) {
    printf(">> TASK (DUMMY): task10 を実行 (通話終了)\n");
    output(disconnect, term_num, 0);
    nmem[0].flag = 1; nmem[0].ln = term_num; nmem[0].state = idle;
}
void task14(int term_num, NMEM *nmem) {
    printf(">> TASK (DUMMY): task14 を実行 (番号エラー)\n");
    output(connect, term_num, BUSYTONE); // (仮) 話中音を鳴らす
    nmem[0].flag = 1; nmem[0].ln = term_num; nmem[0].state = busy;
}
void task20(int term_num, NMEM *nmem) { /* ダミー */ }
void task23(int term_num, NMEM *nmem) { /* ダミー */ }
void task30(int term_num, NMEM *nmem) { /* ダミー */ }
void task40(int term_num, NMEM *nmem) { /* ダミー */ }

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
            
        // ダミーで呼び出すタスク
        case TASK01: task01(term_num, nmem); break;
        case TASK10: task10(term_num, nmem); break;
        case TASK14: task14(term_num, nmem); break;
        case TASK20: task20(term_num, nmem); break;
        case TASK23: task23(term_num, nmem); break;
        case TASK30: task30(term_num, nmem); break;
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
