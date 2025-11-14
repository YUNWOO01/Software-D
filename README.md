`header.h` に定義されている状態番号は以下の通りです。

* **0: `idle`** (待機中)
* **1: `dialtone`** (発信音)
* **2: `ringing`** (呼び出し中 / 着信中)
* **3: `talk`** (通話中)
* **4: `busy`** (話中)

tk_select=制御関数、st_ana=状態分析関数、next_state=状態書き換え関数、output=出力関数、main=main関数
task04=話中処理タスク(相手が話中だった場合に、発信元に話中音 (BUSYTONE) を流し、状態を busy (4) に変更します。)
task12=呼び出し処理タスク(相手を呼び出す処理です。発信元に呼び出し音 (RINGBACKTONE)、着信先に着信音 (RINGINGTONE) を流し、両方の状態を ringing (2) に変更します。)
