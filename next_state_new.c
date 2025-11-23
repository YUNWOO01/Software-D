#include "header.h"

extern MEM data[USERNUM];

void next_state(NMEM *nmem)
{
    for (int i = 0; i < 3; i++) {

        if (nmem[i].flag == 1) {
            int ln = nmem[i].ln;
            int st = nmem[i].state;

            if (ln >= 1 && ln <= USERNUM)
                data[ln].state = st;

            nmem[i].flag = 0;
        }
    }
}
