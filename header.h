/***************************
　ヘッダファイル (header.h)
***************************/
/*最大端末数*/
#define USERNUM	5

#define	INPUT_LEN	256


/*状態番号*/
#define idle 0
#define dialtone 1
#define ringing 2
#define talk 3
#define busy 4

/*イベント番号*/
#define offhook 0
#define onhook 1
#define dial 2

/*発信拒否サービス*/
#define NO 20
#define YES 30

/*パス接続/切断要求番号*/
#define connect 1
#define disconnect 0

/*装置番号*/
#define SR 10
#define RINGINGTONE 11
#define RINGBACKTONE 12
#define BUSYTONE 13
#define DIALTONE 14

/*発着識別子*/
#define originate 1
#define terminate 2

/*分析関数番号*/
#define OR_ANA_FUNC  40
#define NUM_ANA_FUNC 41
#define TER_ANA_FUNC 42

/*タスク番号*/
#define TASK01 501
#define TASK04 504
#define TASK10 510
#define TASK12 512
#define TASK14 514
#define TASK20 520
#define TASK23 523
#define TASK30 530
#define TASK40 540

/*構造体型宣言*/
typedef struct{
	int telnum;
	int state;
	int terminal;
	int or_ac;
	int or_ter_ident;
}MEM;

typedef struct{
	int task;
	int anal;
}OR_ANA;

typedef struct{
	int task;
	int anal;
}ST_ANA;

typedef struct{
	int task;
	int anal;
}ST_ANA_TABLE;

typedef struct{
	int flag;
	int ln;
	int state;
}NMEM;

typedef struct{
	int task;
	int anal;
	int info;
}NUM_ANA;

typedef struct{
	int task;
	int anal;
}TER_ANA;

/*プロトタイプ宣言*/
void next_state(NMEM*);
ST_ANA	st_ana(int,int);
NUM_ANA	num_ana(int);
OR_ANA	or_ana(int);
TER_ANA	ter_ana(int);
void output(int,int,int);
void task01(int,NMEM*);
void task04(int,NMEM*);
void task10(int,NMEM*);
void task12(int,int,NMEM*);
void task14(int,NMEM*);
void task20(int,NMEM*);
void task23(int,NMEM*);
void task30(int,NMEM*);
void task40(int,NMEM*);
void task50(NMEM *nmem);
void tk_select(int,int,int);

