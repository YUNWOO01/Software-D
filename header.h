/***************************
    Header file (header.h)
***************************/

/* ----- Maximum number of terminals ----- */
#define USERNUM 5

/* ----- Maximum input string length ----- */
#define INPUT_LEN 256

/* ----- State numbers (terminal states) ----- */
#define idle 0        /* idle / on-hook */
#define dialtone 1    /* dial tone (ready to dial) */
#define ringing 2     /* ringing state */
#define talk 3        /* in conversation */
#define busy 4        /* busy or occupied */

/* ----- Event numbers (input signals) ----- */
#define offhook 0     /* handset lifted */
#define onhook 1      /* handset put down */
#define dial 2        /* dialing digits */

/* ----- Outgoing call permission identifiers ----- */
#define NO 20         /* not permitted to originate */
#define YES 30        /* permitted to originate */

/* ----- Connection / disconnection request identifiers ----- */
#define connect 1
#define disconnect 0

/* ----- Device numbers (used in output()) ----- */
#define SR 10            /* switch or relay */
#define RINGINGTONE 11   /* tone generator: ringing tone */
#define RINGBACKTONE 12  /* tone generator: ringback tone */
#define BUSYTONE 13      /* tone generator: busy tone */
#define DIALTONE 14      /* tone generator: dial tone */

/* ----- Originate / Terminate identifiers ----- */
#define originate 1      /* originating side */
#define terminate 2      /* terminating side */

/* ----- Analysis function numbers ----- */
#define OR_ANA_FUNC  40  /* originate analysis */
#define NUM_ANA_FUNC 41  /* number analysis */
#define TER_ANA_FUNC 42  /* terminate analysis */

/* ----- Task numbers ----- */
#define TASK01 501
#define TASK04 504
#define TASK10 510
#define TASK12 512
#define TASK14 514
#define TASK20 520
#define TASK23 523
#define TASK30 530
#define TASK40 540

/* ----- Structure declarations ----- */

/* Basic terminal information (used in input()/system data) */
typedef struct {
	int telnum;         /* telephone number (e.g., 4001–4004) */
	int state;          /* current state of the terminal */
	int terminal;       /* connected terminal number (1–4) */
	int or_ac;          /* originate permission (NO/YES) */
	int or_ter_ident;   /* originate or terminate identifier */
} MEM;

/* Originate analysis result */
typedef struct {
	int task;           /* task number */
	int anal;           /* analysis function number */
} OR_ANA;

/* State analysis result */
typedef struct {
	int task;           /* task number */
	int anal;           /* analysis function number */
} ST_ANA;

/* Table form of state analysis result (used internally) */
typedef struct {
	int task;           /* task number */
	int anal;           /* analysis function number */
} ST_ANA_TABLE;

/* State rewrite data (used for updating states) */
typedef struct {
	int flag;           /* 1 = rewrite, 0 = no change */
	int ln;             /* terminal line number to rewrite */
	int state;          /* next state */
} NMEM;

/* Number analysis result */
typedef struct {
	int task;           /* task number */
	int anal;           /* analysis function number */
	int info;           /* destination terminal number */
} NUM_ANA;

/* Terminate analysis result */
typedef struct {
	int task;           /* task number */
	int anal;           /* analysis function number */
} TER_ANA;

/* ----- Function prototypes ----- */
void next_state(NMEM*);                    /* state rewrite function */
ST_ANA st_ana(int, int);                   /* state analysis function */
NUM_ANA num_ana(int);                      /* number analysis */
OR_ANA or_ana(int);                        /* originate analysis */
TER_ANA ter_ana(int);                      /* terminate analysis */
void output(int, int, int);                /* connect/disconnect I/O */
void task01(int, NMEM*);
void task04(int, NMEM*);
void task10(int, NMEM*);
void task12(int, int, NMEM*);
void task14(int, NMEM*);
void task20(int, NMEM*);
void task23(int, NMEM*);
void task30(int, NMEM*);
void task40(int, NMEM*);
void tk_select(int, int, int);             /* main task selector */