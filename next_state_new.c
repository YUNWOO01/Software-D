#include "header.h"

extern MEM data[USERNUM];

void next_state(NMEM *nm)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (nm[i].flag == 1) {

            int ln = nm[i].ln;
            int st = nm[i].state;

            if (ln >= 1 && ln <= 4) {
                data[ln].state = st;
            }

            nm[i].flag = 0;
        }
    }
}
