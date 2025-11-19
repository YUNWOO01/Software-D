#include "header.h"

/* Use the shared terminal state table defined elsewhere */
extern MEM data[USERNUM];

/*
 * State analysis function
 * Decide the next action from (current state of ln) + (incoming signal).
 * Return:
 *   - task != 0 and anal == 0      -> execute this task next
 *   - task == 0 and anal != 0      -> run the indicated analysis function next
 *   - task == 0 and anal == 0      -> undecided (controller may handle)
 */
ST_ANA st_ana(int ln, int sig) {
    ST_ANA r;
    r.task = 0;
    r.anal = 0;

    int st = data[ln].state;

    if (st == idle) {
        if (sig == offhook) {
            /* Lift handset from idle: check originate permission next */
            r.anal = OR_ANA_FUNC;   /* or_ana() */
            return r;
        }
        if (sig == dial) {
            /* Dial while idle (no dialtone): treat as cancel */
            r.task = TASK04;        /* hang-up/cancel */
            return r;
        }
    }
    else if (st == dialtone) {
        if (sig == dial) {
            /* User starts entering digits: analyze number next */
            r.anal = NUM_ANA_FUNC;  /* num_ana() */
            return r;
        }
        if (sig == onhook) {
            /* Hang up while in dialtone: clear originate side */
            r.task = TASK10;
            return r;
        }
    }
    else if (st == ringing) {
        if (sig == offhook && data[ln].or_ter_ident == terminate) {
            /* Terminating side answers: go to connect (talk) */
            r.task = TASK20;
            return r;
        }
        if (sig == onhook && data[ln].or_ter_ident == originate) {
            /* Originating side hangs up during ringing: cancel */
            r.task = TASK14;
            return r;
        }
    }
    else if (st == talk) {
        if (sig == onhook) {
            /* During conversation, a side hangs up: end according to side */
            r.task = (data[ln].or_ter_ident == originate) ? TASK30 : TASK40;
            return r;
        }
    }
    else if (st == busy) {
        if (sig == onhook) {
            /* Clear busy state back to idle (system policy) */
            r.task = TASK10;
            return r;
        }
    }

    /* Fail-safe: if we reach here, choose a safe cancel or leave undecided. */
    r.task = TASK14;   /* conservative cancel */
    r.anal = 0;
    return r;
}
