void initialize_data(void)
{
    // data[0] は未使用

    // 端末1 [cite: 36-39]
    data[1].telnum = 4001;
    data[1].state = idle;     // 状態: idle (0)
    data[1].terminal = 0;
    data[1].or_ac = YES;      // 発信許可: YES (30)
    data[1].or_ter_ident = 0;

    // 端末2 [cite: 40-48]
    data[2].telnum = 4002;
    data[2].state = idle;
    data[2].terminal = 0;
    data[2].or_ac = YES;
    data[2].or_ter_ident = 0;

    // 端末3 [cite: 49-52]
    data[3].telnum = 4003;
    data[3].state = idle;
    data[3].terminal = 0;
    data[3].or_ac = YES;
    data[3].or_ter_ident = 0;

    // 端末4 [cite: 53-61]
    data[4].telnum = 4004;
    data[4].state = idle;
    data[4].terminal = 0;
    data[4].or_ac = YES;
    data[4].or_ter_ident = 0;

    printf("端末データを初期化しました。\n");
}

int main(void)
{
    char input_buffer[INPUT_LEN];
    int term_num;  // 端末番号 (1-4)
    char signal_str[INPUT_LEN];
    int signal;    // 信号名 (offhook:0, onhook:1, dial:2)
    int dial_num = 0; // 電話番号 (dial時のみ使用)

    // 1. 端末状態の初期化
    initialize_data();

    printf("\n--- 電話交換サービスシミュレータ --- \n");
    printf("入力を受け付けます。(例: 1 offhook)\n");
    printf("終了する場合は \"exit\" と入力してください。\n");
    printf("------------------------------------\n");

    // 2. メインループ (入力待ち)
    while(1)
    {
        printf("\n> ");
        fflush(stdout);

        // 入力を1行読み込む
        if (fgets(input_buffer, INPUT_LEN, stdin) == NULL) {
            break; // EOF
        }

        // 入力文字列をパース
        // (簡易的なパース処理。sscanfは入力が不完全だと問題が起きるが、
        //  input()関数の代わりとしてシンプルに実装)

        // dial 400x」のように、3つのパラメータが入力されたか試す
        int scan_count = sscanf(input_buffer, "%d %s %d", &term_num, signal_str, &dial_num);

        // "exit" が入力されたら終了
        if (strncmp(input_buffer, "exit", 4) == 0) {
            printf("シミュレータを終了します。\n");
            break;
        }

        // 入力が2つ (例: 1 offhook) または 3つ (例: 1 dial 4002) でない場合
        if (scan_count < 2) {
            printf("入力エラー: 形式が正しくありません。\n");
            printf("例: 1 offhook (端末番号 信号名)\n");
            printf("例: 1 dial 4002 (端末番号 信号名 電話番号)\n");
            continue;
        }

        // 信号文字列を信号番号に変換 [cite: 101]
        if (strcmp(signal_str, "offhook") == 0) {
            signal = offhook; // 0
        } else if (strcmp(signal_str, "onhook") == 0) {
            signal = onhook;  // 1
        } else if (strcmp(signal_str, "dial") == 0) {
            signal = dial;    // 2
            if (scan_count < 3) {
                printf("入力エラー: dial信号には電話番号が必要です。\n");
                continue;
            }
        } else {
            printf("入力エラー: \"%s\" は不明な信号名です。\n", signal_str);
            continue;
        }

        // 端末番号のチェック
        if (term_num < 1 || term_num > 4) {
            printf("入力エラー: 端末番号は 1 から 4 の範囲で指定してください。\n");
            continue;
        }

        printf("--- 入力受信: [端末%d] [信号:%s] [電話番号:%d] ---\n",
               term_num, signal_str, (signal == dial) ? dial_num : 0);

        // 3. tk_select の呼び出し [cite: 6]
        // (dial以外の信号では dial_num (電話番号) は無視される)
        tk_select(term_num, signal, dial_num);

        printf("--- 処理完了: [端末%d] の現在状態: %d ---\n",
               term_num, data[term_num].state);
    }

    return 0;
}
